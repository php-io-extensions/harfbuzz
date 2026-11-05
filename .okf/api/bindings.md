---
type: API
title: Bindings
description: Every bound HarfBuzz function, handle, struct and constant group, by the stub that declares it.
resource: stubs/
tags: [harfbuzz, api]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: stubs
    resource: stubs/
    title: The stubs
---

# Overview

| Stub | C file | Functions | Classes |
|---|---|---|---|
| `harfbuzz.stub.php` | `src/harfbuzz.c` | — | — (constants `HB_DIRECTION_*`, `HB_SCRIPT_*`, `HB_BUFFER_CLUSTER_LEVEL_*`, `HB_FEATURE_GLOBAL_*`) |
| `hb_font.stub.php` | `src/hb_font.c` | `hb_ft_font_create_referenced`, `hb_font_destroy`, `hb_font_set_scale`, `hb_ft_font_changed` | `hb_font_t` |
| `hb_buffer.stub.php` | `src/hb_buffer.c` | `hb_buffer_create`, `hb_buffer_destroy`, `hb_buffer_reset`, `hb_buffer_add_utf8`, `hb_buffer_guess_segment_properties`, `hb_buffer_set_direction`, `hb_buffer_set_script`, `hb_buffer_set_language`, `hb_language_from_string`, `hb_buffer_set_cluster_level`, `hb_buffer_get_length`, `hb_buffer_get_glyph_infos`, `hb_buffer_get_glyph_positions` | `hb_buffer_t`, `hb_language_t`, `hb_glyph_info_t`, `hb_glyph_position_t` |
| `hb_shape.stub.php` | `src/hb_shape.c` | `hb_shape`, `hb_feature_from_string` | `hb_feature_t` |

[^stubs]: The stubs
