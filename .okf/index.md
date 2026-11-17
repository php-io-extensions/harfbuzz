---
okf_version: "0.2"
---

# ext-harfbuzz

1:1 PHP bindings of HarfBuzz at the 8.3.0 API: fonts from HarfBuzz's own blob, face and font, and hb-ft over ext-freetype faces when ext-freetype is loaded. Version 0.11.0. Linux and macOS.

# API

* [Bindings](api/bindings.md) — Functions, handles, structs and constants by stub.

# Architecture

* [Handles](architecture/handles.md) — Identity table, destroy, the NULL refusal, blobs and faces, and the optional ext-freetype face.
* [Structs](architecture/structs.md) — Glyph infos and positions out, the feature list in.

# Runbooks

* [Build, install, test](runbooks/build.md) — Release and probe builds, and the runs that gate a commit.
* [Adding a binding](runbooks/adding-a-binding.md) — Stub, gen_stub, one C file per stub, `config.m4`, coverage.
