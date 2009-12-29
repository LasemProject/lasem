/*
 * Copyright © 2007-2009  Emmanuel Pacaud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmmathmlattributes.h>

gboolean
lsm_mathml_boolean_attribute_inherit (LsmMathmlBooleanAttribute *attribute, gboolean value)
{
	if (attribute->base.value == NULL)
		attribute->value = value;

	return attribute->value;
}

unsigned int
lsm_mathml_enum_attribute_inherit (LsmMathmlEnumAttribute *attribute, unsigned int value)
{
	if (attribute->base.value == NULL)
		attribute->value = value;

	return attribute->value;
}

double
lsm_mathml_double_attribute_inherit (LsmMathmlDoubleAttribute *attribute, double value)
{
	if (attribute->base.value == NULL)
		attribute->value = value;

	return attribute->value;
}

LsmMathmlColor
lsm_mathml_color_attribute_inherit (LsmMathmlColorAttribute *attribute, LsmMathmlColor value)
{
	if (attribute->base.value == NULL)
		attribute->color = value;

	return attribute->color;
}

const char *
lsm_mathml_string_attribute_inherit (LsmMathmlStringAttribute *attribute, const char *string)
{
	if (attribute->base.value == NULL) {
		g_free (attribute->value);
		attribute->value = g_strdup (string);
	}

	return attribute->value;
}

int
lsm_mathml_script_level_attribute_apply	(LsmMathmlScriptLevelAttribute *attribute,
					 int script_level)
{
	if (attribute->base.value == NULL) {
		attribute->value.level = script_level;
		attribute->value.sign = LSM_MATHML_SCRIPT_LEVEL_SIGN_NONE;

		return script_level;
	}

	if (attribute->value.sign == LSM_MATHML_SCRIPT_LEVEL_SIGN_NONE)
		return attribute->value.level;

	return script_level + attribute->value.level;
}

double
lsm_mathml_length_attribute_normalize (LsmMathmlLengthAttribute *attribute, double default_value, double font_size)
{
	g_return_val_if_fail (attribute != NULL, 0.0);

	attribute->value = lsm_mathml_length_normalize (&attribute->length, default_value, font_size);

	return attribute->value;
}

#include <lsmmathmlstyle.h>
#include <lsmdebug.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>
#include <glib-object.h>
#include <math.h>

/**************************/

typedef struct {
	ptrdiff_t bag_offset;
	const LsmMathmlAttributeBagClass *bag_class;
} LsmMathmlAttributeBagInfos;

typedef struct {
	ptrdiff_t attribute_offset;
	const LsmMathmlAttributeClass *attribute_class;
	LsmMathmlAttributeBagInfos *bag_infos;
} LsmMathmlAttributeInfos;

LsmMathmlAttributeMap *
lsm_mathml_attribute_map_new (void)
{
	LsmMathmlAttributeMap *map;

	map = g_new0 (LsmMathmlAttributeMap, 1);
	g_return_val_if_fail (map != NULL,  NULL);

	map->attribute_hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
	map->bag_hash = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, NULL);

	return map;
}

LsmMathmlAttributeMap *
lsm_mathml_attribute_map_duplicate (const LsmMathmlAttributeMap *from)
{
	LsmMathmlAttributeMap *map;
	GHashTableIter iter;
	void *key;
	void *value;

	map = lsm_mathml_attribute_map_new ();
	g_return_val_if_fail (map != NULL, NULL);

	g_hash_table_iter_init (&iter, from->attribute_hash);
	while (g_hash_table_iter_next (&iter, &key, &value))
		g_hash_table_insert (map->attribute_hash, key, value);

	g_hash_table_iter_init (&iter, from->bag_hash);
	while (g_hash_table_iter_next (&iter, &key, &value))
		g_hash_table_insert (map->bag_hash, key, value);

	return map;
}

void
lsm_mathml_attribute_map_free (LsmMathmlAttributeMap *map)
{
	g_return_if_fail (map != NULL);

	g_hash_table_unref (map->attribute_hash);
	g_hash_table_unref (map->bag_hash);
	g_free (map);
}

