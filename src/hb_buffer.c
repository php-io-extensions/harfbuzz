#include "runtime.h"
#include "../stubs/hb_buffer_arginfo.h"

zend_class_entry *phphb_ce_hb_buffer_t;
static zend_class_entry *phphb_ce_hb_language_t;
static zend_class_entry *phphb_ce_hb_glyph_info_t;
static zend_class_entry *phphb_ce_hb_glyph_position_t;

PHPHB_HANDLE_METHODS(hb_buffer_t)
PHPHB_HANDLE_METHODS(hb_language_t)

ZEND_FUNCTION(hb_buffer_create)
{
	ZEND_PARSE_PARAMETERS_NONE();

	phphb_box(return_value, hb_buffer_create(), phphb_ce_hb_buffer_t);
}

ZEND_FUNCTION(hb_buffer_destroy)
{
	zval *buffer;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
	ZEND_PARSE_PARAMETERS_END();

	/* hb_buffer_destroy takes NULL as a no-op: a second destroy passes through. */
	b = phphb_handle_ptr(buffer);
	hb_buffer_destroy(b);
	phphb_release(Z_OBJ_P(buffer));
}

ZEND_FUNCTION(hb_buffer_reset)
{
	zval *buffer;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_buffer_reset(b);
}

ZEND_FUNCTION(hb_buffer_guess_segment_properties)
{
	zval *buffer;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_buffer_guess_segment_properties(b);
}

ZEND_FUNCTION(hb_buffer_set_direction)
{
	zval *buffer;
	zend_long direction;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_LONG(direction)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_buffer_set_direction(b, (hb_direction_t) direction);
}

ZEND_FUNCTION(hb_buffer_set_script)
{
	zval *buffer;
	zend_long script;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_LONG(script)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_buffer_set_script(b, (hb_script_t) script);
}

ZEND_FUNCTION(hb_buffer_set_cluster_level)
{
	zval *buffer;
	zend_long cluster_level;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_LONG(cluster_level)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	hb_buffer_set_cluster_level(b, (hb_buffer_cluster_level_t) cluster_level);
}

ZEND_FUNCTION(hb_buffer_add_utf8)
{
	zval *buffer;
	zend_string *text;
	zend_long text_length, item_offset, item_length;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_STR(text)
		Z_PARAM_LONG(text_length)
		Z_PARAM_LONG(item_offset)
		Z_PARAM_LONG(item_length)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	/* HarfBuzz reads text_length bytes (-1: up to the NUL), and the item from text + item_offset for
	 * item_length bytes (-1: to the end) without checking either against the text. A negative length other
	 * than -1 makes HarfBuzz return without reading. */
	zend_long effective = text_length == -1 ? (zend_long) strlen(ZSTR_VAL(text)) : text_length;
	bool outside = effective >= 0 && (item_offset < 0 || item_offset > effective || (item_length >= 0 && item_length > effective - item_offset));
	if (!PHPHB_PROBE_OFF("harfbuzz-utf8-length") && ((text_length != -1 && text_length > (zend_long) ZSTR_LEN(text)) || outside)) {
		zend_argument_value_error(3, "must be -1 or not larger than the length of argument #2 ($text), and the item must lie inside the text"); /* binding:harfbuzz-utf8-length */
		RETURN_THROWS();
	}
	hb_buffer_add_utf8(b, ZSTR_VAL(text), (int) text_length, (unsigned int) item_offset, (int) item_length);
}

ZEND_FUNCTION(hb_buffer_set_language)
{
	zval *buffer, *language;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_OBJECT_OF_CLASS(language, phphb_ce_hb_language_t)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	/* A language holding NULL is HB_LANGUAGE_INVALID, which HarfBuzz takes. */
	hb_buffer_set_language(b, phphb_handle_ptr(language));
}

ZEND_FUNCTION(hb_language_from_string)
{
	zend_string *str;
	zend_long len;
	hb_language_t language;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(len)
	ZEND_PARSE_PARAMETERS_END();

	if (!PHPHB_PROBE_OFF("harfbuzz-language-length") && len > (zend_long) ZSTR_LEN(str)) {
		zend_argument_value_error(2, "must not be larger than the length of argument #1 ($str)"); /* binding:harfbuzz-language-length */
		RETURN_THROWS();
	}
	language = hb_language_from_string(ZSTR_VAL(str), (int) len);
	if (language == HB_LANGUAGE_INVALID) {
		/* HB_LANGUAGE_INVALID is NULL: a handle holding NULL, which HarfBuzz reads as no language. */
		object_init_ex(return_value, phphb_ce_hb_language_t);
		return;
	}
	phphb_box(return_value, (void *) language, phphb_ce_hb_language_t);
}

ZEND_FUNCTION(hb_buffer_get_length)
{
	zval *buffer;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_LONG(hb_buffer_get_length(b));
}

ZEND_FUNCTION(hb_buffer_get_glyph_infos)
{
	zval *buffer, *length, item;
	unsigned int count = 0;
	hb_glyph_info_t *infos;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_ZVAL(length)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	infos = hb_buffer_get_glyph_infos(b, &count);
	array_init_size(return_value, count);
	for (unsigned int i = 0; i < count; i++) {
		object_init_ex(&item, phphb_ce_hb_glyph_info_t);
		PHPHB_SET_LONG(&item, "codepoint", infos[i].codepoint);
		PHPHB_SET_LONG(&item, "mask", infos[i].mask);
		PHPHB_SET_LONG(&item, "cluster", infos[i].cluster);
		add_next_index_zval(return_value, &item);
	}
	ZEND_TRY_ASSIGN_REF_LONG(length, count);
}

ZEND_FUNCTION(hb_buffer_get_glyph_positions)
{
	zval *buffer, *length, item;
	unsigned int count = 0;
	hb_glyph_position_t *positions;
	hb_buffer_t *b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_ZVAL(length)
	ZEND_PARSE_PARAMETERS_END();

	b = phphb_live_ptr(buffer, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	positions = hb_buffer_get_glyph_positions(b, &count);
	array_init_size(return_value, count);
	for (unsigned int i = 0; positions != NULL && i < count; i++) {
		object_init_ex(&item, phphb_ce_hb_glyph_position_t);
		PHPHB_SET_LONG(&item, "x_advance", positions[i].x_advance);
		PHPHB_SET_LONG(&item, "y_advance", positions[i].y_advance);
		PHPHB_SET_LONG(&item, "x_offset", positions[i].x_offset);
		PHPHB_SET_LONG(&item, "y_offset", positions[i].y_offset);
		add_next_index_zval(return_value, &item);
	}
	ZEND_TRY_ASSIGN_REF_LONG(length, count);
}

void phphb_register_hb_buffer(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phphb_ce_hb_buffer_t = register_class_hb_buffer_t();
	phphb_handle_setup(phphb_ce_hb_buffer_t);
	phphb_ce_hb_language_t = register_class_hb_language_t();
	phphb_handle_setup(phphb_ce_hb_language_t);
	phphb_ce_hb_glyph_info_t = register_class_hb_glyph_info_t();
	phphb_struct_setup(phphb_ce_hb_glyph_info_t);
	phphb_ce_hb_glyph_position_t = register_class_hb_glyph_position_t();
	phphb_struct_setup(phphb_ce_hb_glyph_position_t);
}
