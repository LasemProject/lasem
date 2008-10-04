/* gmathmlattributes.c
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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
gmathml_length_compute (const GMathmlLength *length, double default_value, double font_size)
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

	g_free (style->math_family);
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

	style->math_family = g_strdup (from->math_family);

	return style;
}

void
gmathml_style_change_script_level (GMathmlStyle *style, int increment)
{
	style->math_size_value = style->math_size_value * pow (style->script_size_multiplier, increment);
	style->script_level += increment;

	if (style->math_size_value < style->script_min_size.value)
		style->math_size_value = style->script_min_size.value;

	style->math_size.value = style->math_size_value;
	style->math_size.unit = GMATHML_UNIT_PT;
}

void
gmathml_style_dump (const GMathmlStyle *style)
{
	printf ("math_size =              %g\n", style->math_size_value);
	printf ("script_level =           %d (%s)\n", style->script_level, style->display_style ? "TRUE" : "FALSE");
	printf ("script_size_multiplier = %g\n", style->script_size_multiplier);
}

typedef struct {
	ptrdiff_t attr_offset;
	void (*finalize) (void *);
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
gmathml_attribute_map_add_attribute_full (GMathmlAttributeMap *map,
					  const char *attr_name,
					  ptrdiff_t attr_offset,
					  GMathmlAttributeFinalizeFunc finalize)
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
	attr_infos->finalize = finalize;

	g_hash_table_insert (map->hash, (char *) attr_name, attr_infos);
}

void
gmathml_attribute_map_add_attribute (GMathmlAttributeMap *map,
				     const char *attr_name,
				     ptrdiff_t attr_offset)
{
	gmathml_attribute_map_add_attribute_full (map, attr_name, attr_offset, NULL);
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

		if (attr_infos->finalize != NULL)
			attr_infos->finalize (attribute);
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

static void
gmathml_attribute_named_parse (GMathmlAttributeNamed *attribute,
			       unsigned int *style_value,
			       GMathmlNamedConvert convert)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = convert (string);
	*style_value = attribute->value;
}

void
gmathml_attribute_mode_parse (GMathmlAttributeNamed *attribute,
			      unsigned int *style_value)
{
	return gmathml_attribute_named_parse (attribute, style_value, gmathml_mode_from_string);
}

void
gmathml_attribute_form_parse (GMathmlAttributeNamed *attribute,
			      unsigned int *style_value)
{
	return gmathml_attribute_named_parse (attribute, style_value, gmathml_form_from_string);
}

void
gmathml_attribute_variant_parse (GMathmlAttributeNamed *attribute,
				 unsigned int *style_value)
{
	return gmathml_attribute_named_parse (attribute, style_value, gmathml_variant_from_string);
}

void
gmathml_attribute_line_parse (GMathmlAttributeNamed *attribute,
			      unsigned int *style_value)
{
	return gmathml_attribute_named_parse (attribute, style_value, gmathml_line_from_string);
}

void
gmathml_attribute_string_parse (GMathmlAttributeString *attribute,
				char **style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);
	g_return_if_fail (*style_value != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		g_free (attribute->value);
		attribute->value = g_strdup (*style_value);
	} else {
		g_free (*style_value);
		*style_value = g_strdup (string);
	}
}

void
gmathml_attribute_string_finalize (void *abstract)
{
	GMathmlAttributeString *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->value);
	attribute->value = NULL;
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
		GMathmlUnit unit;
		double value;

		value = g_strtod (string, &unit_str);
		unit = gmathml_unit_from_string (unit_str);

		if (unit == GMATHML_UNIT_NONE) {
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
		} else if (unit == GMATHML_UNIT_PERCENT) {
			unit = style_value->unit;
			value *= style_value->value / 100.0;
		}

		attribute->length.unit = unit;
		attribute->length.value = value;

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

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style != NULL);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->space = *style_value;
	} else {
		attribute->space.name = gmathml_space_name_from_string (string);
		if (attribute->space.name == GMATHML_SPACE_NAME_ERROR) {
			GMathmlUnit unit;
			char *unit_str;
			double value;

			value = g_strtod (string, &unit_str);
			unit = gmathml_unit_from_string (unit_str);

			if (unit == GMATHML_UNIT_NONE) {
				unit = style_value->length.unit;
				value *= style_value->length.value;
			} else if (unit == GMATHML_UNIT_PERCENT) {
				unit = style_value->length.unit;
				value *= style_value->length.value / 100.0;
			}

			attribute->space.length.unit = unit;
			attribute->space.length.value = value;
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
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_VERY_THIN:
			attribute->value = gmathml_length_compute (&style->very_thin_math_space,
								   style->very_thin_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_THIN:
			attribute->value = gmathml_length_compute (&style->thin_math_space,
								   style->thin_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_MEDIUM:
			attribute->value = gmathml_length_compute (&style->medium_math_space,
								   style->medium_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_THICK:
			attribute->value = gmathml_length_compute (&style->thick_math_space,
								   style->thick_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_VERY_THICK:
			attribute->value = gmathml_length_compute (&style->very_thick_math_space,
								   style->very_thick_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_VERY_VERY_THICK:
			attribute->value = gmathml_length_compute (&style->very_very_thick_math_space,
								   style->very_very_thick_math_space_value,
								   style->math_size_value);
			break;
		case GMATHML_SPACE_NAME_ERROR:
		default:
			attribute->value = gmathml_length_compute (&attribute->space.length,
								   style_value->length.value,
								   style->math_size_value);
	}
}

GMathmlSpaceList *
gmathml_space_list_new (unsigned int n_spaces)
{
	GMathmlSpaceList *space_list;

	space_list = g_new (GMathmlSpaceList, 1);
	if (space_list == NULL)
		return NULL;

	space_list->n_spaces = n_spaces;

	if (n_spaces > 0) {
		space_list->spaces = g_new (GMathmlSpace, n_spaces);

		if (space_list->spaces == NULL) {
			g_free (space_list);
			return NULL;
		}
	} else
		space_list->spaces = NULL;

	return space_list;
}

void
gmathml_space_list_free (GMathmlSpaceList *space_list)
{
	if (space_list == NULL)
		return;

	space_list->n_spaces = 0;

	g_free (space_list->spaces);
	g_free (space_list);
}

GMathmlSpaceList *
gmathml_space_list_duplicate (const GMathmlSpaceList *space_list)
{
	GMathmlSpaceList *new_space_list;

	g_return_val_if_fail (space_list != NULL, NULL);

	new_space_list = gmathml_space_list_new (space_list->n_spaces);
	memcpy (new_space_list->spaces, space_list->spaces,
		sizeof (GMathmlSpace) * space_list->n_spaces);

	return new_space_list;
}

void
gmathml_attribute_space_list_finalize (void *abstract)
{
	GMathmlAttributeSpaceList *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	gmathml_space_list_free (attribute->space_list);
	g_free (attribute->values);
	attribute->space_list = NULL;
	attribute->values = NULL;
}

void
gmathml_attribute_space_list_parse (GMathmlAttributeSpaceList *attribute,
				    GMathmlSpaceList *style_value,
				    const GMathmlStyle *style)
{
	unsigned int i;
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	gmathml_attribute_space_list_finalize (attribute);

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
	if (string == NULL) {
		attribute->space_list = gmathml_space_list_duplicate (style_value);
	} else {
		unsigned int n_items;
		char *unit_str;
		char **items;

		items = g_strsplit_set (string, " ", -1);
		n_items = g_strv_length (items);

		attribute->space_list = gmathml_space_list_new (n_items);
		for (i = 0; i < n_items; i++) {
			GMathmlSpace *space;

			space = &attribute->space_list->spaces[i];

			space->name = gmathml_space_name_from_string (items[i]);
			if (space->name == GMATHML_SPACE_NAME_ERROR) {
				GMathmlUnit unit;
				double value;

				value = g_strtod (items[i], &unit_str);
				unit = gmathml_unit_from_string (unit_str);

				if (style_value->n_spaces > 0) {
					unsigned int index;

					index = MIN (i, style_value->n_spaces - 1);

					if (unit == GMATHML_UNIT_NONE) {
						unit = style_value->spaces[index].length.unit;
						value *= style_value->spaces[index].length.value;
					} else if (unit == GMATHML_UNIT_PERCENT) {
						unit = style_value->spaces[index].length.unit;
						value *= style_value->spaces[index].length.value / 100.0;
					}
				} else {
					unit = GMATHML_UNIT_PX;
					value = 0.0;
				}

				space->length.unit = unit;
				space->length.value = value;
			} else {
				space->length.value = 0.0;
				space->length.unit = GMATHML_UNIT_PX;
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
			case GMATHML_SPACE_NAME_VERY_VERY_THIN:
				attribute->values[i] = gmathml_length_compute (&style->very_very_thin_math_space,
									       style->very_very_thin_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_VERY_THIN:
				attribute->values[i] = gmathml_length_compute (&style->very_thin_math_space,
									       style->very_thin_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_THIN:
				attribute->values[i] = gmathml_length_compute (&style->thin_math_space,
									       style->thin_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_MEDIUM:
				attribute->values[i] = gmathml_length_compute (&style->medium_math_space,
									       style->medium_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_THICK:
				attribute->values[i] = gmathml_length_compute (&style->thick_math_space,
									       style->thick_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_VERY_THICK:
				attribute->values[i] = gmathml_length_compute (&style->very_thick_math_space,
									       style->very_thick_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_VERY_VERY_THICK:
				attribute->values[i] = gmathml_length_compute (&style->very_very_thick_math_space,
									       style->very_very_thick_math_space_value,
									       style->math_size_value);
				break;
			case GMATHML_SPACE_NAME_ERROR:
			default:
				if (style_value->n_spaces > 0) {
					unsigned int index;

					index = MIN (i, style_value->n_spaces - 1);

					attribute->values[i] = gmathml_length_compute
						(&attribute->space_list->spaces[i].length,
						 style_value->spaces[index].length.value,
						 style->math_size_value);
				} else
					attribute->values[i] = gmathml_length_compute
						(&attribute->space_list->spaces[i].length,
						 0.0,
						 style->math_size_value);
		}
	}
}

void
gmathml_attribute_named_list_finalize (void *abstract)
{
	GMathmlAttributeNamedList *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;
	attribute->values = NULL;
}

static void
gmathml_attribute_named_list_parse (GMathmlAttributeNamedList *attribute,
				    GMathmlNamedList *style_value,
				    GMathmlNamedConvert convert)
{
	const char *string;
	char **items;
	unsigned int i;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;

	string = gmathml_attribute_value_get_actual_value ((GMathmlAttributeValue *) attribute);
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
gmathml_attribute_row_align_list_parse (GMathmlAttributeNamedList *attribute,
					GMathmlNamedList *style_value)
{
	gmathml_attribute_named_list_parse (attribute, style_value, gmathml_row_align_from_string);
}

void
gmathml_attribute_column_align_list_parse (GMathmlAttributeNamedList *attribute,
					   GMathmlNamedList *style_value)
{
	gmathml_attribute_named_list_parse (attribute, style_value, gmathml_column_align_from_string);
}

void
gmathml_attribute_line_list_parse (GMathmlAttributeNamedList *attribute,
				   GMathmlNamedList *style_value)
{
	gmathml_attribute_named_list_parse (attribute, style_value, gmathml_line_from_string);
}
