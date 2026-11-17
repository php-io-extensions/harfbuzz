#include "runtime.h"
#include "../stubs/hb_font_arginfo.h"

zend_class_entry *phphb_ce_hb_font_t;
static zend_class_entry *phphb_ce_hb_variation_t;

PHPHB_HANDLE_METHODS(hb_font_t)

/* The hb_variation_t list for hb_font_set_variations: one C struct per element, in list order. NULL with
 * an exception set when an element is not an hb_variation_t or a property was unset(); the caller
 * efree()s a non-NULL result. */
static hb_variation_t *phphb_variations(HashTable *list, uint32_t arg_num)
{
	hb_variation_t *variations = safe_emalloc(MAX(zend_hash_num_elements(list), 1), sizeof(hb_variation_t), 0);
	uint32_t i = 0;
	zval *element, rv, *tag, *value;

	ZEND_HASH_FOREACH_VAL(list, element) {
		ZVAL_DEREF(element);
		if (Z_TYPE_P(element) != IS_OBJECT || Z_OBJCE_P(element) != phphb_ce_hb_variation_t) {
			efree(variations);
			zend_argument_type_error(arg_num, "must be a list of hb_variation_t, %s given at position %u", zend_zval_value_name(element), i);
			return NULL;
		}
		/* Typed properties: an int and a float, or uninitialized after unset(), when PHP throws its own Error. */
		tag = zend_read_property(phphb_ce_hb_variation_t, Z_OBJ_P(element), ZEND_STRL("tag"), false, &rv);
		variations[i].tag = Z_TYPE_P(tag) == IS_LONG ? (hb_tag_t) Z_LVAL_P(tag) : 0;
		value = zend_read_property(phphb_ce_hb_variation_t, Z_OBJ_P(element), ZEND_STRL("value"), false, &rv);
		variations[i].value = Z_TYPE_P(value) == IS_DOUBLE ? (float) Z_DVAL_P(value) : 0.0f;
		if (EG(exception)) {
			efree(variations);
			return NULL;
		}
		i++;
	} ZEND_HASH_FOREACH_END();

	return variations;
}

ZEND_FUNCTION(hb_font_create)
{
	zval *face;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(face, phphb_ce_hb_face_t)
	ZEND_PARSE_PARAMETERS_END();

	/* HarfBuzz reads a NULL face as the empty face. */
	phphb_box(return_value, hb_font_create(phphb_handle_ptr(face)), phphb_ce_hb_font_t);
}

ZEND_FUNCTION(hb_font_destroy)
{
	zval *font;
	hb_font_t *f;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(font, phphb_ce_hb_font_t)
	ZEND_PARSE_PARAMETERS_END();

	/* hb_font_destroy takes NULL as a no-op; a font whose face was destroyed with its library is not one. */
	f = phphb_handle_ptr(font);
	if (f != NULL) {
		f = phphb_live_ptr(font, 1);
		if (EG(exception)) {
			RETURN_THROWS();
		}
	}
	hb_font_destroy(f);
	phphb_release(Z_OBJ_P(font));
}

ZEND_FUNCTION(hb_font_set_scale)
{
	zval *font;
	zend_long x_scale, y_scale;
	hb_font_t *f;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS(font, phphb_ce_hb_font_t)
		Z_PARAM_LONG(x_scale)
		Z_PARAM_LONG(y_scale)
	ZEND_PARSE_PARAMETERS_END();

	f = phphb_live_ptr(font, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_font_set_scale(f, (int) x_scale, (int) y_scale);
}

ZEND_FUNCTION(hb_font_set_variations)
{
	zval *font;
	HashTable *list = NULL;
	zend_long variations_length;
	hb_variation_t *variations = NULL;
	hb_font_t *f;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS(font, phphb_ce_hb_font_t)
		Z_PARAM_ARRAY_HT_OR_NULL(list)
		Z_PARAM_LONG(variations_length)
	ZEND_PARSE_PARAMETERS_END();

	f = phphb_live_ptr(font, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	if (!PHPHB_PROBE_OFF("harfbuzz-variations-length") && (variations_length < 0 || variations_length > (list != NULL ? (zend_long) zend_hash_num_elements(list) : 0))) {
		zend_argument_value_error(3, "must not be larger than the number of elements in argument #2 ($variations)"); /* binding:harfbuzz-variations-length */
		RETURN_THROWS();
	}
	if (list != NULL && (variations = phphb_variations(list, 2)) == NULL) {
		RETURN_THROWS();
	}
	hb_font_set_variations(f, variations, (unsigned int) variations_length);
	if (variations != NULL) {
		efree(variations);
	}
}

void phphb_register_hb_font(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phphb_ce_hb_font_t = register_class_hb_font_t();
	phphb_handle_setup(phphb_ce_hb_font_t);
	phphb_ce_hb_variation_t = register_class_hb_variation_t();
	phphb_struct_setup(phphb_ce_hb_variation_t);
}
