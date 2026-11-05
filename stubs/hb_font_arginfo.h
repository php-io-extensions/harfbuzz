/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c0e078b7a8557f7e83b2b1a65d4747e31376d8c9 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_ft_font_create_referenced, 0, 1, hb_font_t, 0)
	ZEND_ARG_OBJ_INFO(0, ft_face, FT_Face, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_font_destroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_font_set_scale, 0, 3, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
	ZEND_ARG_TYPE_INFO(0, x_scale, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y_scale, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_hb_ft_font_changed arginfo_hb_font_destroy

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hb_font_t___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_font_t_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_hb_font_t_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(hb_ft_font_create_referenced);
ZEND_FUNCTION(hb_font_destroy);
ZEND_FUNCTION(hb_font_set_scale);
ZEND_FUNCTION(hb_ft_font_changed);
ZEND_METHOD(hb_font_t, __construct);
ZEND_METHOD(hb_font_t, pointer);
ZEND_METHOD(hb_font_t, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_ft_font_create_referenced, arginfo_hb_ft_font_create_referenced)
	ZEND_FE(hb_font_destroy, arginfo_hb_font_destroy)
	ZEND_FE(hb_font_set_scale, arginfo_hb_font_set_scale)
	ZEND_FE(hb_ft_font_changed, arginfo_hb_ft_font_changed)
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
