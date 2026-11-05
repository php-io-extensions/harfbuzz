<?php

declare(strict_types=1);

// Site harfbuzz-null-handle (outcome 1): HarfBuzz dereferences the hb_buffer_t and hb_font_t it is given.
// A handle that holds NULL (destroyed by hb_buffer_destroy / hb_font_destroy, or made by fromPointer(0))
// gives it NULL.

beforeEach(fn () => requireProbeBuild());

it('refuses a destroyed buffer, which HarfBuzz dereferences (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        hb_buffer_destroy($buffer);
        hb_buffer_add_utf8($buffer, 'abc', -1, 0, -1);

        return 'added';
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-null-handle');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: hb_buffer_add_utf8(): Argument #1 ($buffer) holds NULL: it was destroyed, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});

it('refuses a destroyed font, which HarfBuzz dereferences (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        hb_buffer_add_utf8($buffer, 'abc', -1, 0, -1);
        hb_buffer_guess_segment_properties($buffer);
        hb_font_destroy($font);
        hb_shape($font, $buffer, null, 0);

        return 'shaped';
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-null-handle');

    expect($kept['threw'])->toBe('ValueError: hb_shape(): Argument #1 ($font) holds NULL: it was destroyed, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});
