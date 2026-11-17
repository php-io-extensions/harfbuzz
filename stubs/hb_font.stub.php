<?php

/** @generate-class-entries */

/**
 * An hb_font_t. One PHP object per native pointer; dropping it does not call hb_font_destroy().
 * fromPointer() trusts any address other than 0.
 */
final class hb_font_t
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

final class hb_variation_t
{
    public int $tag = 0;
    public float $value = 0.0;
}

/** The font references the face and reads it with HarfBuzz's OpenType functions: hb_face_destroy() drops one reference. */
function hb_font_create(hb_face_t $face): hb_font_t {}

function hb_font_destroy(hb_font_t $font): void {}

function hb_font_set_scale(hb_font_t $font, int $x_scale, int $y_scale): void {}

/** $variations is a list of hb_variation_t, or null; $variations_length is passed as given. */
function hb_font_set_variations(hb_font_t $font, ?array $variations, int $variations_length): void {}
