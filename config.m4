PHP_ARG_ENABLE([harfbuzz],
  [whether to enable harfbuzz support],
  [AS_HELP_STRING([--enable-harfbuzz], [Enable the HarfBuzz bindings])],
  [no])

PHP_ARG_ENABLE([harfbuzz-probes],
  [whether to build the harfbuzz probe switch],
  [AS_HELP_STRING([--enable-harfbuzz-probes],
    [Build the PHPHB_PROBE_OFF switch that tests/Probes drives: env PHPHB_PROBE_OFF=<site id> turns that kept refusal off. Never for a release build])],
  [no],
  [no])

if test "$PHP_HARFBUZZ" != "no"; then
  if test "$PHP_HARFBUZZ_PROBES" != "no"; then
    AC_DEFINE([PHPHB_PROBES], [1], [Kept refusal sites can be turned off through env PHPHB_PROBE_OFF])
  fi

  dnl HarfBuzz 8.3.0 and FreeType 2.13.2 (freetype2 26.1.20) are the floors; hb-ft links both.
  PKG_CHECK_MODULES([HARFBUZZ], [harfbuzz >= 8.3.0 freetype2 >= 26.1.20])
  PHP_EVAL_INCLINE([$HARFBUZZ_CFLAGS])
  PHP_EVAL_LIBLINE([$HARFBUZZ_LIBS], [HARFBUZZ_SHARED_LIBADD])
  PHP_SUBST([HARFBUZZ_SHARED_LIBADD])

  PHP_NEW_EXTENSION([harfbuzz],
    [src/harfbuzz.c src/runtime.c src/hb_font.c src/hb_buffer.c src/hb_shape.c],
    [$ext_shared],, [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -Wall -Wextra])
  PHP_ADD_BUILD_DIR([$ext_builddir/src])
  PHP_ADD_EXTENSION_DEP(harfbuzz, freetype)
fi
