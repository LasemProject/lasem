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
#include <stdio.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>
#include <math.h>

double
gmathml_length_compute (GMathmlLength *length, double default_value, double font_size)
{
	switch (length->unit) {
		case GMATHML_UNIT_PX:
		case GMATHML_UNIT_PT:
			return length->value;
		case GMATHML_UNIT_PC:
			return length->value * 72.0 / 6.0;
		case GMATHML_UNIT_CM:
			return length->value * 72.0 / 2.54;
		case GMATHML_UNIT_MM:
			return length->value * 72.0 / 25.4;
		case GMATHML_UNIT_IN:
			return length->value * 72.0;
		case GMATHML_UNIT_EM:
			return length->value * font_size;
		case GMATHML_UNIT_EX:
			return length->value * font_size * 0.5;
		case GMATHML_UNIT_PERCENT:
			return default_value * length->value / 100.0;
		case GMATHML_UNIT_NONE:
			return default_value * length->value;
	}

	return 0.0;
}

GMathmlStyle *
gmathml_style_new (void)
{
	GMathmlStyle *style = g_new0 (GMathmlStyle, 1);

	return style;
}

void
gmathml_style_free (GMathmlStyle *style)
{
	g_return_if_fail (style != NULL);

	g_free (style);
}

GMathmlStyle *
gmathml_style_duplicate (const GMathmlStyle *from)
{
	GMathmlStyle *style;

	g_return_val_if_fail (from != NULL, NULL);

	style = g_new (GMathmlStyle, 1);
	g_return_val_if_fail (style != NULL, NULL);

	memcpy (style, from, sizeof (GMathmlStyle));

	return style;
}

void
gmathml_style_change_script_level (GMathmlStyle *style, int increment)
{
	style->font_size = style->font_size * pow (style->script_size_multiplier, increment);
	style->script_level += increment;

	if (style->font_size < style->script_min_size.value)
		style->font_size = style->script_min_size.value;

	style->math_size.value = style->font_size;
	style->math_size.unit = GMATHML_UNIT_PT;
}

void
gmathml_style_dump (const GMathmlStyle *style)
{
	printf ("font_size =              %g\n", style->font_size);
	printf ("script_level =           %d (%s)\n", style->script_level, style->display_style ? "TRUE" : "FALSE");
	printf ("script_size_multiplier = %g\n", style->script_size_multiplier);
}

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
				 gboolean *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	g_message ("boolean = -%s-", string);
	attribute->value = (strcmp (string, "true") == 0);
	*style_value = attribute->value;
}

void
gmathml_attribute_double_parse (GMathmlAttributeDouble *attribute,
				double *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atof (string);
	*style_value = attribute->value;
}

void
gmathml_attribute_script_level_parse (GMathmlAttributeScriptLevel *attribute,
				      int *style_value)
{
	const char *string;
	int value;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
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
gmathml_attribute_color_parse (GMathmlAttributeColor *attribute,
			       GMathmlColor *style_color)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_color != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
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
gmathml_attribute_mode_parse (GMathmlAttributeMode *attribute,
			      GMathmlMode *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = gmathml_mode_from_string (string);
	*style_value = attribute->value;
}

void
gmathml_attribute_variant_parse (GMathmlAttributeLength *attribute,
				 GMathmlVariant *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = gmathml_variant_from_string (string);
	*style_value = attribute->value;
}

void
gmathml_attribute_length_parse (GMathmlAttributeLength *attribute,
				GMathmlLength *style_value,
				double font_size)
{
	const char *string;
	char *unit_str;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->length = *style_value;
	} else {
		attribute->length.value = g_strtod (string, &unit_str);
		attribute->length.unit = gmathml_unit_from_string (unit_str);
		*style_value = attribute->length;
	}

	attribute->value = gmathml_length_compute (&attribute->length, style_value->value, font_size);
}

void
gmathml_attribute_space_parse (GMathmlAttributeSpace *attribute,
			       GMathmlSpace *style_value,
			       GMathmlStyle *style)
{
	const char *string;
	char *unit_str;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->space = *style_value;
	} else {
		attribute->space.name = gmathml_space_name_from_string (string);
		if (attribute->space.name == GMATHML_SPACE_NAME_ERROR) {
			attribute->space.length.value = g_strtod (string, &unit_str);
			attribute->space.length.unit = gmathml_unit_from_string (unit_str);
		} else {
			attribute->space.length.value = 0.0;
			attribute->space.length.unit = GMATHML_UNIT_PX;
		}
		*style_value = attribute->space;
	}

	switch (attribute->space.name) {
		case GMATHML_SPACE_NAME_VERY_VERY_THIN:
			attribute->value = gmathml_length_compute (&style->very_very_thin_math_space,
								   style->very_very_thin_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_VERY_THIN:
			attribute->value = gmathml_length_compute (&style->very_thin_math_space,
								   style->very_thin_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_THIN:
			attribute->value = gmathml_length_compute (&style->thin_math_space,
								   style->thin_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_MEDIUM:
			attribute->value = gmathml_length_compute (&style->medium_math_space,
								   style->medium_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_THICK:
			attribute->value = gmathml_length_compute (&style->thick_math_space,
								   style->thick_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_VERY_THICK:
			attribute->value = gmathml_length_compute (&style->very_thick_math_space,
								   style->very_thick_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_VERY_VERY_THICK:
			attribute->value = gmathml_length_compute (&style->very_very_thick_math_space,
								   style->very_very_thick_math_space_value,
								   style->font_size);
			break;
		case GMATHML_SPACE_NAME_ERROR:
		default:
			attribute->value = gmathml_length_compute (&attribute->space.length,
								   style_value->length.value,
								   style->font_size);
	}
}
