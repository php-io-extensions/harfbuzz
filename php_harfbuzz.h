#ifndef PHP_HARFBUZZ_H
#define PHP_HARFBUZZ_H

extern zend_module_entry harfbuzz_module_entry;
#define phpext_harfbuzz_ptr &harfbuzz_module_entry

#define PHP_HARFBUZZ_VERSION "0.11.0"

#if defined(ZTS) && defined(COMPILE_DL_HARFBUZZ)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
