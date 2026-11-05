# Security Policy

## Supported versions

ext-harfbuzz is pre-1.0. No 0.x release receives security fixes or advisories; fixes land in the
next release line. Security support starts with 1.0.

| Version | Security fixes |
|---------|----------------|
| < 1.0   | No             |

## Reporting a vulnerability

Please don't open a public issue for a security problem.

Report it privately through GitHub: the **Report a vulnerability** button on the
[php-io-extensions/harfbuzz](https://github.com/php-io-extensions/harfbuzz) repository's **Security** tab.
If that isn't available, email **info@projectsaturnstudios.com**.

Include what you found, the affected version, your platform, PHP build (NTS or ZTS), the HarfBuzz
and FreeType versions (`php --ri harfbuzz` and `php --ri freetype`), the font file and text that
trigger it if you can share them, and the smallest PHP script that shows it. Reports are read and
weighed for the release line in development; before 1.0 there is no response-time commitment.

A defect in HarfBuzz itself belongs with the HarfBuzz project. Report it here as well when
ext-harfbuzz should guard against it.

## Security model

ext-harfbuzz binds HarfBuzz one to one: it passes what PHP gives it to HarfBuzz and does not
validate fonts or text, so it is exactly as robust as the HarfBuzz and FreeType it is linked
against. Keep them patched; the installers link whatever pkg-config finds.

- **Refusals.** HarfBuzz reads the objects, lengths and counts it is given without checking them.
  The extension refuses a call only where HarfBuzz would read memory it should not: a font or buffer
  that holds NULL, an hb-ft font whose `FT_Library` was done, an `FT_Face` that holds NULL, a text
  length, item or language length past the string, and a feature count past the list. Each refusal is
  a `ValueError`, and each has a probe test that shows the crash without it.
- **Pass-through.** Calls HarfBuzz makes safe on NULL (a second `hb_*_destroy()`, an invalid language)
  reach it unchanged.
- **Faces.** A font holds its own reference to the FreeType face, so `FT_Done_Face()` alone does not
  free what the font reads.
- **Addresses.** `fromPointer()` trusts any address it is given. Never pass it an integer from
  untrusted input.

A report is in scope when ext-harfbuzz itself reads or writes memory it should not, leaks, or
crashes on a call that its README does not document as a pass-through.
