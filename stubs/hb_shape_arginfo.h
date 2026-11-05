/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a6031a6110d12e4eb924c1f4ce0969d6bfc89bf1 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_shape, 0, 4, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(0, features, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, num_features, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_feature_from_string, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, feature, hb_feature_t, 1)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(hb_shape);
ZEND_FUNCTION(hb_feature_from_string);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_shape, arginfo_hb_shape)
	ZEND_FE(hb_feature_from_string, arginfo_hb_feature_from_string)
	ZEND_FE_END
};

static zend_class_entry *register_class_hb_feature_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_feature_t", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_tag_default_value;
	ZVAL_LONG(&property_tag_default_value, 0);
	zend_string *property_tag_name = zend_string_init("tag", sizeof("tag") - 1, 1);
	zend_declare_typed_property(class_entry, property_tag_name, &property_tag_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_tag_name);

	zval property_value_default_value;
	ZVAL_LONG(&property_value_default_value, 0);
	zend_string *property_value_name = zend_string_init("value", sizeof("value") - 1, 1);
	zend_declare_typed_property(class_entry, property_value_name, &property_value_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_value_name);

	zval property_start_default_value;
	ZVAL_LONG(&property_start_default_value, 0);
	zend_string *property_start_name = zend_string_init("start", sizeof("start") - 1, 1);
	zend_declare_typed_property(class_entry, property_start_name, &property_start_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_start_name);

	zval property_end_default_value;
	ZVAL_LONG(&property_end_default_value, 0);
	zend_string *property_end_name = zend_string_init("end", sizeof("end") - 1, 1);
	zend_declare_typed_property(class_entry, property_end_name, &property_end_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_end_name);

	return class_entry;
}
