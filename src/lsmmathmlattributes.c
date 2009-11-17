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
#include <lsmmathmlstyle.h>
#include <lsmdebug.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>
#include <glib-object.h>
#include <math.h>

gboolean
lsm_mathml_boolean_attribute_inherit (LsmMathmlBooleanAttribute *attribute, gboolean value)
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

#if 0
void
lsm_mathml_boolean_attribute_parse (LsmMathmlBooleanAttribute *attribute,
				 gboolean *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = (strcmp (string, "true") == 0);
	*style_value = attribute->value;
}

void
lsm_mathml_unsigned_attribute_parse (LsmMathmlUnsignedAttribute *attribute,
				  unsigned int *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atoi (string);
	*style_value = attribute->value;
}

void
lsm_mathml_double_attribute_parse (LsmMathmlDoubleAttribute *attribute,
				double *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atof (string);
	*style_value = attribute->value;
}

#endif

void
lsm_mathml_string_attribute_parse (LsmMathmlStringAttribute *attribute,
				char **style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);
	g_return_if_fail (*style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		g_free (attribute->value);
		attribute->value = g_strdup (*style_value);
	} else {
		g_free (*style_value);
		*style_value = g_strdup (string);
		g_free (attribute->value);
		attribute->value = g_strdup (string);
	}
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
lsm_mathml_string_attribute_finalize (void *abstract)
{
	LsmMathmlStringAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->value);
	attribute->value = NULL;
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

void
lsm_mathml_attribute_map_add_string (LsmMathmlAttributeMap *map,
				     char const *name,
				     ptrdiff_t offset)
{
	lsm_mathml_attribute_map_add_attribute_full (map, name, offset, &string_attribute_class);
}

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

static LsmMathmlColor *
lsm_mathml_color_copy (LsmMathmlColor *color)
{
	LsmMathmlColor *copy;

	copy = g_new (LsmMathmlColor, 1);
	memcpy (copy, color, sizeof (LsmMathmlColor));

	return copy;
}

GType
lsm_mathml_color_get_type (void)
{
	static GType our_type = 0;
	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlColor",
			 (GBoxedCopyFunc) lsm_mathml_color_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

static LsmMathmlLength *
lsm_mathml_length_copy (LsmMathmlLength *length)
{
	LsmMathmlLength *copy;

	copy = g_new (LsmMathmlLength, 1);
	memcpy (copy, length, sizeof (LsmMathmlLength));

	return copy;
}

GType
lsm_mathml_length_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlLength",
			 (GBoxedCopyFunc) lsm_mathml_length_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

static LsmMathmlSpace *
lsm_mathml_space_copy (LsmMathmlSpace *space)
{
	LsmMathmlSpace *copy;

	copy = g_new (LsmMathmlSpace, 1);
	memcpy (copy, space, sizeof (LsmMathmlSpace));

	return copy;
}

GType
lsm_mathml_space_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlSpace",
			 (GBoxedCopyFunc) lsm_mathml_space_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

GType
lsm_mathml_space_list_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlSpaceList",
			 (GBoxedCopyFunc) lsm_mathml_space_list_duplicate,
			 (GBoxedFreeFunc) lsm_mathml_space_list_free);
	return our_type;
}

double
lsm_mathml_length_compute (const LsmMathmlLength *length, double default_value, double font_size)
{
	g_return_val_if_fail (length != NULL, 0.0);

	switch (length->unit) {
		case LSM_MATHML_UNIT_PX:
		case LSM_MATHML_UNIT_PT:
			return length->value;
		case LSM_MATHML_UNIT_PC:
			return length->value * 72.0 / 6.0;
		case LSM_MATHML_UNIT_CM:
			return length->value * 72.0 / 2.54;
		case LSM_MATHML_UNIT_MM:
			return length->value * 72.0 / 25.4;
		case LSM_MATHML_UNIT_IN:
			return length->value * 72.0;
		case LSM_MATHML_UNIT_EM:
			return length->value * font_size;
		case LSM_MATHML_UNIT_EX:
			return length->value * font_size * 0.5;
		case LSM_MATHML_UNIT_PERCENT:
			return default_value * length->value / 100.0;
		case LSM_MATHML_UNIT_NONE:
			return default_value * length->value;
	}

	return 0.0;
}


