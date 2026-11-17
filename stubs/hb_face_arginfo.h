/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 905c29f789510f98c5bfc2e81539962895f2987f */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_blob_create_from_file, 0, 1, hb_blob_t, 0)
	ZEND_ARG_TYPE_INFO(0, file_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_blob_destroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, blob, hb_blob_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_face_count, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, blob, hb_blob_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_face_create, 0, 2, hb_face_t, 0)
	ZEND_ARG_OBJ_INFO(0, blob, hb_blob_t, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_face_destroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, face, hb_face_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hb_blob_t___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_blob_t_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_blob_t_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_hb_face_t___construct arginfo_class_hb_blob_t___construct

#define arginfo_class_hb_face_t_pointer arginfo_class_hb_blob_t_pointer

#define arginfo_class_hb_face_t_fromPointer arginfo_class_hb_blob_t_fromPointer

ZEND_FUNCTION(hb_blob_create_from_file);
ZEND_FUNCTION(hb_blob_destroy);
ZEND_FUNCTION(hb_face_count);
ZEND_FUNCTION(hb_face_create);
ZEND_FUNCTION(hb_face_destroy);
ZEND_METHOD(hb_blob_t, __construct);
ZEND_METHOD(hb_blob_t, pointer);
ZEND_METHOD(hb_blob_t, fromPointer);
ZEND_METHOD(hb_face_t, __construct);
ZEND_METHOD(hb_face_t, pointer);
ZEND_METHOD(hb_face_t, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_blob_create_from_file, arginfo_hb_blob_create_from_file)
	ZEND_FE(hb_blob_destroy, arginfo_hb_blob_destroy)
	ZEND_FE(hb_face_count, arginfo_hb_face_count)
	ZEND_FE(hb_face_create, arginfo_hb_face_create)
	ZEND_FE(hb_face_destroy, arginfo_hb_face_destroy)
	ZEND_FE_END
};

static const zend_function_entry class_hb_blob_t_methods[] = {
	ZEND_ME(hb_blob_t, __construct, arginfo_class_hb_blob_t___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(hb_blob_t, pointer, arginfo_class_hb_blob_t_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(hb_blob_t, fromPointer, arginfo_class_hb_blob_t_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_hb_face_t_methods[] = {
	ZEND_ME(hb_face_t, __construct, arginfo_class_hb_face_t___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(hb_face_t, pointer, arginfo_class_hb_face_t_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(hb_face_t, fromPointer, arginfo_class_hb_face_t_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_hb_blob_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_blob_t", class_hb_blob_t_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_hb_face_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_face_t", class_hb_face_t_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
