/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: acaa813eecdb4283bc2efa3a9de0fcdc26b3648c */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_buffer_create, 0, 0, hb_buffer_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_destroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
ZEND_END_ARG_INFO()

#define arginfo_hb_buffer_reset arginfo_hb_buffer_destroy

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_add_utf8, 0, 5, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, text_length, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, item_offset, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, item_length, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_hb_buffer_guess_segment_properties arginfo_hb_buffer_destroy

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_set_direction, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(0, direction, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_set_script, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(0, script, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_set_language, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_OBJ_INFO(0, language, hb_language_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_language_from_string, 0, 2, hb_language_t, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_set_cluster_level, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(0, cluster_level, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_get_length, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_buffer_get_glyph_infos, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, buffer, hb_buffer_t, 0)
	ZEND_ARG_TYPE_INFO(1, length, IS_LONG, 1)
ZEND_END_ARG_INFO()

#define arginfo_hb_buffer_get_glyph_positions arginfo_hb_buffer_get_glyph_infos

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hb_buffer_t___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_buffer_t_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_buffer_t_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_hb_language_t___construct arginfo_class_hb_buffer_t___construct

#define arginfo_class_hb_language_t_pointer arginfo_class_hb_buffer_t_pointer

#define arginfo_class_hb_language_t_fromPointer arginfo_class_hb_buffer_t_fromPointer

ZEND_FUNCTION(hb_buffer_create);
ZEND_FUNCTION(hb_buffer_destroy);
ZEND_FUNCTION(hb_buffer_reset);
ZEND_FUNCTION(hb_buffer_add_utf8);
ZEND_FUNCTION(hb_buffer_guess_segment_properties);
ZEND_FUNCTION(hb_buffer_set_direction);
ZEND_FUNCTION(hb_buffer_set_script);
ZEND_FUNCTION(hb_buffer_set_language);
ZEND_FUNCTION(hb_language_from_string);
ZEND_FUNCTION(hb_buffer_set_cluster_level);
ZEND_FUNCTION(hb_buffer_get_length);
ZEND_FUNCTION(hb_buffer_get_glyph_infos);
ZEND_FUNCTION(hb_buffer_get_glyph_positions);
ZEND_METHOD(hb_buffer_t, __construct);
ZEND_METHOD(hb_buffer_t, pointer);
ZEND_METHOD(hb_buffer_t, fromPointer);
ZEND_METHOD(hb_language_t, __construct);
ZEND_METHOD(hb_language_t, pointer);
ZEND_METHOD(hb_language_t, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_buffer_create, arginfo_hb_buffer_create)
	ZEND_FE(hb_buffer_destroy, arginfo_hb_buffer_destroy)
	ZEND_FE(hb_buffer_reset, arginfo_hb_buffer_reset)
	ZEND_FE(hb_buffer_add_utf8, arginfo_hb_buffer_add_utf8)
	ZEND_FE(hb_buffer_guess_segment_properties, arginfo_hb_buffer_guess_segment_properties)
	ZEND_FE(hb_buffer_set_direction, arginfo_hb_buffer_set_direction)
	ZEND_FE(hb_buffer_set_script, arginfo_hb_buffer_set_script)
	ZEND_FE(hb_buffer_set_language, arginfo_hb_buffer_set_language)
	ZEND_FE(hb_language_from_string, arginfo_hb_language_from_string)
	ZEND_FE(hb_buffer_set_cluster_level, arginfo_hb_buffer_set_cluster_level)
	ZEND_FE(hb_buffer_get_length, arginfo_hb_buffer_get_length)
	ZEND_FE(hb_buffer_get_glyph_infos, arginfo_hb_buffer_get_glyph_infos)
	ZEND_FE(hb_buffer_get_glyph_positions, arginfo_hb_buffer_get_glyph_positions)
	ZEND_FE_END
};

static const zend_function_entry class_hb_buffer_t_methods[] = {
	ZEND_ME(hb_buffer_t, __construct, arginfo_class_hb_buffer_t___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(hb_buffer_t, pointer, arginfo_class_hb_buffer_t_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(hb_buffer_t, fromPointer, arginfo_class_hb_buffer_t_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_hb_language_t_methods[] = {
	ZEND_ME(hb_language_t, __construct, arginfo_class_hb_language_t___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(hb_language_t, pointer, arginfo_class_hb_language_t_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(hb_language_t, fromPointer, arginfo_class_hb_language_t_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_hb_buffer_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_buffer_t", class_hb_buffer_t_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_hb_language_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_language_t", class_hb_language_t_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_hb_glyph_info_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_glyph_info_t", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_codepoint_default_value;
	ZVAL_LONG(&property_codepoint_default_value, 0);
	zend_string *property_codepoint_name = zend_string_init("codepoint", sizeof("codepoint") - 1, 1);
	zend_declare_typed_property(class_entry, property_codepoint_name, &property_codepoint_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_codepoint_name);

	zval property_mask_default_value;
	ZVAL_LONG(&property_mask_default_value, 0);
	zend_string *property_mask_name = zend_string_init("mask", sizeof("mask") - 1, 1);
	zend_declare_typed_property(class_entry, property_mask_name, &property_mask_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_mask_name);

	zval property_cluster_default_value;
	ZVAL_LONG(&property_cluster_default_value, 0);
	zend_string *property_cluster_name = zend_string_init("cluster", sizeof("cluster") - 1, 1);
	zend_declare_typed_property(class_entry, property_cluster_name, &property_cluster_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_cluster_name);

	return class_entry;
}

static zend_class_entry *register_class_hb_glyph_position_t(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "hb_glyph_position_t", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_x_advance_default_value;
	ZVAL_LONG(&property_x_advance_default_value, 0);
	zend_string *property_x_advance_name = zend_string_init("x_advance", sizeof("x_advance") - 1, 1);
	zend_declare_typed_property(class_entry, property_x_advance_name, &property_x_advance_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_x_advance_name);

	zval property_y_advance_default_value;
	ZVAL_LONG(&property_y_advance_default_value, 0);
	zend_string *property_y_advance_name = zend_string_init("y_advance", sizeof("y_advance") - 1, 1);
	zend_declare_typed_property(class_entry, property_y_advance_name, &property_y_advance_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_y_advance_name);

	zval property_x_offset_default_value;
	ZVAL_LONG(&property_x_offset_default_value, 0);
	zend_string *property_x_offset_name = zend_string_init("x_offset", sizeof("x_offset") - 1, 1);
	zend_declare_typed_property(class_entry, property_x_offset_name, &property_x_offset_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_x_offset_name);

	zval property_y_offset_default_value;
	ZVAL_LONG(&property_y_offset_default_value, 0);
	zend_string *property_y_offset_name = zend_string_init("y_offset", sizeof("y_offset") - 1, 1);
	zend_declare_typed_property(class_entry, property_y_offset_name, &property_y_offset_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_y_offset_name);

	return class_entry;
}