void
lsm_mathml_script_level_attribute_parse (LsmMathmlScriptLevelAttribute *attribute,
					 int *style_value)
{
	const char *string;
	int value;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	value = atoi (string);
	if (string[0] == '+')
		attribute->value = value + *style_value;
	else if (string[0] == '-')
		attribute->value = value - *style_value;
	else
		attribute->value = value;
	*style_value = attribute->value;
}

void
lsm_mathml_color_attribute_parse (LsmMathmlColorAttribute *attribute,
				  LsmMathmlColor *style_color)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_color != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->color.red = style_color->red;
		attribute->color.green = style_color->green;
		attribute->color.blue = style_color->blue;
		attribute->color.alpha = style_color->alpha;
		return;
	}

	if (strcmp (string, "transparent") == 0) {
		attribute->color.red = 0.0;
		attribute->color.green = 0.0;
		attribute->color.blue = 0.0;
		attribute->color.alpha = 0.0;
	} else {
		PangoColor color;

		pango_color_parse (&color, string);
		attribute->color.alpha = 1.0;
		attribute->color.red = color.red / 65535.0;
		attribute->color.green = color.green / 65535.0;
		attribute->color.blue = color.blue / 65535.0;
	}
	*style_color = attribute->color;
}

void
lsm_mathml_mode_attribute_parse (LsmMathmlEnumAttribute *attribute,
				 unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_mode_from_string);
}

void
lsm_mathml_display_attribute_parse (LsmMathmlEnumAttribute *attribute,
				    unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_display_from_string);
}

void
lsm_mathml_form_attribute_parse (LsmMathmlEnumAttribute *attribute,
				 unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_form_from_string);
}

void
lsm_mathml_font_style_attribute_parse (LsmMathmlEnumAttribute *attribute,
				       unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_font_style_from_string);
}

void
lsm_mathml_font_weight_attribute_parse (LsmMathmlEnumAttribute *attribute,
					unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_font_weight_from_string);
}

void
lsm_mathml_variant_attribute_parse (LsmMathmlEnumAttribute *attribute,
				    unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_variant_from_string);
}

void
lsm_mathml_line_attribute_parse (LsmMathmlEnumAttribute *attribute,
				 unsigned int *style_value)
{
	return lsm_mathml_enum_attribute_parse (attribute, style_value, lsm_mathml_line_from_string);
}

