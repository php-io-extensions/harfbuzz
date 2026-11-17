/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3a8805e3ffc10a5f470cf780a8adfdecc07141a2 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_font_create, 0, 1, hb_font_t, 0)
	ZEND_ARG_OBJ_INFO(0, face, hb_face_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_font_destroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_font_set_scale, 0, 3, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
	ZEND_ARG_TYPE_INFO(0, x_scale, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y_scale, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_font_set_variations, 0, 3, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
	ZEND_ARG_TYPE_INFO(0, variations, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, variations_length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hb_font_t___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_font_t_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_font_t_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(hb_font_create);
ZEND_FUNCTION(hb_font_destroy);
ZEND_FUNCTION(hb_font_set_scale);
ZEND_FUNCTION(hb_font_set_variations);
ZEND_METHOD(hb_font_t, __construct);
ZEND_METHOD(hb_font_t, pointer);
ZEND_METHOD(hb_font_t, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_font_create, arginfo_hb_font_create)
	ZEND_FE(hb_font_destroy, arginfo_hb_font_destroy)
	ZEND_FE(hb_font_set_scale, arginfo_hb_font_set_scale)
	ZEND_FE(hb_font_set_variations, arginfo_hb_font_set_variations)
	ZEND_FE_END
};

static const zend_function_entry class_hb_font_t_methods[] = {
	ZEND_ME(hb_font_t, __construct, arginfo_class_hb_font_t___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(hb_font_t, pointer, arginfo_class_hb_font_t_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(hb_font_t, fromPointer, arginfo_class_hb_font_t_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_hb_font_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_font_t", class_hb_font_t_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_hb_variation_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_variation_t", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_tag_default_value;
	ZVAL_LONG(&property_tag_default_value, 0);
	zend_string *property_tag_name = zend_string_init("tag", sizeof("tag") - 1, 1);
	zend_declare_typed_property(class_entry, property_tag_name, &property_tag_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_tag_name);

	zval property_value_default_value;
	ZVAL_DOUBLE(&property_value_default_value, 0.0);
	zend_string *property_value_name = zend_string_init("value", sizeof("value") - 1, 1);
	zend_declare_typed_property(class_entry, property_value_name, &property_value_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release(property_value_name);

	return class_entry;
}
