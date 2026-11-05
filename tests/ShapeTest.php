<?php

declare(strict_types=1);

it('shapes three monospace letters into three equal advances, clusters 0 1 2', function (): void {
    [$font, $buffer] = monoShaper('abc');
    hb_shape($font, $buffer, null, 0);

    $infos = hb_buffer_get_glyph_infos($buffer, $infoCount);
    $positions = hb_buffer_get_glyph_positions($buffer, $positionCount);

    expect($infoCount)->toBe(3)
        ->and($positionCount)->toBe(3)
        ->and($infos[0])->toBeInstanceOf(hb_glyph_info_t::class)
        ->and($positions[0])->toBeInstanceOf(hb_glyph_position_t::class)
        ->and(array_map(fn (hb_glyph_info_t $i): int => $i->cluster, $infos))->toBe([0, 1, 2])
        ->and(array_unique(array_map(fn (hb_glyph_position_t $p): int => $p->x_advance, $positions)))->toHaveCount(1)
        ->and($positions[0]->x_advance)->toBeGreaterThan(0);
});

it('keeps a combining mark in its base character cluster', function (): void {
    [$font, $buffer] = monoShaper("e\u{0301}");
    hb_shape($font, $buffer, null, 0);

    $clusters = array_map(fn (hb_glyph_info_t $i): int => $i->cluster, hb_buffer_get_glyph_infos($buffer, $count));
    expect(array_values(array_unique($clusters)))->toBe([0]);
});

it('shapes invalid UTF-8 as U+FFFD with its own cluster', function (): void {
    [$font, $buffer] = monoShaper("a\xFFb");

    expect(array_map(fn (hb_glyph_info_t $i): int => $i->codepoint, hb_buffer_get_glyph_infos($buffer, $count)))->toBe([0x61, 0xFFFD, 0x62]);
    hb_shape($font, $buffer, null, 0);

    expect(array_map(fn (hb_glyph_info_t $i): int => $i->cluster, hb_buffer_get_glyph_infos($buffer, $count)))->toBe([0, 1, 2]);
});

it('parses a feature string and passes features to hb_shape', function (): void {
    expect(hb_feature_from_string('-kern', -1, $feature))->toBeTrue()
        ->and($feature)->toBeInstanceOf(hb_feature_t::class)
        ->and($feature->value)->toBe(0)
        ->and($feature->start)->toBe(HB_FEATURE_GLOBAL_START)
        ->and($feature->end)->toBe(HB_FEATURE_GLOBAL_END);

    [$font, $buffer] = monoShaper('AV');
    hb_shape($font, $buffer, [$feature], 1);

    expect(hb_buffer_get_length($buffer))->toBe(2);
});

it('answers an empty list for an empty buffer', function (): void {
    $buffer = hb_buffer_create();

    expect(hb_buffer_get_glyph_infos($buffer, $count))->toBe([])
        ->and($count)->toBe(0);
});

it('sets direction, script, language and cluster level as HarfBuzz reads them', function (): void {
    [$font, $buffer] = monoShaper('ab');
    hb_buffer_reset($buffer);
    hb_buffer_add_utf8($buffer, 'ab', -1, 0, -1);
    hb_buffer_set_direction($buffer, HB_DIRECTION_RTL);
    hb_buffer_set_script($buffer, HB_SCRIPT_LATIN);
    hb_buffer_set_language($buffer, hb_language_from_string('en', -1));
    hb_buffer_set_cluster_level($buffer, HB_BUFFER_CLUSTER_LEVEL_CHARACTERS);
    hb_shape($font, $buffer, null, 0);

    // Right to left: the glyph for 'b' (cluster 1) comes first.
    expect(array_map(fn (hb_glyph_info_t $i): int => $i->cluster, hb_buffer_get_glyph_infos($buffer, $count)))->toBe([1, 0]);
});

it('follows the FreeType size after hb_ft_font_changed, whatever scale was set before', function (): void {
    // HarfBuzz 8.3's hb-ft takes advances from the FreeType size and ignores hb_font_set_scale; 9 and newer
    // scale them. hb_ft_font_changed sets the scale from the FreeType size on both.
    [$font, $buffer, $face] = monoShaper('a');
    hb_shape($font, $buffer, null, 0);
    $before = hb_buffer_get_glyph_positions($buffer, $count)[0]->x_advance;

    hb_font_set_scale($font, 5 * 32 * 64, 5 * 32 * 64);
    FT_Set_Pixel_Sizes($face, 0, 64);
    hb_ft_font_changed($font);
    hb_buffer_reset($buffer);
    hb_buffer_add_utf8($buffer, 'a', -1, 0, -1);
    hb_buffer_guess_segment_properties($buffer);
    hb_shape($font, $buffer, null, 0);

    expect(hb_buffer_get_glyph_positions($buffer, $count)[0]->x_advance)->toBe(2 * $before);
});