void
lsm_mathml_attribute_map_add_bag_attribute  (LsmMathmlAttributeMap *map,
					     const char *name,
					     ptrdiff_t attribute_offset,
					     const LsmMathmlAttributeClass *attribute_class,
					     ptrdiff_t bag_offset,
					     const LsmMathmlAttributeBagClass *bag_class)
{
	LsmMathmlAttributeInfos *attribute_infos;
	LsmMathmlAttributeBagInfos *bag_infos;

	g_return_if_fail (map != NULL);
	g_return_if_fail (name != NULL);
	g_return_if_fail (attribute_offset >= 0);

	if (g_hash_table_lookup (map->attribute_hash, name) != NULL) {
		g_warning ("[LsmMathmlAttributeMap::add_attribute] %s already defined", name);
		return;
	}

	attribute_infos = g_new (LsmMathmlAttributeInfos, 1);
	attribute_infos->attribute_offset = attribute_offset;
	attribute_infos->attribute_class = attribute_class;

	g_hash_table_insert (map->attribute_hash, (char *) name, attribute_infos);

	if (bag_class != NULL) {
		bag_infos = g_hash_table_lookup (map->bag_hash, bag_class);
		if (bag_infos == NULL) {
			bag_infos = g_new (LsmMathmlAttributeBagInfos, 1);
			bag_infos->bag_offset = bag_offset;
			bag_infos->bag_class = bag_class;

			g_hash_table_insert (map->bag_hash, (void *) bag_class, bag_infos);
		}
		attribute_infos->bag_infos = bag_infos;
	} else
		attribute_infos->bag_infos = NULL;

}

void
lsm_mathml_attribute_map_add_attribute_full (LsmMathmlAttributeMap *map,
					     const char *name,
					     ptrdiff_t offset,
					     const LsmMathmlAttributeClass *attribute_class)
{
	lsm_mathml_attribute_map_add_bag_attribute (map, name, offset, attribute_class, 0, NULL);
}

void
lsm_mathml_attribute_map_add_attribute_full_width_default (LsmMathmlAttributeMap *map,
							   const char *name,
							   ptrdiff_t offset,
							   const LsmMathmlAttributeClass *attribute_class)
{
	lsm_mathml_attribute_map_add_bag_attribute (map, name, offset, attribute_class, 0, NULL);
}

void
lsm_mathml_attribute_map_add_attribute (LsmMathmlAttributeMap *map,
					const char *name,
					ptrdiff_t offset)
{
	lsm_mathml_attribute_map_add_bag_attribute (map, name, offset, NULL, 0, NULL);
}

static LsmMathmlAttribute *
_get_attribute (LsmMathmlAttributeMap *map,
		void *instance,
		const char *name)
{
	LsmMathmlAttributeInfos *attribute_infos;
	LsmMathmlAttribute *attribute;

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL) {
		lsm_debug ("[LsmMathmlAttribute] Attribute not found (%s)", name);
		return NULL;
	}

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		LsmMathmlAttributeBag **bag_ptr;

		bag_ptr = (void *)(instance + attribute_infos->bag_infos->bag_offset);
		g_return_val_if_fail (bag_ptr != NULL, FALSE);

		if (*bag_ptr == NULL)
			*bag_ptr = attribute_infos->bag_infos->bag_class->init ();
		g_return_val_if_fail (*bag_ptr != NULL, FALSE);

		attribute = (((void *) *bag_ptr) + attribute_infos->attribute_offset);
	}

	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute;
}

gboolean
lsm_mathml_attribute_map_set_attribute (LsmMathmlAttributeMap *map,
					void *instance,
					const char *name,
					const char *value)
{
	LsmMathmlAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute = _get_attribute (map, instance, name);
	if (attribute == NULL)
		return FALSE;

	g_free (attribute->value);
	attribute->value = value != NULL ? g_strdup (value) : NULL;

	return TRUE;
}

