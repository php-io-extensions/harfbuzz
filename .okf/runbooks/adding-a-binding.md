---
type: Runbook
title: Adding a binding
description: Declare it in the stub for its group, regenerate arginfo, implement it beside that group, test it, and refresh coverage.
resource: stubs/
tags: [harfbuzz, contributing]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: build
    resource: runbooks/build.md
    title: Build runbook
---

# Overview

1. Check the function exists in HarfBuzz 8.3.0 (the floor). Add it, its class or its `@cvalue` constant to the stub of its group: `stubs/hb_font.stub.php`, `hb_buffer.stub.php`, `hb_shape.stub.php`, or `harfbuzz.stub.php` for constants. A new group is a new stub and a new `src/<Group>.c`, added to `config.m4` and registered from `PHP_MINIT_FUNCTION` in `src/harfbuzz.c`. ext-guard checks the stub against the HarfBuzz headers.
2. `php84 /opt/homebrew/opt/php@8.4/lib/php/build/gen_stub.php stubs`. Commit the stub and the arginfo.
3. Implement it in the C file that includes that arginfo, one written-out `ZEND_FUNCTION` per stub function. A buffer or font argument HarfBuzz dereferences goes through `phphb_live_ptr`. A new handle class gets `phphb_handle_setup` and `PHPHB_HANDLE_METHODS`, and `pointer`/`fromPointer` under `glue` in `bindings.json` (the maintainer adds those). A new struct gets `phphb_struct_setup`.
4. Write the test first, watch it fail, build per [build](/runbooks/build.md), watch it pass.
5. `php84 scripts/support.php` refreshes SUPPORT.md and the README badge.

[^build]: Build runbook
