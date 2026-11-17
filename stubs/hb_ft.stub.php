<?php

/**
 * hb-ft: registered only when ext-freetype is loaded, since its fonts read an ext-freetype FT_Face.
 *
 * @generate-function-entries
 */

/** $ft_face is an ext-freetype FT_Face. HarfBuzz references the native face; FT_Done_Face drops one reference. */
function hb_ft_font_create_referenced(FT_Face $ft_face): hb_font_t {}

function hb_ft_font_changed(hb_font_t $font): void {}