gboolean
lsm_mathml_attribute_map_set_css_attribute (LsmMathmlAttributeMap *map,
					    void *instance,
					    const char *name,
					    const char *value,
					    LsmMathmlCssType type)
{
	LsmMathmlAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute = _get_attribute (map, instance, name);
	if (attribute == NULL)
		return FALSE;

	g_free (attribute->css_value);
	attribute->css_value = value != NULL ? g_strdup (value) : NULL;
	attribute->css_type = type;

	return TRUE;
}

char const *
lsm_mathml_attribute_map_get_attribute (LsmMathmlAttributeMap *map,
					void *instance,
					const char *name)
{
	LsmMathmlAttributeInfos *attribute_infos;
	LsmMathmlAttribute *attribute;

	g_return_val_if_fail (map != NULL, NULL);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return NULL;

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		LsmMathmlAttributeBag **bag_ptr;

		bag_ptr = (void *)(instance + attribute_infos->bag_infos->bag_offset);
		g_return_val_if_fail (bag_ptr != NULL, NULL);
		if (*bag_ptr == NULL)
			return NULL;
		attribute = (void *)(*bag_ptr + attribute_infos->attribute_offset);
	}
	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute->value;
}

gboolean
lsm_mathml_attribute_map_is_attribute_defined (LsmMathmlAttributeMap *map,
					       void *instance,
					       const char *name)
{
	LsmMathmlAttributeInfos *attribute_infos;
	LsmMathmlAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

gboolean
lsm_mathml_attribute_is_defined (const LsmMathmlAttribute *attribute)
{
	return (attribute != NULL && attribute->value != NULL);
}

static void
lsm_mathml_attribute_finalize_cb (gpointer key,
				  gpointer value,
				  gpointer instance)
{
	LsmMathmlAttributeInfos *attribute_infos = value;
	LsmMathmlAttribute *attribute;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	if (attribute != NULL) {
		g_free (attribute->value);
		g_free (attribute->css_value);

		attribute->value = NULL;
		attribute->css_value = NULL;

		if (attribute_infos->attribute_class != NULL &&
		    attribute_infos->attribute_class->finalize != NULL)
			attribute_infos->attribute_class->finalize (attribute);
	}
}

static void
lsm_mathml_attribute_bag_finalize_cb (gpointer key,
				      gpointer value,
				      gpointer instance)
{
	LsmMathmlAttributeBagInfos *bag_infos = value;
	LsmMathmlAttributeBag **bag_ptr;

	bag_ptr = (void *)(instance + bag_infos->bag_offset);
	if (*bag_ptr != NULL) {
		if (bag_infos->bag_class->finalize != NULL)
			bag_infos->bag_class->finalize (*bag_ptr);

		*bag_ptr = NULL;
	}
}

void
lsm_mathml_attribute_map_free_attributes (LsmMathmlAttributeMap *map, void *instance)
{
	g_return_if_fail (map != NULL);

	g_hash_table_foreach (map->attribute_hash, lsm_mathml_attribute_finalize_cb, instance);
	g_hash_table_foreach (map->bag_hash, lsm_mathml_attribute_bag_finalize_cb, instance);
}

char const *
lsm_mathml_attribute_get_value (const LsmMathmlAttribute *attribute)
{
	g_return_val_if_fail (attribute != NULL, NULL);

	if (attribute->css_value != NULL &&
	    attribute->css_type >= LSM_MATHML_CSS_TYPE_AUTHOR)
		return attribute->css_value;

	return attribute->value;
}

void
lsm_mathml_enum_attribute_parse (LsmMathmlEnumAttribute *attribute,
			      unsigned int *style_value,
			      LsmDomNamedConvert convert)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = convert (string);
	*style_value = attribute->value;
}

