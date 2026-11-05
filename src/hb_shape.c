#include "runtime.h"
#include "../stubs/hb_shape_arginfo.h"

extern zend_class_entry *phphb_ce_hb_font_t;
extern zend_class_entry *phphb_ce_hb_buffer_t;
static zend_class_entry *phphb_ce_hb_feature_t;

/* A typed int property of a struct object: the type is enforced on write, so it is an int or, after
 * unset(), uninitialized; then PHP's own Error is thrown and the caller checks EG(exception). */
static zend_long phphb_long_prop(zend_object *obj, const char *name, size_t len)
{
	zval rv, *value = zend_read_property(obj->ce, obj, name, len, false, &rv);

	return Z_TYPE_P(value) == IS_LONG ? Z_LVAL_P(value) : 0;
}

/* The hb_feature_t list for hb_shape: one C struct per element, in list order. NULL with an exception
 * set when an element is not an hb_feature_t; the caller efree()s a non-NULL result. */
static hb_feature_t *phphb_features(HashTable *list, uint32_t arg_num)
{
	hb_feature_t *features = safe_emalloc(MAX(zend_hash_num_elements(list), 1), sizeof(hb_feature_t), 0);
	uint32_t i = 0;
	zval *element;

	ZEND_HASH_FOREACH_VAL(list, element) {
		ZVAL_DEREF(element);
		if (Z_TYPE_P(element) != IS_OBJECT || Z_OBJCE_P(element) != phphb_ce_hb_feature_t) {
			efree(features);
			zend_argument_type_error(arg_num, "must be a list of hb_feature_t, %s given at position %u", zend_zval_value_name(element), i);
			return NULL;
		}
		features[i].tag = (hb_tag_t) phphb_long_prop(Z_OBJ_P(element), ZEND_STRL("tag"));
		features[i].value = (uint32_t) phphb_long_prop(Z_OBJ_P(element), ZEND_STRL("value"));
		features[i].start = (unsigned int) phphb_long_prop(Z_OBJ_P(element), ZEND_STRL("start"));
		features[i].end = (unsigned int) phphb_long_prop(Z_OBJ_P(element), ZEND_STRL("end"));
		if (EG(exception)) {
			efree(features);
			return NULL;
		}
		i++;
	} ZEND_HASH_FOREACH_END();

	return features;
}

ZEND_FUNCTION(hb_shape)
{
	zval *font, *buffer;
	HashTable *list = NULL;
	zend_long num_features;
	hb_feature_t *features = NULL;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(font, phphb_ce_hb_font_t)
		Z_PARAM_OBJECT_OF_CLASS(buffer, phphb_ce_hb_buffer_t)
		Z_PARAM_ARRAY_HT_OR_NULL(list)
		Z_PARAM_LONG(num_features)
	ZEND_PARSE_PARAMETERS_END();

	hb_font_t *f = phphb_live_ptr(font, 1);
	hb_buffer_t *b = EG(exception) ? NULL : phphb_live_ptr(buffer, 2);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	if (!PHPHB_PROBE_OFF("harfbuzz-num-features") && (num_features < 0 || num_features > (list != NULL ? (zend_long) zend_hash_num_elements(list) : 0))) {
		zend_argument_value_error(4, "must not be larger than the number of elements in argument #3 ($features)"); /* binding:harfbuzz-num-features */
		RETURN_THROWS();
	}
	if (list != NULL && (features = phphb_features(list, 3)) == NULL) {
		RETURN_THROWS();
	}
	hb_shape(f, b, features, (unsigned int) num_features);
	if (features != NULL) {
		efree(features);
	}
}

ZEND_FUNCTION(hb_feature_from_string)
{
	zend_string *str;
	zend_long len;
	zval *out, feature_obj;
	hb_feature_t feature;
	hb_bool_t ok;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(len)
		Z_PARAM_ZVAL(out)
	ZEND_PARSE_PARAMETERS_END();

	memset(&feature, 0, sizeof(feature));
	ok = hb_feature_from_string(ZSTR_VAL(str), (int) len, &feature);

	object_init_ex(&feature_obj, phphb_ce_hb_feature_t);
	PHPHB_SET_LONG(&feature_obj, "tag", feature.tag);
	PHPHB_SET_LONG(&feature_obj, "value", feature.value);
	PHPHB_SET_LONG(&feature_obj, "start", feature.start);
	PHPHB_SET_LONG(&feature_obj, "end", feature.end);
	ZEND_TRY_ASSIGN_REF_TMP(out, &feature_obj);

	RETURN_BOOL(ok);
}

void phphb_register_hb_shape(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phphb_ce_hb_feature_t = register_class_hb_feature_t();
	phphb_struct_setup(phphb_ce_hb_feature_t);
}