void
lsm_mathml_length_attribute_parse (LsmMathmlLengthAttribute *attribute,
				   LsmMathmlLength *style_value,
				   double font_size)
{
	const char *string;
	char *unit_str;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_mathml_attribute_get_value ((LsmMathmlAttribute *) attribute);
	if (string == NULL) {
		attribute->length = *style_value;
	} else {
		LsmMathmlUnit unit;
		double value;

		value = g_strtod (string, &unit_str);
		unit = lsm_mathml_unit_from_string (unit_str);

		if (unit == LSM_MATHML_UNIT_NONE) {
			unit = style_value->unit;
			if (unit_str[0] != '\0') {
				if (strcmp (unit_str, "big") == 0) {
					value = 1.5;
				} else if (strcmp (unit_str, "small") == 0) {
					value = 0.75;
				} else if (strcmp (unit_str, "normal") == 0) {
					value = 1.0;
				}
			}
			value *= style_value->value;
		} else if (unit == LSM_MATHML_UNIT_PERCENT) {
			unit = style_value->unit;
			value *= style_value->value / 100.0;
		}

		attribute->length.unit = unit;
		attribute->length.value = value;

		*style_value = attribute->length;
	}

	attribute->value = lsm_mathml_length_compute (&attribute->length, style_value->value, font_size);
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
			attribute->value = lsm_mathml_length_compute (&style->very_very_thin_math_space,
								   style->very_very_thin_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_VERY_THIN:
			attribute->value = lsm_mathml_length_compute (&style->very_thin_math_space,
								   style->very_thin_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_THIN:
			attribute->value = lsm_mathml_length_compute (&style->thin_math_space,
								   style->thin_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_MEDIUM:
			attribute->value = lsm_mathml_length_compute (&style->medium_math_space,
								   style->medium_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_THICK:
			attribute->value = lsm_mathml_length_compute (&style->thick_math_space,
								   style->thick_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_VERY_THICK:
			attribute->value = lsm_mathml_length_compute (&style->very_thick_math_space,
								   style->very_thick_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_VERY_VERY_THICK:
			attribute->value = lsm_mathml_length_compute (&style->very_very_thick_math_space,
								   style->very_very_thick_math_space_value,
								   style->math_size_value);
			break;
		case LSM_MATHML_SPACE_NAME_INFINITY:
			attribute->value = G_MAXDOUBLE;
			break;
		case LSM_MATHML_SPACE_NAME_ERROR:
		default:
			attribute->value = lsm_mathml_length_compute (&attribute->space.length,
								   style_value->length.value,
								   style->math_size_value);
	}
}

LsmMathmlSpaceList *
lsm_mathml_space_list_new (unsigned int n_spaces)
{
	LsmMathmlSpaceList *space_list;

	space_list = g_new (LsmMathmlSpaceList, 1);
	if (space_list == NULL)
		return NULL;

	space_list->n_spaces = n_spaces;

	if (n_spaces > 0) {
		space_list->spaces = g_new (LsmMathmlSpace, n_spaces);

		if (space_list->spaces == NULL) {
			g_free (space_list);
			return NULL;
		}
	} else
		space_list->spaces = NULL;

	return space_list;
}

void
lsm_mathml_space_list_free (LsmMathmlSpaceList *space_list)
{
	if (space_list == NULL)
		return;

	space_list->n_spaces = 0;

	g_free (space_list->spaces);
	g_free (space_list);
}

LsmMathmlSpaceList *
lsm_mathml_space_list_duplicate (const LsmMathmlSpaceList *space_list)
{
	LsmMathmlSpaceList *new_space_list;

	g_return_val_if_fail (space_list != NULL, NULL);

	new_space_list = lsm_mathml_space_list_new (space_list->n_spaces);
	memcpy (new_space_list->spaces, space_list->spaces,
		sizeof (LsmMathmlSpace) * space_list->n_spaces);

	return new_space_list;
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
				attribute->values[i] = lsm_mathml_length_compute (&style->very_very_thin_math_space,
									       style->very_very_thin_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_VERY_THIN:
				attribute->values[i] = lsm_mathml_length_compute (&style->very_thin_math_space,
									       style->very_thin_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_THIN:
				attribute->values[i] = lsm_mathml_length_compute (&style->thin_math_space,
									       style->thin_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_MEDIUM:
				attribute->values[i] = lsm_mathml_length_compute (&style->medium_math_space,
									       style->medium_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_THICK:
				attribute->values[i] = lsm_mathml_length_compute (&style->thick_math_space,
									       style->thick_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_VERY_THICK:
				attribute->values[i] = lsm_mathml_length_compute (&style->very_thick_math_space,
									       style->very_thick_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_VERY_VERY_THICK:
				attribute->values[i] = lsm_mathml_length_compute (&style->very_very_thick_math_space,
									       style->very_very_thick_math_space_value,
									       style->math_size_value);
				break;
			case LSM_MATHML_SPACE_NAME_ERROR:
			default:
				if (style_value->n_spaces > 0) {
					unsigned int index;

					index = MIN (i, style_value->n_spaces - 1);

					attribute->values[i] = lsm_mathml_length_compute
						(&attribute->space_list->spaces[i].length,
						 style_value->spaces[index].length.value,
						 style->math_size_value);
				} else
					attribute->values[i] = lsm_mathml_length_compute
						(&attribute->space_list->spaces[i].length,
						 0.0,
						 style->math_size_value);
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
