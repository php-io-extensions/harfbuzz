<?php

declare(strict_types=1);

if (! extension_loaded('harfbuzz')) {
    throw new RuntimeException('The harfbuzz extension is not loaded; run ./install-macos.sh or ./install-debian-trixie.sh');
}

/** The monospace test font: Menlo (face 0 of the collection) on macOS, DejaVu Sans Mono on Linux. */
function monoFont(): string
{
    return PHP_OS_FAMILY === 'Darwin'
        ? '/System/Library/Fonts/Menlo.ttc'
        : '/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf';
}

/** The loaded HarfBuzz library's version, as the extension's info table shows it. */
function hbVersion(): string
{
    ob_start();
    (new ReflectionExtension('harfbuzz'))->info();
    $info = (string) ob_get_clean();

    return preg_match('/HarfBuzz library => (\S+)/', $info, $m) === 1 ? $m[1] : '';
}

/** Signal numbers a child process dies by (SIGBUS is 10 on macOS, 7 on Linux). */
const PROBE_SIGBUS = PHP_OS_FAMILY === 'Darwin' ? 10 : 7;
const PROBE_SIGSEGV = 11;

/**
 * Skips a tests/Probes test unless the loaded ext-harfbuzz is a probe build (--enable-harfbuzz-probes),
 * the only build where env PHPHB_PROBE_OFF turns a kept refusal off.
 */
function requireProbeBuild(): void
{
    if (! defined('PHPHB_PROBES')) {
        test()->markTestSkipped('ext-harfbuzz is not a probe build (configure --enable-harfbuzz-probes): PHPHB_PROBES is undefined');
    }
}

/**
 * Skips a probe whose crash is a short read past PHP memory: only libgmalloc's guard pages (macOS) make it
 * fault every time; elsewhere it lands in mapped heap or not as the allocator lays it out.
 */
function requireGuardPages(): void
{
    if (PHP_OS_FAMILY !== 'Darwin') {
        test()->markTestSkipped('the probe faults its over-read at a libgmalloc guard page, which only macOS has');
    }
}

/**
 * Runs $code in a child of this PHP, with the same ini configuration, after it opens the mono
 * face at 32 pixels ($face), an hb-ft font over it ($font) and an empty buffer ($buffer). The code is a
 * function body that uses those three: what it returns comes back as 'result' (JSON), a throw as 'threw'
 * ("Class: message"). $siteOff names the kept site env PHPHB_PROBE_OFF turns off (a probe build only);
 * null keeps every site. The child runs under libgmalloc with Zend's allocator off on macOS, so a read of
 * freed or short memory faults.
 *
 * @return array{signal: ?int, exit: ?int, result: mixed, threw: ?string, survived: bool, stdout: string, stderr: string}
 */
function hbChild(string $code, ?string $siteOff = null): array
{
    $font = monoFont();
    $script = <<<PHP
    <?php
    declare(strict_types=1);
    FT_Init_FreeType(\$library);
    FT_New_Face(\$library, '{$font}', 0, \$face);
    FT_Set_Pixel_Sizes(\$face, 0, 32);
    \$font = hb_ft_font_create_referenced(\$face);
    \$buffer = hb_buffer_create();
    try {
        \$result = (static function () use (\$library, \$face, \$font, \$buffer) {
    {$code}
        })();
        echo "\\nRESULT ", json_encode(\$result, JSON_PRESERVE_ZERO_FRACTION | JSON_PARTIAL_OUTPUT_ON_ERROR), "\\n";
    } catch (Throwable \$t) {
        echo "\\nREFUSED ", get_class(\$t), ': ', \$t->getMessage(), "\\n";
    }
    echo "SURVIVED\\n";
    PHP;

    $base = tempnam(sys_get_temp_dir(), 'hb-child');
    $file = $base.'.php';
    file_put_contents($file, $script);

    $env = getenv();
    unset($env['PHPHB_PROBE_OFF'], $env['DYLD_INSERT_LIBRARIES'], $env['USE_ZEND_ALLOC']);
    $env['XDEBUG_MODE'] = 'off';
    if ($siteOff !== null) {
        $env['PHPHB_PROBE_OFF'] = $siteOff;
    }
    if (PHP_OS_FAMILY === 'Darwin') {
        $env['DYLD_INSERT_LIBRARIES'] = '/usr/lib/libgmalloc.dylib';
        $env['USE_ZEND_ALLOC'] = '0';
    }

    // No -n: the child reads the same ini files and PHP_INI_SCAN_DIR as this PHP, so it loads the same modules.
    $process = proc_open([PHP_BINARY, '-d', 'memory_limit=128M', $file],
        [0 => ['file', '/dev/null', 'r'], 1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes, null, $env);
    $stdout = (string) stream_get_contents($pipes[1]);
    $stderr = (string) stream_get_contents($pipes[2]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    do {
        $status = proc_get_status($process);
    } while ($status['running'] && usleep(10_000) === null);
    proc_close($process);
    @unlink($base);
    @unlink($file);

    return [
        'signal' => $status['signaled'] ? $status['termsig'] : null,
        'exit' => $status['signaled'] ? null : $status['exitcode'],
        'result' => preg_match('/^RESULT (.*)$/m', $stdout, $m) === 1 ? json_decode($m[1], true) : null,
        'threw' => preg_match('/^REFUSED (.*)$/m', $stdout, $m) === 1 ? $m[1] : null,
        'survived' => str_contains($stdout, "SURVIVED\n"),
        'stdout' => $stdout,
        'stderr' => $stderr,
    ];
}

/**
 * The mono face at 32 pixels through ext-freetype, an hb-ft font over it, and a buffer holding $text
 * with its segment properties guessed. The library and face live as long as the font.
 *
 * @return array{0: hb_font_t, 1: hb_buffer_t, 2: FT_Face}
 */
function monoShaper(string $text): array
{
    static $faces = [];

    FT_Init_FreeType($library) === 0 || throw new RuntimeException('FT_Init_FreeType failed');
    FT_New_Face($library, monoFont(), 0, $face) === 0 || throw new RuntimeException('FT_New_Face failed for '.monoFont());
    FT_Set_Pixel_Sizes($face, 0, 32);
    $faces[] = [$library, $face];

    $font = hb_ft_font_create_referenced($face);
    $buffer = hb_buffer_create();
    hb_buffer_add_utf8($buffer, $text, -1, 0, -1);
    hb_buffer_guess_segment_properties($buffer);

    return [$font, $buffer, $face];
}