void
lsm_mathml_enum_list_attribute_parse (LsmMathmlEnumListAttribute *attribute,
				   LsmMathmlEnumList *style_value,
				   LsmDomNamedConvert convert)
{
	const char *string;
	char **items;
	unsigned int i;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		if (style_value->n_values > 0) {
			attribute->values = g_new (unsigned int, style_value->n_values);
			memcpy (attribute->values, style_value->values,
				sizeof (unsigned int) * style_value->n_values);
		} else
			attribute->values = NULL;
		attribute->n_values = style_value->n_values;

		return;
	}

	items = g_strsplit_set (string, " ", -1);
	attribute->n_values = g_strv_length (items);

	attribute->values = g_new (unsigned int, attribute->n_values);
	for (i = 0; i < attribute->n_values; i++)
		attribute->values[i] = convert (items[i]);

	g_strfreev (items);
}

void
lsm_mathml_enum_list_attribute_finalize (void *abstract)
{
	LsmMathmlEnumListAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;
	attribute->values = NULL;
}

static const LsmMathmlAttributeClass string_attribute_class = {
	.finalize = lsm_mathml_string_attribute_finalize
};

static const LsmMathmlAttributeClass enum_list_attribute_class = {
	.finalize = lsm_mathml_enum_list_attribute_finalize
};

void
lsm_mathml_attribute_map_add_enum_list (LsmMathmlAttributeMap *map,
					char const *name,
					ptrdiff_t offset)
{
	lsm_mathml_attribute_map_add_attribute_full (map, name, offset, &enum_list_attribute_class);
}

void
lsm_mathml_space_attribute_parse (LsmMathmlSpaceAttribute *attribute,
				  LsmMathmlSpace *style_value,
				  LsmMathmlStyle *style)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->space = *style_value;
	} else {
		attribute->space.name = lsm_mathml_space_name_from_string (string);
		if (attribute->space.name == LSM_MATHML_SPACE_NAME_ERROR) {
			LsmMathmlUnit unit;
			char *unit_str;
			double value;

			value = g_strtod (string, &unit_str);
			unit = lsm_mathml_unit_from_string (unit_str);

			if (unit == LSM_MATHML_UNIT_NONE) {
				unit = style_value->length.unit;
				value *= style_value->length.value;
			} else if (unit == LSM_MATHML_UNIT_PERCENT) {
				unit = style_value->length.unit;
				value *= style_value->length.value / 100.0;
			}

			attribute->space.length.unit = unit;
			attribute->space.length.value = value;
		} else {
			attribute->space.length.value = 0.0;
			attribute->space.length.unit = LSM_MATHML_UNIT_PX;
		}

		*style_value = attribute->space;
	}

	switch (attribute->space.name) {
		case LSM_MATHML_SPACE_NAME_VERY_VERY_THIN:
			attribute->value = style->very_very_thin_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_VERY_THIN:
			attribute->value = style->very_thin_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_THIN:
			attribute->value = style->thin_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_MEDIUM:
			attribute->value = style->medium_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_THICK:
			attribute->value = style->thick_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_VERY_THICK:
			attribute->value = style->very_thick_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_VERY_VERY_THICK:
			attribute->value = style->very_very_thick_math_space;
			break;
		case LSM_MATHML_SPACE_NAME_INFINITY:
			attribute->value = G_MAXDOUBLE;
			break;
		case LSM_MATHML_SPACE_NAME_ERROR:
		default:
			attribute->value = lsm_mathml_length_normalize (&attribute->space.length,
									style_value->length.value,
									style->math_size);
	}
}

static void
lsm_mathml_space_list_attribute_finalize (void *abstract)
{
	LsmMathmlSpaceListAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	lsm_mathml_space_list_free (attribute->space_list);
	g_free (attribute->values);
	attribute->space_list = NULL;
	attribute->values = NULL;
}

