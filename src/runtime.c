#include "runtime.h"
#include "zend_interfaces.h"

static zend_object_handlers phphb_handle_handlers;
static zend_object_handlers phphb_struct_handlers;

static zend_object *phphb_create_object(zend_class_entry *ce)
{
	phphb_handle *intern = zend_object_alloc(sizeof(phphb_handle), ce);

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &phphb_handle_handlers;
	intern->ptr = NULL;
	ZVAL_UNDEF(&intern->library);

	return &intern->std;
}

/* The boxes key: one PHP object per native address and class, so a fromPointer() of another class at the
 * same address does not clear a live handle. */
typedef struct {
	void *ptr;
	zend_class_entry *ce;
} phphb_box_key;

static void phphb_forget(phphb_handle *intern)
{
	phphb_box_key key = { intern->ptr, intern->std.ce };

	if (intern->ptr != NULL
		&& zend_hash_str_find_ptr(&PHPHB_G(boxes), (const char *) &key, sizeof(key)) == &intern->std) {
		zend_hash_str_del(&PHPHB_G(boxes), (const char *) &key, sizeof(key));
	}
}

static void phphb_free_object(zend_object *object)
{
	phphb_handle *intern = phphb_handle_from(object);

	phphb_forget(intern);
	intern->ptr = NULL;
	zval_ptr_dtor(&intern->library);
	zend_object_std_dtor(object);
}

void phphb_handle_setup(zend_class_entry *ce)
{
	static bool ready = false;

	if (!ready) {
		memcpy(&phphb_handle_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		phphb_handle_handlers.offset = XtOffsetOf(phphb_handle, std);
		phphb_handle_handlers.free_obj = phphb_free_object;
		phphb_handle_handlers.clone_obj = NULL;
		ready = true;
	}
	ce->create_object = phphb_create_object;
	ce->default_object_handlers = &phphb_handle_handlers;
	ce->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE | ZEND_ACC_FINAL;
}

void phphb_struct_setup(zend_class_entry *ce)
{
	static bool ready = false;

	if (!ready) {
		memcpy(&phphb_struct_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		phphb_struct_handlers.clone_obj = NULL;
		ready = true;
	}
	ce->default_object_handlers = &phphb_struct_handlers;
	ce->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE | ZEND_ACC_FINAL;
}

void phphb_box(zval *rv, void *ptr, zend_class_entry *ce)
{
	phphb_box_key key = { ptr, ce };
	zend_object *existing;

	if (ptr == NULL) {
		ZVAL_NULL(rv);
		return;
	}
	existing = zend_hash_str_find_ptr(&PHPHB_G(boxes), (const char *) &key, sizeof(key));
	if (existing != NULL) {
		ZVAL_OBJ_COPY(rv, existing);
		return;
	}
	object_init_ex(rv, ce);
	phphb_handle_from(Z_OBJ_P(rv))->ptr = ptr;
	zend_hash_str_update_ptr(&PHPHB_G(boxes), (const char *) &key, sizeof(key), Z_OBJ_P(rv));
}

void *phphb_handle_ptr(zval *zv)
{
	return phphb_handle_from(Z_OBJ_P(zv))->ptr;
}

zend_long phphb_freetype_pointer(zval *handle)
{
	zval address;
	zend_long ptr;

	zend_call_method_with_0_params(Z_OBJ_P(handle), Z_OBJCE_P(handle), NULL, "pointer", &address);
	ptr = Z_TYPE(address) == IS_LONG ? Z_LVAL(address) : 0;
	zval_ptr_dtor(&address);

	return ptr;
}

void *phphb_live_ptr(zval *zv, uint32_t arg_num)
{
	phphb_handle *intern = phphb_handle_from(Z_OBJ_P(zv));
	void *ptr = intern->ptr;
	/* FT_Done_FreeType destroys every face of its library whatever its reference count: the font's face is gone. */
	bool face_gone = ptr != NULL && Z_TYPE(intern->library) == IS_OBJECT && phphb_freetype_pointer(&intern->library) == 0;

	if (!PHPHB_PROBE_OFF("harfbuzz-null-handle") && (ptr == NULL || face_gone)) {
		zend_argument_value_error(arg_num, ptr == NULL ? "holds NULL: it was destroyed, or made from address 0" : "reads a face that FT_Done_FreeType destroyed"); /* binding:harfbuzz-null-handle */
	}
	return ptr;
}

void phphb_release(zend_object *obj)
{
	phphb_handle *intern = phphb_handle_from(obj);

	phphb_forget(intern);
	intern->ptr = NULL;
}

#ifdef PHPHB_PROBES
bool phphb_probe_off(const char *id)
{
	const char *off = getenv("PHPHB_PROBE_OFF");

	return off != NULL && strcmp(off, id) == 0;
}
#endif
