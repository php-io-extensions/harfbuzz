<?php

declare(strict_types=1);

// Site harfbuzz-null-handle (outcome 1), second condition: FT_Done_FreeType destroys every face of the
// library, whatever its reference count, so an hb-ft font over one of them reads a freed face.

beforeEach(fn () => requireProbeBuild());

it('refuses a font whose FT_Library was done, which reads the freed face (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        FT_Done_FreeType($library);
        hb_buffer_add_utf8($buffer, 'abc', -1, 0, -1);
        hb_buffer_guess_segment_properties($buffer);
        hb_shape($font, $buffer, null, 0);

        return hb_buffer_get_length($buffer);
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-null-handle');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: hb_shape(): Argument #1 ($font) reads a face that FT_Done_FreeType destroyed')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});

it('passes a second hb_buffer_destroy, which HarfBuzz takes as a no-op on NULL', function (): void {
    expect(hbChild("hb_buffer_destroy(\$buffer);\nhb_buffer_destroy(\$buffer);\nreturn 'twice';")['result'])->toBe('twice');
});
