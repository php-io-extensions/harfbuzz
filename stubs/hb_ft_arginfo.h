/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 28948488069be88cd225b0015c62d90bc6ba9e05 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_hb_ft_font_create_referenced, 0, 1, hb_font_t, 0)
	ZEND_ARG_OBJ_INFO(0, ft_face, FT_Face, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hb_ft_font_changed, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, font, hb_font_t, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(hb_ft_font_create_referenced);
ZEND_FUNCTION(hb_ft_font_changed);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(hb_ft_font_create_referenced, arginfo_hb_ft_font_create_referenced)
	ZEND_FE(hb_ft_font_changed, arginfo_hb_ft_font_changed)
	ZEND_FE_END
};
