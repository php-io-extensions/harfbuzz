<?php

declare(strict_types=1);

// ext-freetype is optional: without it ext-harfbuzz loads and shapes with its own font loading, and hb-ft
// (the functions that take or need an ext-freetype face) is not registered.

it('loads without ext-freetype, shapes with hb_font_create, and registers no hb-ft function', function (): void {
    $code = sprintf(<<<'PHP'
        $font = hb_font_create(hb_face_create(hb_blob_create_from_file('%s'), 0));
        $buffer = hb_buffer_create();
        hb_buffer_add_utf8($buffer, 'ab', -1, 0, -1);
        hb_buffer_guess_segment_properties($buffer);
        hb_shape($font, $buffer, null, 0);
        echo json_encode([
            'freetype' => extension_loaded('freetype'),
            'hb_ft_font_create_referenced' => function_exists('hb_ft_font_create_referenced'),
            'hb_ft_font_changed' => function_exists('hb_ft_font_changed'),
            'advances' => array_map(fn (hb_glyph_position_t $p): int => $p->x_advance, hb_buffer_get_glyph_positions($buffer, $count)),
        ]);
    PHP, monoFont());

    $process = proc_open([PHP_BINARY, '-n', '-d', 'extension='.harfbuzzModule(), '-r', $code],
        [0 => ['file', '/dev/null', 'r'], 1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes);
    $stdout = (string) stream_get_contents($pipes[1]);
    $stderr = (string) stream_get_contents($pipes[2]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    $exit = proc_close($process);

    expect([$exit, $stderr, json_decode($stdout, true)])->toBe([0, '', [
        'freetype' => false,
        'hb_ft_font_create_referenced' => false,
        'hb_ft_font_changed' => false,
        'advances' => [1233, 1233],
    ]]);
});

it('registers hb-ft when ext-freetype is loaded', function (): void {
    expect(extension_loaded('freetype'))->toBeTrue()
        ->and(function_exists('hb_ft_font_create_referenced'))->toBeTrue()
        ->and(function_exists('hb_ft_font_changed'))->toBeTrue();
});
