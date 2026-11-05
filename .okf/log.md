# Log

## 2026-11-05

* Bundle created with ext-harfbuzz 0.11.0: 19 functions at the HarfBuzz 8.3.0 API, the floor (Ubuntu 24.04's HarfBuzz): the hb-ft font over an ext-freetype `FT_Face`, its scale, buffers and their segment properties, `hb_shape` with features, `hb_feature_from_string`, glyph infos and positions. Built on Homebrew PHP 8.4 NTS and ZTS against HarfBuzz 14.6.0, and on Ubuntu against 8.3.0. [bindings](api/bindings.md), [handles](architecture/handles.md), [structs](architecture/structs.md), [build](runbooks/build.md).
* **Refusals**: HarfBuzz dereferences a NULL buffer or font (FreeType does not), and reads past the lengths and counts it is given. Five sites, each with a probe in `tests/Probes`: `harfbuzz-null-handle` (also an hb-ft font whose `FT_Library` was done), `harfbuzz-ft-face-null`, `harfbuzz-utf8-length` (also an item outside the text, which HarfBuzz 8.3 reads past and 14.6 clamps), `harfbuzz-language-length`, `harfbuzz-num-features`. Pass through, probed: `hb_feature_from_string` with a long `$len` (false), a second `hb_*_destroy`.
* **hb-ft and memory faces**: a font over an `FT_New_Memory_Face` face keeps working after `FT_Done_Face` and after PHP drops the face and its string: ext-freetype's native face owns the bytes. `MemoryFaceTest` covers both.
* **HarfBuzz 8.3 vs 14.6**: 8.3's hb-ft takes advances from the FreeType size and ignores `hb_font_set_scale`; 9 and newer scale them. `hb_ft_font_changed` after `FT_Set_Pixel_Sizes` gives the same advances on both.
* **Handle runtime**: identity is keyed by address and class, so a `fromPointer()` of another class does not clear a live handle. An `hb_feature_t` property after `unset()` throws PHP's uninitialized-property `Error` instead of reading as 0. Invalid UTF-8 becomes U+FFFD in the buffer.
* **Coverage**: `scripts/support.php` counts every `HB_EXTERN … name (` of the headers hb.h, hb-ot.h and hb-aat.h include, and hb-ft.h, at the 8.3.0 tag.
