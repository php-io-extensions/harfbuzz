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

/** $ft_face is an ext-freetype FT_Face. HarfBuzz references the native face; FT_Done_Face drops one reference. */
function hb_ft_font_create_referenced(FT_Face $ft_face): hb_font_t {}

function hb_font_destroy(hb_font_t $font): void {}

function hb_font_set_scale(hb_font_t $font, int $x_scale, int $y_scale): void {}

function hb_ft_font_changed(hb_font_t $font): void {}
