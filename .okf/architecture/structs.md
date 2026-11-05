---
type: Module
title: Structs
description: Glyph infos and positions copied out, the feature list copied in, and the length refusals.
resource: src/hb_shape.c
tags: [harfbuzz, structs]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: shape
    resource: src/hb_shape.c
    title: hb_shape and phphb_features
---

# Overview

A struct class is `final`, not cloneable, not serializable, with one typed public property per C field under the C name, zero by default, and no constructor (`phphb_struct_setup`). HarfBuzz's private `var`, `var1` and `var2` fields are not read.

`hb_buffer_get_glyph_infos` and `hb_buffer_get_glyph_positions` copy each element into a new object and write HarfBuzz's count to the by-reference `$length`.

`hb_shape` copies its `hb_feature_t` list into a C array (`phphb_features`, a `helpers.marshal` function that throws `TypeError` for an element that is not an `hb_feature_t`). A property that was `unset()` is read without the silent flag, so PHP throws its uninitialized-property `Error` and HarfBuzz is not called. `$num_features` is passed as given, after the refusal of a negative count or a count past the list (site `harfbuzz-num-features`).[^shape]

HarfBuzz reads the lengths it is given: `hb_buffer_add_utf8` refuses a `$text_length` other than -1 past the string, and an item (`$item_offset`, `$item_length`) outside the text, which HarfBuzz 8.3 reads past (`harfbuzz-utf8-length`), `hb_language_from_string` a `$len` past the string (`harfbuzz-language-length`). `hb_feature_from_string` stops at the string's terminating NUL, so its `$len` passes through.

[^shape]: hb_shape and phphb_features
