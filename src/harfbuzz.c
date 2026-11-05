/*
 * harfbuzz: 1:1 bindings of HarfBuzz at the 8.3.0 API. Functions, handles, structs and constants keep
 * their C names. Dropping the last PHP reference does not destroy the native object. hb-ft takes the
 * FT_Face handles of ext-freetype.
 */

#include "runtime.h"
#include "ext/standard/info.h"
#include "../stubs/harfbuzz_arginfo.h"

ZEND_DECLARE_MODULE_GLOBALS(harfbuzz)

static PHP_GINIT_FUNCTION(harfbuzz)
{
#if defined(COMPILE_DL_HARFBUZZ) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	zend_hash_init(&harfbuzz_globals->boxes, 64, NULL, NULL, 1);
}

static PHP_GSHUTDOWN_FUNCTION(harfbuzz)
{
	zend_hash_destroy(&harfbuzz_globals->boxes);
}

PHP_MINIT_FUNCTION(harfbuzz)
{
	register_harfbuzz_symbols(module_number);
#ifdef PHPHB_PROBES
	/* A probe build: env PHPHB_PROBE_OFF=<site id> turns that kept refusal off (tests/Probes). */
	REGISTER_BOOL_CONSTANT("PHPHB_PROBES", true, CONST_PERSISTENT);
#endif
	phphb_register_hb_font(module_number);
	phphb_register_hb_buffer(module_number);
	phphb_register_hb_shape(module_number);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(harfbuzz)
{
#if defined(COMPILE_DL_HARFBUZZ) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_MINFO_FUNCTION(harfbuzz)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "harfbuzz support", "enabled");
	php_info_print_table_row(2, "Version", PHP_HARFBUZZ_VERSION);
	php_info_print_table_row(2, "HarfBuzz headers", HB_VERSION_STRING);
	php_info_print_table_row(2, "HarfBuzz library", hb_version_string());
	php_info_print_table_end();
}

static const zend_module_dep harfbuzz_deps[] = {
	ZEND_MOD_REQUIRED("freetype")
	ZEND_MOD_END
};

zend_module_entry harfbuzz_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	harfbuzz_deps,
	"harfbuzz",
	NULL,
	PHP_MINIT(harfbuzz),
	NULL,
	PHP_RINIT(harfbuzz),
	NULL,
	PHP_MINFO(harfbuzz),
	PHP_HARFBUZZ_VERSION,
	PHP_MODULE_GLOBALS(harfbuzz),
	PHP_GINIT(harfbuzz),
	PHP_GSHUTDOWN(harfbuzz),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_HARFBUZZ
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(harfbuzz)
#endif
