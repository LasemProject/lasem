/* gmathmlattributes.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmlattributes.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>

typedef struct {
	ptrdiff_t attr_offset;
} GMathmlAttributeInfos;

GMathmlAttributeMap *
gmathml_attribute_map_new (void)
{
	GMathmlAttributeMap *map;

	map = g_new0 (GMathmlAttributeMap, 1);
	g_return_val_if_fail (map != NULL,  NULL);

	map->hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);

	return map;
}

void
gmathml_attribute_map_free (GMathmlAttributeMap *map)
{
	g_return_if_fail (map != NULL);

	g_hash_table_unref (map->hash);
	g_free (map);
}

void
gmathml_attribute_map_add_attribute (GMathmlAttributeMap *map,
				     const char *attr_name,
				     ptrdiff_t attr_offset)
{
	GMathmlAttributeInfos *attr_infos;

	g_return_if_fail (map != NULL);
	g_return_if_fail (attr_name != NULL);
	g_return_if_fail (attr_offset >= 0);

	if (g_hash_table_lookup (map->hash, attr_name) != NULL) {
		g_warning ("[GMathmlAttributes::add_attribute] %s already defined", attr_name);
		return;
	}

	attr_infos = g_new (GMathmlAttributeInfos, 1);
	attr_infos->attr_offset = attr_offset;

	g_hash_table_insert (map->hash, (char *) attr_name, attr_infos);
}

gboolean
gmathml_attribute_map_set_attribute (GMathmlAttributeMap *map,
				     void *instance,
				     const char *attr_name,
				     const char *attr_value)
{
	GMathmlAttributeInfos *attr_infos;
	GMathmlAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	g_free (attribute->value);
	attribute->value = attr_value != NULL ? g_strdup (attr_value) : NULL;

	return TRUE;
}

char const *
gmathml_attribute_map_get_attribute (GMathmlAttributeMap *map,
				     void *instance,
				     const char *attr_name)
{
	GMathmlAttributeInfos *attr_infos;
	GMathmlAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, NULL);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return NULL;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute->value;
}

gboolean
gmathml_attribute_map_is_attribute_defined (GMathmlAttributeMap *map,
					    void *instance,
					    const char *attr_name)
{
	GMathmlAttributeInfos *attr_infos;
	GMathmlAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

static void
gmathml_attribute_finalize_cb (gpointer key,
			       gpointer value,
			       gpointer instance)
{
	GMathmlAttributeInfos *attr_infos = value;
	GMathmlAttributeValue *attribute;

	attribute = (void *)(instance + attr_infos->attr_offset);
	if (attribute != NULL) {
		g_free (attribute->value);
		g_free (attribute->css_value);
	}
}

void
gmathml_attribute_map_free_attributes (GMathmlAttributeMap *map, void *instance)
{
	g_return_if_fail (map != NULL);

	g_hash_table_foreach (map->hash, gmathml_attribute_finalize_cb, instance);
}

char const *
gmathml_attribute_value_get_actual_value (const GMathmlAttributeValue *attribute)
{
	g_return_val_if_fail (attribute != NULL, NULL);

	if (attribute->css_value != NULL &&
	    attribute->css_type >= GMATHML_CSS_TYPE_AUTHOR)
		return attribute->css_value;

	return attribute->value;
}

void
gmathml_attribute_boolean_parse (GMathmlAttributeBoolean *attribute,
				 gboolean default_value)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = default_value;
		return;
	}

	attribute->value = (strcmp (string, "true") == 0);
}

void
gmathml_attribute_double_parse (GMathmlAttributeDouble *attribute,
				double default_value)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = default_value;
		return;
	}

	attribute->value = atof (string);
}

void
gmathml_attribute_script_level_parse (GMathmlAttributeScriptLevel *attribute,
				      int default_value)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);

	if (string == NULL)
		attribute->value = default_value;
	else {
		int value;

		value = atoi (string);
		if (string[0] == '+')
			attribute->value = value + default_value;
		else if (string[0] == '-')
			attribute->value = value - default_value;
		else
			attribute->value = value;
	}
}

void
gmathml_attribute_color_parse (GMathmlAttributeColor *attribute,
			       double red,
			       double green,
			       double blue,
			       double alpha)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);

	if (string == NULL) {
		attribute->red = red;
		attribute->green = green;
		attribute->blue = blue;
		attribute->alpha = alpha;
	} else {
		if (strcmp (string, "transparent") == 0) {
			attribute->red = 0.0;
			attribute->green = 0.0;
			attribute->blue = 0.0;
			attribute->alpha = 0.0;
		} else {
			PangoColor color;

			pango_color_parse (&color, string);
			attribute->alpha = 1.0;
			attribute->red = color.red / 65535.0;
			attribute->green = color.green / 65535.0;
			attribute->blue = color.blue / 65535.0;
		}
	}
}

void gmathml_attribute_variant_parse (GMathmlAttributeLength *attribute,
				      GMathmlVariant variant)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);

	if (string == NULL)
		attribute->value = variant;
	else
		attribute->value = gmathml_variant_from_string (string);
}

void gmathml_attribute_length_parse (GMathmlAttributeLength *attribute,
				     double value, GMathmlUnit unit)
{
	const char *string;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);

	if (string == NULL) {
		attribute->value = value;
		attribute->unit = unit;
	} else {
		char *unit_str;

		attribute->value = g_strtod (string, &unit_str);
		attribute->unit = gmathml_unit_from_string (unit_str);
		if (attribute->unit == GMATHML_UNIT_NONE)
			attribute->value *= 100.0;
	}
}
