<?php

declare(strict_types=1);

// hb_font_set_variations on variableFont(): 'A' is 500 wide at the default (wght 400), 700 at wght 900,
// 400 at wght 100, and in proportion between.

const WGHT = (0x77 << 24) | (0x67 << 16) | (0x68 << 8) | 0x74;

/** The x advance of 'A' in $font. */
function advanceOfA(hb_font_t $font): int
{
    $buffer = hb_buffer_create();
    hb_buffer_add_utf8($buffer, 'A', -1, 0, -1);
    hb_buffer_guess_segment_properties($buffer);
    hb_shape($font, $buffer, null, 0);
    $advance = hb_buffer_get_glyph_positions($buffer, $count)[0]->x_advance;
    hb_buffer_destroy($buffer);

    return $advance;
}

function variation(int $tag, float $value): hb_variation_t
{
    $variation = new hb_variation_t();
    $variation->tag = $tag;
    $variation->value = $value;

    return $variation;
}

it('shapes at the instance the variations name, clamps a value to its axis, and returns to the default with none', function (): void {
    $font = hb_font_create(hb_face_create(hb_blob_create_from_file(variableFont()), 0));
    $at = function (float $weight) use ($font): int {
        hb_font_set_variations($font, [variation(WGHT, $weight)], 1);

        return advanceOfA($font);
    };

    expect(advanceOfA($font))->toBe(500)
        ->and([$at(900.0), $at(650.0), $at(250.0), $at(100.0), $at(2000.0)])->toBe([700, 600, 450, 400, 700]);

    hb_font_set_variations($font, null, 0);

    expect(advanceOfA($font))->toBe(500);
});

it('passes only the first variations_length variations, and ignores an axis the font lacks', function (): void {
    $font = hb_font_create(hb_face_create(hb_blob_create_from_file(variableFont()), 0));
    $wdth = (0x77 << 24) | (0x64 << 16) | (0x74 << 8) | 0x68;

    hb_font_set_variations($font, [variation($wdth, 50.0), variation(WGHT, 900.0)], 1);
    $first = advanceOfA($font);
    hb_font_set_variations($font, [variation($wdth, 50.0), variation(WGHT, 900.0)], 2);

    expect([$first, advanceOfA($font)])->toBe([500, 700]);
});

it('refuses a list element that is not an hb_variation_t, and a variation whose property was unset', function (): void {
    $font = hb_font_create(hb_face_create(hb_blob_create_from_file(variableFont()), 0));
    $unset = variation(WGHT, 900.0);
    unset($unset->value);

    expect(fn () => hb_font_set_variations($font, [variation(WGHT, 900.0), 'wght=900'], 2))
        ->toThrow(TypeError::class, 'must be a list of hb_variation_t, string given at position 1')
        ->and(fn () => hb_font_set_variations($font, [new hb_feature_t()], 1))
        ->toThrow(TypeError::class, 'must be a list of hb_variation_t, hb_feature_t given at position 0')
        ->and(fn () => hb_font_set_variations($font, [$unset], 1))
        ->toThrow(Error::class, 'must not be accessed before initialization');
});
