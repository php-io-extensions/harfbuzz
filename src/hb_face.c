#include "runtime.h"
#include "../stubs/hb_face_arginfo.h"

zend_class_entry *phphb_ce_hb_blob_t;
zend_class_entry *phphb_ce_hb_face_t;

PHPHB_HANDLE_METHODS(hb_blob_t)
PHPHB_HANDLE_METHODS(hb_face_t)

ZEND_FUNCTION(hb_blob_create_from_file)
{
	zend_string *file_name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(file_name)
	ZEND_PARSE_PARAMETERS_END();

	/* Never NULL: a file that does not open answers hb_blob_get_empty(). */
	phphb_box(return_value, hb_blob_create_from_file(ZSTR_VAL(file_name)), phphb_ce_hb_blob_t);
}

ZEND_FUNCTION(hb_blob_destroy)
{
	zval *blob;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(blob, phphb_ce_hb_blob_t)
	ZEND_PARSE_PARAMETERS_END();

	/* HarfBuzz takes NULL as a no-op. */
	hb_blob_destroy(phphb_handle_ptr(blob));
	phphb_release(Z_OBJ_P(blob));
}

ZEND_FUNCTION(hb_face_count)
{
	zval *blob;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(blob, phphb_ce_hb_blob_t)
	ZEND_PARSE_PARAMETERS_END();

	/* HarfBuzz answers 0 for a NULL blob. */
	RETURN_LONG((zend_long) hb_face_count(phphb_handle_ptr(blob)));
}

ZEND_FUNCTION(hb_face_create)
{
	zval *blob;
	zend_long index;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(blob, phphb_ce_hb_blob_t)
		Z_PARAM_LONG(index)
	ZEND_PARSE_PARAMETERS_END();

	/* HarfBuzz reads a NULL blob as the empty blob. */
	phphb_box(return_value, hb_face_create(phphb_handle_ptr(blob), (unsigned int) index), phphb_ce_hb_face_t);
}

ZEND_FUNCTION(hb_face_destroy)
{
	zval *face;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(face, phphb_ce_hb_face_t)
	ZEND_PARSE_PARAMETERS_END();

	/* HarfBuzz takes NULL as a no-op. */
	hb_face_destroy(phphb_handle_ptr(face));
	phphb_release(Z_OBJ_P(face));
}

void phphb_register_hb_face(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phphb_ce_hb_blob_t = register_class_hb_blob_t();
	phphb_handle_setup(phphb_ce_hb_blob_t);
	phphb_ce_hb_face_t = register_class_hb_face_t();
	phphb_handle_setup(phphb_ce_hb_face_t);
}
