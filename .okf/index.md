---
okf_version: "0.2"
---

# ext-harfbuzz

1:1 PHP bindings of HarfBuzz at the 8.3.0 API, with hb-ft over ext-freetype faces. Version 0.11.0. Linux and macOS.

# API

* [Bindings](api/bindings.md) — Functions, handles, structs and constants by stub.

# Architecture

* [Handles](architecture/handles.md) — Identity table, destroy, the NULL refusal, and the ext-freetype face.
* [Structs](architecture/structs.md) — Glyph infos and positions out, the feature list in.

# Runbooks

* [Build, install, test](runbooks/build.md) — After ext-freetype, release and probe builds.
* [Adding a binding](runbooks/adding-a-binding.md) — Stub, gen_stub, one C file per stub, `config.m4`, coverage.
