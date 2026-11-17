#include "runtime.h"
#include "zend_interfaces.h"
#include "../stubs/hb_ft_arginfo.h"

static zend_class_entry *phphb_ce_FT_Face;
static zend_class_entry *phphb_ce_FT_Library;

ZEND_FUNCTION(hb_ft_font_create_referenced)
{
	zval *ft_face, address, library;
	FT_Face face;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(ft_face, phphb_ce_FT_Face)
	ZEND_PARSE_PARAMETERS_END();

	face = (FT_Face) (uintptr_t) phphb_freetype_pointer(ft_face);
	if (!PHPHB_PROBE_OFF("harfbuzz-ft-face-null") && face == NULL) {
		zend_argument_value_error(1, "holds NULL: it was released, or made from address 0"); /* binding:harfbuzz-ft-face-null */
		RETURN_THROWS();
	}
	phphb_box(return_value, hb_ft_font_create_referenced(face), phphb_ce_hb_font_t);

	/* The ext-freetype FT_Library object of the face (FT_GlyphSlotRec.library): FT_Done_FreeType on it
	 * destroys the face this font reads. */
	if (face != NULL && face->glyph != NULL && Z_TYPE_P(return_value) == IS_OBJECT) {
		phphb_handle *font = phphb_handle_from(Z_OBJ_P(return_value));

		ZVAL_LONG(&address, (zend_long) (uintptr_t) face->glyph->library);
		zend_call_method_with_1_params(NULL, phphb_ce_FT_Library, NULL, "frompointer", &library, &address);
		zval_ptr_dtor(&font->library);
		ZVAL_COPY_VALUE(&font->library, &library);
	}
}

ZEND_FUNCTION(hb_ft_font_changed)
{
	zval *font;
	hb_font_t *f;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(font, phphb_ce_hb_font_t)
	ZEND_PARSE_PARAMETERS_END();

	f = phphb_live_ptr(font, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_ft_font_changed(f);
}

void phphb_register_hb_ft(int module_number)
{
	(void) module_number;
	/* ZEND_MOD_OPTIONAL("freetype") starts ext-freetype first when it is loaded. Without it no FT_Face
	 * exists, so no hb-ft font can be made, and hb-ft is not registered. */
	phphb_ce_FT_Face = zend_hash_str_find_ptr(CG(class_table), ZEND_STRL("ft_face"));
	phphb_ce_FT_Library = zend_hash_str_find_ptr(CG(class_table), ZEND_STRL("ft_library"));
	if (phphb_ce_FT_Face == NULL || phphb_ce_FT_Library == NULL) {
		return;
	}
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
}
