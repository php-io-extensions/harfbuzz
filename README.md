<div align="center" id="header">

<h1>ext-harfbuzz</h1>

<h3>HarfBuzz bound 1:1 into PHP 8.4+, at the 8.3.0 API, over ext-freetype faces</h3>


[![CI](https://img.shields.io/github/actions/workflow/status/php-io-extensions/harfbuzz/ci.yml?style=flat-square&label=CI)](https://github.com/php-io-extensions/harfbuzz/actions/workflows/ci.yml)
[![HarfBuzz API](https://img.shields.io/badge/HarfBuzz_API-3.8%25_%2819%2F497%29-blue?style=flat-square)](SUPPORT.md)
[![Packagist](https://img.shields.io/packagist/v/php-io-extensions/harfbuzz?style=flat-square)](https://packagist.org/packages/php-io-extensions/harfbuzz)
[![PHP](https://img.shields.io/packagist/dependency-v/php-io-extensions/harfbuzz/php?style=flat-square&label=PHP)](https://packagist.org/packages/php-io-extensions/harfbuzz)
[![License](https://img.shields.io/github/license/php-io-extensions/harfbuzz?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-linux_%7C_macos-blue?style=flat-square&logo=linux&logoColor=white)](#requirements)
[![OKF](https://img.shields.io/badge/OKF-0.2-6f42c1?style=flat-square&label=knowledge%20bundle)](.okf/index.md)
[![PIE](https://img.shields.io/badge/PIE-installable-4f5b93?style=flat-square&logo=php&logoColor=white)](https://github.com/php/pie)

</div>

Drawing text is more than drawing one glyph per character. "fi" can be one ligature glyph, "e" followed by a combining accent can be one glyph or a glyph and a mark placed over it, "AV" sits closer than "AH", Arabic letters change shape with their neighbours, and Hebrew runs right to left. Before a renderer draws anything, something has to turn a string into the right glyphs, in the right order, at the right positions, and remember which characters each glyph came from so a cursor can land between them.

<b>HarfBuzz</b> is that something. It is the shaping engine behind Chrome, Firefox, Android, GNOME and LibreOffice: give it a font and a buffer of text, call `hb_shape()`, and read back glyph ids, clusters, advances and offsets, with the font's OpenType features applied. It is written in C, and its documentation covers every call.

<b>The only problem?</b> PHP can't reach it. This extension binds HarfBuzz one to one under the C names, at the HarfBuzz 8.3.0 API: fonts over [ext-freetype](https://github.com/php-io-extensions/freetype) faces (hb-ft), buffers and their segment properties, shaping with features, and the shaped glyph infos and positions. Structs are typed classes, lists are PHP arrays with their counts kept as separate arguments, and the lengths HarfBuzz would read past are refused before it reads them. No defaults, no composites, no opinions: what HarfBuzz documents is what you call.

## Basic usage

Most ways you will interface with ext-harfbuzz are in two parts -

Make a font and fill a buffer. The font is an hb-ft font over an ext-freetype `FT_Face` at the size you set in FreeType; the buffer holds UTF-8 text, and `hb_buffer_guess_segment_properties()` picks its direction, script and language:

```php
FT_Init_FreeType($library);
FT_New_Face($library, '/System/Library/Fonts/Menlo.ttc', 0, $face);   // DejaVuSansMono.ttf on Linux
FT_Set_Pixel_Sizes($face, 0, 32);
$font = hb_ft_font_create_referenced($face);

$buffer = hb_buffer_create();
hb_buffer_add_utf8($buffer, "Café e\u{0301}", -1, 0, -1);
hb_buffer_guess_segment_properties($buffer);
```

Shape it and walk the glyphs. Each glyph has its glyph id in the font (`codepoint` after shaping), its cluster (the byte offset of the text it came from) and its advance and offset in 26.6 fixed point. Destroy what you made when you're done:

```php
hb_shape($font, $buffer, null, 0);

$infos = hb_buffer_get_glyph_infos($buffer, $count);
$positions = hb_buffer_get_glyph_positions($buffer, $count);
$x = 0;
for ($i = 0; $i < $count; $i++) {
    printf("glyph %4d  cluster %d  x %6.2f  offset %5.2f\n", $infos[$i]->codepoint, $infos[$i]->cluster, ($x + $positions[$i]->x_offset) / 64, $positions[$i]->x_offset / 64);
    $x += $positions[$i]->x_advance;
}
printf("width %.2f px\n", $x / 64);

hb_buffer_destroy($buffer);
hb_font_destroy($font);
FT_Done_Face($face);
FT_Done_FreeType($library);
```

```console
$ php shape.php
glyph   38  cluster 0  x   0.00  offset  0.00
glyph   68  cluster 1  x  19.27  offset  0.00
glyph   73  cluster 2  x  38.53  offset  0.00
glyph  171  cluster 3  x  57.80  offset  0.00
glyph    3  cluster 5  x  77.06  offset  0.00
glyph  171  cluster 6  x  96.33  offset  0.00
width 115.59 px
```

`é` is two bytes, so the space is cluster 5. The decomposed `e` and U+0301 at bytes 6 to 8 became the same precomposed glyph 171 as the `é` before it: one glyph, one cluster. Load each glyph id with `FT_Load_Glyph()` and draw it at its `x`.

## Requirements

| | |
|---|---|
| OS | Linux or macOS. `composer.json` lists those two families. |
| PHP | 8.4 or newer, NTS or ZTS. |
| Extensions | [ext-freetype](https://github.com/php-io-extensions/freetype), loaded first: `hb_ft_font_create_referenced()` takes its `FT_Face`. The installers write `40-harfbuzz.ini`, after ext-freetype's `30-freetype.ini`. |
| Libraries | HarfBuzz 8.3.0 or newer, built with FreeType (hb-ft), with its headers, found through `pkg-config` (`harfbuzz`, `freetype2`). 8.3.0 is the floor: nothing newer is bound. |

To build it you need a C compiler, `pkg-config`, HarfBuzz's and FreeType's development files, and the development files of the PHP you intend to load it into:

```bash
# Debian, Ubuntu, Raspberry Pi OS
sudo apt install build-essential pkg-config libharfbuzz-dev libfreetype-dev php8.4-dev

# macOS: Xcode Command Line Tools, and Homebrew's PHP ships phpize and php-config
xcode-select --install
brew install pkgconf harfbuzz freetype php@8.4
```

## Installation

### With PHP PIE (recommended)

[PIE](https://github.com/php/pie) is the PHP Foundation's installer for extensions. It downloads the tagged source from Packagist, builds it against the PHP on your `PATH`, installs the module and enables it. `sudo` where your PHP's extension directory is root-owned, as on Debian; not on a Homebrew Mac. Install the libraries and ext-freetype first:

```bash
pie install php-io-extensions/freetype
pie install php-io-extensions/harfbuzz
```

### Manually (install script)

The repository ships two installers, one per platform. Each one stops when ext-freetype is not loaded, builds for each PHP it targets, installs `harfbuzz.so` into that PHP's `extension_dir`, writes `40-harfbuzz.ini` into its `conf.d` scan directory, and checks the module loads. The macOS one builds in a temporary copy of the checkout and ad-hoc signs the module so the system loads it on first use; the Debian one builds in the checkout, writes the ini into the CLI, FPM and Apache `conf.d` of that PHP, and cleans the build products out afterwards.

```bash
git clone https://github.com/php-io-extensions/harfbuzz.git
cd harfbuzz

./install-debian-trixie.sh             # Debian Trixie, Ubuntu 24.04+, Raspberry Pi OS: the php on PATH
./install-macos.sh                     # Homebrew php@8.4 and php@8.4-zts
./install-macos.sh /path/to/bin/php    # the macOS script takes specific PHP binaries
```

### Manually (phpize)

```bash
git clone https://github.com/php-io-extensions/harfbuzz.git
cd harfbuzz
phpize
./configure --enable-harfbuzz
make -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu)"
sudo make install
```

Then enable it after ext-freetype: add `extension=harfbuzz.so` to your `php.ini` below `extension=freetype.so`, or drop that line into a file in the scan directory that `php --ini` reports, named to sort after freetype's.

### Verify

`php --ri harfbuzz` also names the HarfBuzz headers it was built against and the library it runs with:

```console
$ php --ri harfbuzz

harfbuzz

harfbuzz support => enabled
Version => 0.11.0
HarfBuzz headers => 14.6.0
HarfBuzz library => 14.6.0
```

## API

Function, handle, struct, field and constant names are the C names, so [HarfBuzz's API reference](https://harfbuzz.github.io/reference-manual.html) is the reference: 19 functions, one stub per group in [`stubs/`](stubs), each declared with its PHP types. [SUPPORT.md](SUPPORT.md) lists every function HarfBuzz 8.3.0 declares in its public headers, bound or not; the API badge is computed from it.

| Stub | Functions | What it covers |
|---|---|---|
| `hb_font` | 4 | `hb_ft_font_create_referenced()`, `hb_font_destroy()`, `hb_font_set_scale()`, `hb_ft_font_changed()`; the `hb_font_t` handle. |
| `hb_buffer` | 13 | Create, destroy, reset, `hb_buffer_add_utf8()`, the segment properties (direction, script, language, cluster level, guessed), `hb_language_from_string()`, the length, glyph infos and positions; `hb_buffer_t`, `hb_language_t`, `hb_glyph_info_t`, `hb_glyph_position_t`. |
| `hb_shape` | 2 | `hb_shape()` and `hb_feature_from_string()`; `hb_feature_t`. |
| `harfbuzz` | — | The `HB_DIRECTION_*`, `HB_SCRIPT_*`, `HB_BUFFER_CLUSTER_LEVEL_*` constants, `HB_FEATURE_GLOBAL_START` and `HB_FEATURE_GLOBAL_END`. |

### Translations

Every C parameter is its own PHP parameter, in C order. The only differences from C:

- `const char *` is a `string`; its length stays its own parameter: `hb_buffer_add_utf8($buffer, $text, -1, 0, -1)`.
- An out-parameter is a by-reference parameter: `hb_feature_from_string('-kern', -1, $feature)` writes an `hb_feature_t` to `$feature`, and `hb_buffer_get_glyph_infos($buffer, $length)` writes the count to `$length`.
- `const hb_feature_t *features` is a list of `hb_feature_t`, or `null`; `$num_features` stays its own parameter. An array HarfBuzz returns with an out count is a list of fresh copies.
- HarfBuzz's integer types and enums (`hb_codepoint_t`, `hb_position_t`, `hb_direction_t`, `hb_script_t`, …) are `int`. `hb_bool_t` is `bool`.
- A struct is a `final` class with one typed public property per field: `hb_glyph_info_t` (`codepoint`, `mask`, `cluster`), `hb_glyph_position_t` (`x_advance`, `y_advance`, `x_offset`, `y_offset`), `hb_feature_t` (`tag`, `value`, `start`, `end`). HarfBuzz's private `var` fields are not read. An `hb_feature_t` property that was `unset()` is not read as 0: `hb_shape()` throws PHP's `Error` for an uninitialized typed property.
- `hb_font_t`, `hb_buffer_t` and `hb_language_t` are `final` handles with a private constructor, not cloneable or serializable, with `pointer(): int` and `static fromPointer(int $pointer): static`. `fromPointer()` trusts any address. The same native pointer and class in one thread is the same PHP object.

### Lifetime

Dropping the last PHP reference does not destroy the native object. `hb_font_destroy()` and `hb_buffer_destroy()` release their handle; a second destroy passes NULL, which HarfBuzz takes. HarfBuzz interns languages, so an `hb_language_t` is never released; `hb_language_from_string()` answers a handle holding NULL (`HB_LANGUAGE_INVALID`) for an empty tag. An hb-ft font holds its own reference to the FreeType face, so `FT_Done_Face()` alone leaves the font working.

HarfBuzz 8.3's hb-ft takes advances from the FreeType size and ignores `hb_font_set_scale()`; 9 and newer scale them. After `FT_Set_Pixel_Sizes()`, call `hb_ft_font_changed()`: both give the same advances then.

### Refusals

HarfBuzz reads the objects and lengths it is given without checking them. The extension refuses these calls, each of which crashes the process otherwise ([`tests/Probes`](tests/Probes) shows each crash on a probe build):

- An `hb_font_t` or `hb_buffer_t` that holds NULL (destroyed, or made from address 0): `ValueError`.
- An hb-ft font whose face's `FT_Library` was done: `ValueError`. `FT_Done_FreeType()` destroys every face of the library whatever its reference count.
- An `FT_Face` that holds NULL in `hb_ft_font_create_referenced()`: `ValueError`.
- A `$text_length` other than -1 larger than the string, or an item (`$item_offset`, `$item_length`) outside the text, in `hb_buffer_add_utf8()`: `ValueError`.
- A `$len` larger than the string in `hb_language_from_string()`: `ValueError`.
- A `$num_features` below 0 or larger than the list (0 for `null`) in `hb_shape()`: `ValueError`. A list element that is not an `hb_feature_t` is a `TypeError`.

## Extensions you might use with this one

* [ext-freetype](https://github.com/php-io-extensions/freetype): required. It opens and sizes the face HarfBuzz shapes with, and renders the glyph ids HarfBuzz returns.
* [ext-fontconfig](https://github.com/php-io-extensions/fontconfig): finds the font for a family, or a fallback font for the characters the first one lacks, so each run of text shapes with a font that has its glyphs.

## AI-assisted development

This codebase was built with LLM assistance under human direction. Prompts, reviews, and architectural decisions were owned by a professional software engineer — the model accelerates drafting and exploration; it does not replace judgment on APIs, dependency direction, or what ships.

## Agentic Documentation

This repository carries an [Open Knowledge Format](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md) bundle at [`.okf/`](.okf/): Markdown with YAML frontmatter, readable by you and loadable by a coding agent. It records what the README doesn't: every function, class and constant by stub, how handles keep one PHP object per native pointer, how a font reaches its ext-freetype face and library, how glyph infos, positions and feature lists are copied, each refusal and the length it guards, how to build release and probe builds, how to add a binding, and a log of every change that altered behaviour.

Start at [`.okf/index.md`](.okf/index.md). [`AGENTS.md`](AGENTS.md) is the short rule sheet an agent reads before touching the C, the stubs, or packaging; each rule points into the bundle for the reason behind it.

Point your agent at the repo and it has the same context the maintainer has. Point yourself at it when the README's answer is "it does X" and you want "because".

## Resources

* [HarfBuzz API reference](https://harfbuzz.github.io/reference-manual.html): the manual for every call this extension binds; this README only adds what PHP changes.
* [HarfBuzz 8.3.0](https://github.com/harfbuzz/harfbuzz/tree/8.3.0): the upstream tag the stubs and [SUPPORT.md](SUPPORT.md) are checked against.
* [PIE](https://github.com/php/pie): the PHP Foundation's extension installer.
* [`gen_stub.php`](https://github.com/php/php-src/blob/master/build/gen_stub.php): the php-src stub format. [`stubs/`](stubs) holds the API declarations; each `*_arginfo.h` is generated from its stub.
* [Open Knowledge Format](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md): the spec the [`.okf/`](.okf/) bundle follows.

## Running the test suite

The suite is Pest v4: shaping Latin text, a combining mark, invalid UTF-8, right-to-left text and a kerning pair with `-kern`, features, scale changes, memory faces, handle identity, and the coverage script. It runs on Linux and macOS, with Menlo on macOS and DejaVu Sans Mono on Linux (`fonts-dejavu-core`), and needs ext-freetype. Build both modules, then enable them through an ini scan directory rather than `-d`, so the child processes the tests start load them too:

```bash
git clone https://github.com/php-io-extensions/freetype.git
(cd freetype && phpize && ./configure --enable-freetype && make -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu)")
git clone https://github.com/php-io-extensions/harfbuzz.git
cd harfbuzz
phpize && ./configure --enable-harfbuzz && make -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu)"
composer install
mkdir -p /tmp/harfbuzz-ini
echo "extension=$PWD/../freetype/modules/freetype.so" > /tmp/harfbuzz-ini/30-freetype.ini
echo "extension=$PWD/modules/harfbuzz.so" > /tmp/harfbuzz-ini/40-harfbuzz.ini
PHP_INI_SCAN_DIR=":/tmp/harfbuzz-ini" php -d memory_limit=128M vendor/bin/pest
```

```console
Tests:    11 skipped, 17 passed (38 assertions)
Duration: 2.57s
```

The eleven skipped tests are in [`tests/Probes`](tests/Probes): they run only against a probe build (`./configure --enable-harfbuzz --enable-harfbuzz-probes`, or `HARFBUZZ_CONFIGURE=--enable-harfbuzz-probes ./install-macos.sh`), which can turn each refusal off to show the crash it prevents. On macOS the probes run their child processes under libgmalloc, so a short read faults every time; two of them need its guard pages and skip on Linux. Never install a probe build for use.

The leading `:` keeps your PHP's own scan directory and adds the second one. If that PHP already loads an installed `freetype.so` or `harfbuzz.so`, the suite warns `Module "harfbuzz" is already loaded` and tests the installed one instead; copy the scan directory somewhere, replace its freetype and harfbuzz inis with the lines above, and point `PHP_INI_SCAN_DIR` at the copy without the colon. A module only loads in the PHP whose `phpize` built it: for Homebrew's ZTS build, run `phpize` and pass `--with-php-config` from `/opt/homebrew/opt/php@8.4-zts/bin`. Set `XDEBUG_MODE=off` for any timing you intend to read.

## Contributing

Open an issue first for anything beyond a typo. Pull requests change a stub in `stubs/` and the C under `src/` together, regenerate the `*_arginfo.h` files with gen_stub, regenerate [SUPPORT.md](SUPPORT.md) and the API badge with `php scripts/support.php`, and add a Pest test for the behaviour. A new refusal needs a probe test that shows the crash it prevents. CI must be green on every cell of the matrix, NTS and ZTS alike. Read [`AGENTS.md`](AGENTS.md) for the rules the code follows.

## Security

ext-harfbuzz hands fonts and text, either of which can come from the network, to HarfBuzz. See [SECURITY.md](SECURITY.md) for what it guards against and how to report a vulnerability.

## License

MIT. See [LICENSE](LICENSE). HarfBuzz is under the "Old MIT" license; it is linked, not shipped.