void
lsm_mathml_space_list_attribute_parse (LsmMathmlSpaceListAttribute *attribute,
				       LsmMathmlSpaceList *style_value,
				       const LsmMathmlStyle *style)
{
	unsigned int i;
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	lsm_mathml_space_list_attribute_finalize (attribute);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->space_list = lsm_mathml_space_list_duplicate (style_value);
	} else {
		unsigned int n_items;
		char *unit_str;
		char **items;

		items = g_strsplit_set (string, " ", -1);
		n_items = g_strv_length (items);

		attribute->space_list = lsm_mathml_space_list_new (n_items);
		for (i = 0; i < n_items; i++) {
			LsmMathmlSpace *space;

			space = &attribute->space_list->spaces[i];

			space->name = lsm_mathml_space_name_from_string (items[i]);
			if (space->name == LSM_MATHML_SPACE_NAME_ERROR) {
				LsmMathmlUnit unit;
				double value;

				value = g_strtod (items[i], &unit_str);
				unit = lsm_mathml_unit_from_string (unit_str);

				if (style_value->n_spaces > 0) {
					unsigned int index;

					index = MIN (i, style_value->n_spaces - 1);

					if (unit == LSM_MATHML_UNIT_NONE) {
						unit = style_value->spaces[index].length.unit;
						value *= style_value->spaces[index].length.value;
					} else if (unit == LSM_MATHML_UNIT_PERCENT) {
						unit = style_value->spaces[index].length.unit;
						value *= style_value->spaces[index].length.value / 100.0;
					}
				} else {
					unit = LSM_MATHML_UNIT_PX;
					value = 0.0;
				}

				space->length.unit = unit;
				space->length.value = value;
			} else {
				space->length.value = 0.0;
				space->length.unit = LSM_MATHML_UNIT_PX;
			}

			/* FIXME copy the new values to style */
			if (i < style_value->n_spaces)
				style_value->spaces[i] = *space;
		}
		g_strfreev (items);
	}

	attribute->values = g_new (double, attribute->space_list->n_spaces);

	for (i = 0; i < attribute->space_list->n_spaces; i++) {
		switch (attribute->space_list->spaces[i].name) {
			case LSM_MATHML_SPACE_NAME_VERY_VERY_THIN:
				attribute->values[i] = style->very_very_thin_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_VERY_THIN:
				attribute->values[i] = style->very_thin_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_THIN:
				attribute->values[i] = style->thin_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_MEDIUM:
				attribute->values[i] = style->medium_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_THICK:
				attribute->values[i] = style->thick_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_VERY_THICK:
				attribute->values[i] = style->very_thick_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_VERY_VERY_THICK:
				attribute->values[i] = style->very_very_thick_math_space;
				break;
			case LSM_MATHML_SPACE_NAME_ERROR:
			default:
				if (style_value->n_spaces > 0) {
					unsigned int index;

					index = MIN (i, style_value->n_spaces - 1);

					attribute->values[i] = lsm_mathml_length_normalize
						(&attribute->space_list->spaces[i].length,
						 style_value->spaces[index].length.value,
						 style->math_size);
				} else
					attribute->values[i] = lsm_mathml_length_normalize
						(&attribute->space_list->spaces[i].length,
						 0.0,
						 style->math_size);
		}
	}
}

void
lsm_mathml_row_align_list_attribute_parse (LsmMathmlEnumListAttribute *attribute,
					   LsmMathmlEnumList *style_value)
{
	lsm_mathml_enum_list_attribute_parse (attribute, style_value, lsm_mathml_row_align_from_string);
}

void
lsm_mathml_column_align_list_attribute_parse (LsmMathmlEnumListAttribute *attribute,
					      LsmMathmlEnumList *style_value)
{
	lsm_mathml_enum_list_attribute_parse (attribute, style_value, lsm_mathml_column_align_from_string);
}

void
lsm_mathml_line_list_attribute_parse (LsmMathmlEnumListAttribute *attribute,
				      LsmMathmlEnumList *style_value)
{
	lsm_mathml_enum_list_attribute_parse (attribute, style_value, lsm_mathml_line_from_string);
}

static const LsmMathmlAttributeClass space_list_attribute_class = {
	.finalize = lsm_mathml_space_list_attribute_finalize
};

void
lsm_mathml_attribute_map_add_space_list (LsmMathmlAttributeMap *map,
					 char const *name,
					 ptrdiff_t offset)
{
	lsm_mathml_attribute_map_add_attribute_full (map, name, offset, &space_list_attribute_class);
}

