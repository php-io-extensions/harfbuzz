<?php

declare(strict_types=1);

// Site harfbuzz-ft-face-null (outcome 1): hb_ft_font_create_referenced() reads the FT_Face it is given.
// An ext-freetype FT_Face whose pointer() is 0 (released by FT_Done_Face, or made by fromPointer(0))
// gives it NULL.

beforeEach(fn () => requireProbeBuild());

it('refuses a released FT_Face, which hb-ft dereferences (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        FT_Done_Face($face);
        hb_ft_font_create_referenced($face);

        return 'created';
    PHP;

    $kept = hbChild($code);
    $off = hbChild($code, 'harfbuzz-ft-face-null');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: hb_ft_font_create_referenced(): Argument #1 ($ft_face) holds NULL: it was released, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});
