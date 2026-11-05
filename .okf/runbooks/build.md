---
type: Runbook
title: Build, install, test
description: Release and probe builds on macOS and Linux after ext-freetype, and the test runs that gate a commit.
resource: install-macos.sh
tags: [harfbuzz, build]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: mac
    resource: install-macos.sh
    title: macOS installer
---

# Overview

Install ext-freetype first: both installers stop when it is not loaded. The ini is `40-harfbuzz.ini`, after ext-freetype's `30-freetype.ini`.

macOS: `bash install-macos.sh` builds in a temp copy for Homebrew `php@8.4` and `php@8.4-zts`, installs, ad-hoc signs and enables the ini. It fails on any warning from `src/`.[^mac]

Linux (Debian trixie, Ubuntu 24.04, whose HarfBuzz 8.3.0 is the floor): `bash install-debian-trixie.sh` in the checkout. It needs sudo to install the `.so` and the ini files.

Probe build: `HARFBUZZ_CONFIGURE=--enable-harfbuzz-probes bash install-macos.sh /opt/homebrew/opt/php@8.4/bin/php`. Run `tests/Probes` (macOS runs the children under libgmalloc), then reinstall the release build. Never ship a probe build.

Tests: `composer install`, then `XDEBUG_MODE=off php84 -d memory_limit=128M vendor/bin/pest` and the same with `zhp`. Gate a commit on the exit code.

[^mac]: macOS installer
