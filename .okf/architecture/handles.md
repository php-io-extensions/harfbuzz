---
type: Module
title: Handles
description: One PHP object per native pointer and class, released by hb_*_destroy, refused when it holds NULL or its face is gone, blobs and faces passed through as NULL, and the FT_Face read from the optional ext-freetype.
resource: src/runtime.c
tags: [harfbuzz, zend, lifetime]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-16T22:00:00-05:00 }
sources:
  - id: runtime
    resource: src/runtime.h
    title: phphb_handle and the identity table
---

# Overview

A handle is `phphb_handle { void *ptr; zval library; zend_object std; }`. `ptr` is the native object, or NULL once destroyed or when made by `fromPointer(0)`. `library` is set on an hb-ft font only: the ext-freetype `FT_Library` its face belongs to. `free_obj` drops the identity entry and `library`; it never calls `hb_*_destroy`.[^runtime]

`phphb_box` returns the PHP object already stored for that address and class, or a new one. The identity table is module global `boxes`, keyed by (address, class), one per ZTS thread, so a `fromPointer()` of another class at the same address does not disturb a live handle. `hb_blob_destroy`, `hb_face_destroy`, `hb_font_destroy` and `hb_buffer_destroy` call `phphb_release` after the native call. A second destroy passes NULL, which HarfBuzz takes.[^runtime]

`hb_face_create` and `hb_font_create` read their blob and face with the unchecked `phphb_handle_ptr`: HarfBuzz 8.3.0 reads a NULL blob as `hb_blob_get_empty()` and a NULL face as `hb_face_get_empty()`, so a destroyed handle passes through as it would from `fromPointer(0)`. `hb_blob_create_from_file` never answers NULL: a file that does not open is the empty blob. A face references its blob and a font its face, so destroying the blob and face after `hb_font_create` leaves the font working.

HarfBuzz dereferences the buffer and font it is given, so `phphb_live_ptr` (a `helpers.marshal` function) refuses a buffer or font holding NULL with a `ValueError` (site `harfbuzz-null-handle`). The same check refuses an hb-ft font whose `FT_Library` reports `pointer()` 0: `FT_Done_FreeType` destroys every face of the library whatever its reference count, and the font would read the destroyed face. An `hb_language_t` holding NULL is `HB_LANGUAGE_INVALID`, which HarfBuzz takes, so languages are read with the unchecked `phphb_handle_ptr`.[^runtime]

`hb_ft_font_create_referenced` takes an ext-freetype `FT_Face`. ext-freetype is optional (`ZEND_MOD_OPTIONAL("freetype")`): when it is loaded it starts first, and `phphb_register_hb_ft` finds `FT_Face` and `FT_Library` in the class table and registers the `hb_ft` stub's functions; without it they are not registered. The call reads the native face through `FT_Face::pointer()`. A face holding NULL is refused (site `harfbuzz-ft-face-null`). The font keeps `FT_Library::fromPointer(face->glyph->library)` in `library`. HarfBuzz takes its own reference on the native face, so `FT_Done_Face` drops one reference and the font stays usable.

[^runtime]: phphb_handle and the identity table
