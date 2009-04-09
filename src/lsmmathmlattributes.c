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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>
#include <math.h>

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

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
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

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
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
lsm_mathml_mode_attribute_parse (LsmDomEnumAttribute *attribute,
			      unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_mode_from_string);
}

void
lsm_mathml_display_attribute_parse (LsmDomEnumAttribute *attribute,
				 unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_display_from_string);
}

void
lsm_mathml_form_attribute_parse (LsmDomEnumAttribute *attribute,
			      unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_form_from_string);
}

void
lsm_mathml_font_style_attribute_parse (LsmDomEnumAttribute *attribute,
				    unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_font_style_from_string);
}

void
lsm_mathml_font_weight_attribute_parse (LsmDomEnumAttribute *attribute,
				     unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_font_weight_from_string);
}

void
lsm_mathml_variant_attribute_parse (LsmDomEnumAttribute *attribute,
				 unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_variant_from_string);
}

void
lsm_mathml_line_attribute_parse (LsmDomEnumAttribute *attribute,
			      unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_mathml_line_from_string);
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

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
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

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
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

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
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
lsm_mathml_row_align_list_attribute_parse (LsmDomEnumListAttribute *attribute,
					LsmDomEnumList *style_value)
{
	lsm_dom_enum_list_attribute_parse (attribute, style_value, lsm_mathml_row_align_from_string);
}

void
lsm_mathml_column_align_list_attribute_parse (LsmDomEnumListAttribute *attribute,
					   LsmDomEnumList *style_value)
{
	lsm_dom_enum_list_attribute_parse (attribute, style_value, lsm_mathml_column_align_from_string);
}

void
lsm_mathml_line_list_attribute_parse (LsmDomEnumListAttribute *attribute,
				   LsmDomEnumList *style_value)
{
	lsm_dom_enum_list_attribute_parse (attribute, style_value, lsm_mathml_line_from_string);
}

static const LsmDomAttributeClass space_list_attribute_class = {
	.finalize = lsm_mathml_space_list_attribute_finalize
};

void
lsm_dom_attribute_map_add_space_list (LsmDomAttributeMap *map,
				   char const *name,
				   ptrdiff_t offset)
{
	lsm_dom_attribute_map_add_attribute_full (map, name, offset, &space_list_attribute_class);
}

