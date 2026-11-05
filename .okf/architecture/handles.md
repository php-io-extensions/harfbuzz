---
type: Module
title: Handles
description: One PHP object per native pointer and class, released by hb_*_destroy, refused when it holds NULL or its face is gone, and the FT_Face read from ext-freetype.
resource: src/runtime.c
tags: [harfbuzz, zend, lifetime]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: runtime
    resource: src/runtime.h
    title: phphb_handle and the identity table
---

# Overview

A handle is `phphb_handle { void *ptr; zval library; zend_object std; }`. `ptr` is the native object, or NULL once destroyed or when made by `fromPointer(0)`. `library` is set on an hb-ft font only: the ext-freetype `FT_Library` its face belongs to. `free_obj` drops the identity entry and `library`; it never calls `hb_*_destroy`.[^runtime]

`phphb_box` returns the PHP object already stored for that address and class, or a new one. The identity table is module global `boxes`, keyed by (address, class), one per ZTS thread, so a `fromPointer()` of another class at the same address does not disturb a live handle. `hb_font_destroy` and `hb_buffer_destroy` call `phphb_release` after the native call. A second destroy passes NULL, which HarfBuzz takes.[^runtime]

HarfBuzz dereferences the buffer and font it is given, so `phphb_live_ptr` (a `helpers.marshal` function) refuses a buffer or font holding NULL with a `ValueError` (site `harfbuzz-null-handle`). The same check refuses an hb-ft font whose `FT_Library` reports `pointer()` 0: `FT_Done_FreeType` destroys every face of the library whatever its reference count, and the font would read the destroyed face. An `hb_language_t` holding NULL is `HB_LANGUAGE_INVALID`, which HarfBuzz takes, so languages are read with the unchecked `phphb_handle_ptr`.[^runtime]

`hb_ft_font_create_referenced` takes an ext-freetype `FT_Face`: MINIT finds that class in the class table (`ZEND_MOD_REQUIRED("freetype")` registers it first), and the call reads the native face through `FT_Face::pointer()`. A face holding NULL is refused (site `harfbuzz-ft-face-null`). The font keeps `FT_Library::fromPointer(face->glyph->library)` in `library`. HarfBuzz takes its own reference on the native face, so `FT_Done_Face` drops one reference and the font stays usable.

[^runtime]: phphb_handle and the identity table
