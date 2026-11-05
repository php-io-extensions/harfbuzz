#ifndef PHPHB_RUNTIME_H
#define PHPHB_RUNTIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_harfbuzz.h"
#include <hb.h>
#include <hb-ft.h>

ZEND_BEGIN_MODULE_GLOBALS(harfbuzz)
	HashTable boxes;               /* (native address, class) => zend_object*, live handles only, not refcounted */
ZEND_END_MODULE_GLOBALS(harfbuzz)

ZEND_EXTERN_MODULE_GLOBALS(harfbuzz)
#define PHPHB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(harfbuzz, v)

typedef struct {
	void *ptr;                     /* the native object; NULL once destroyed, or from fromPointer(0) */
	zval library;                  /* an hb-ft font: the ext-freetype FT_Library of its face, whose FT_Done_FreeType destroys the face */
	zend_object std;
} phphb_handle;

static zend_always_inline phphb_handle *phphb_handle_from(zend_object *obj)
{
	return (phphb_handle *) ((char *) obj - XtOffsetOf(phphb_handle, std));
}

/* Handle class setup: object layout, no clone, not serializable, final. */
void phphb_handle_setup(zend_class_entry *ce);
/* Struct class setup: final, no clone, not serializable. */
void phphb_struct_setup(zend_class_entry *ce);
/* The PHP object holding ptr, or a new one of class ce; NULL ptr → null. */
void phphb_box(zval *rv, void *ptr, zend_class_entry *ce);
/* The native pointer a handle holds (NULL once destroyed, or from fromPointer(0)). */
void *phphb_handle_ptr(zval *zv);
/* The native pointer of handle parameter arg_num for a HarfBuzz call that dereferences it: NULL, with
 * a ValueError, when the handle holds NULL or is an hb-ft font whose FT_Library was done. */
void *phphb_live_ptr(zval *zv, uint32_t arg_num);
/* The address an ext-freetype handle holds, through its pointer() method. */
zend_long phphb_freetype_pointer(zval *handle);
/* The native object is gone: pointer cleared, identity entry dropped. */
void phphb_release(zend_object *obj);

#ifdef PHPHB_PROBES
bool phphb_probe_off(const char *id);
# define PHPHB_PROBE_OFF(id) phphb_probe_off(id)
#else
# define PHPHB_PROBE_OFF(id) 0
#endif

/* __construct (private, empty), pointer() and fromPointer() for a handle class. fromPointer(0) answers a
 * handle that holds NULL, which HarfBuzz receives as NULL. Any other address is trusted. */
#define PHPHB_HANDLE_METHODS(cls) \
	ZEND_METHOD(cls, __construct) { ZEND_PARSE_PARAMETERS_NONE(); } \
	ZEND_METHOD(cls, pointer) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long) (uintptr_t) phphb_handle_ptr(ZEND_THIS)); } \
	ZEND_METHOD(cls, fromPointer) { zend_long p; ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(p) ZEND_PARSE_PARAMETERS_END(); \
		zend_class_entry *scope = zend_get_called_scope(execute_data); \
		if (p == 0) { object_init_ex(return_value, scope); return; } \
		phphb_box(return_value, (void *) (uintptr_t) p, scope); }

void phphb_register_hb_font(int module_number);
void phphb_register_hb_buffer(int module_number);
void phphb_register_hb_shape(int module_number);

#define PHPHB_SET_LONG(rv, field, value) zend_update_property_long(Z_OBJCE_P(rv), Z_OBJ_P(rv), field, sizeof(field) - 1, (zend_long) (value))

#endif
