/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#include <lsmsvgattributes.h>
#include <lsmsvgcolors.h>
#include <lsmsvgutils.h>
#include <lsmsvgstyle.h>
#include <lsmdebug.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const LsmSvgDashArray lsm_svg_dash_array_null = {0, NULL};

LsmSvgDashArray *
lsm_svg_dash_array_new (unsigned int n_dashes)
{
	LsmSvgDashArray *array;

	g_return_val_if_fail (n_dashes > 0, (LsmSvgDashArray *) &lsm_svg_dash_array_null);

	array = g_new (LsmSvgDashArray, 1);
	if (array != NULL) {
		array->n_dashes = n_dashes;
		array->dashes = g_new (double, n_dashes);
		if (array->dashes != NULL)
			return array;
		g_free (array);
	}

	return (LsmSvgDashArray *) &lsm_svg_dash_array_null;
}

void
lsm_svg_dash_array_free (LsmSvgDashArray *array)
{
	if (array == NULL || array == &lsm_svg_dash_array_null)
		return;

	g_free (array->dashes);
	g_free (array);
}

LsmSvgDashArray *
lsm_svg_dash_array_duplicate (const LsmSvgDashArray *origin)
{
	LsmSvgDashArray *duplicate;

	if (origin == NULL || origin == &lsm_svg_dash_array_null)
		return (LsmSvgDashArray *) &lsm_svg_dash_array_null;

	duplicate = lsm_svg_dash_array_new (origin->n_dashes);

	if (duplicate != &lsm_svg_dash_array_null)
		memcpy (duplicate->dashes, origin->dashes, sizeof (double) * origin->n_dashes);

	return duplicate;
}

static const char *
lsm_svg_attribute_get_value (LsmDomAttribute *attribute)
{
	const char *string;

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);

	if (g_strcmp0 (string, "inherit") == 0)
		return NULL;

	return string;
}

const LsmSvgColor lsm_svg_color_null = {0.0, 0.0, 0.0};

void
lsm_svg_double_attribute_parse (LsmSvgDoubleAttribute *attribute,
				double *double_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (double_value != NULL);

	string = lsm_svg_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string == NULL) {
		attribute->value = *double_value;
		return;
	}

	attribute->value = atof (string);
	*double_value = attribute->value;
}

static double
lsm_svg_length_compute (const LsmSvgLength *length,
			const LsmSvgStyle *style,
			LsmSvgLengthDirection direction)
{
	double viewport;
	double viewport_scale;

	g_return_val_if_fail (length != NULL, 0.0);

	switch (direction) {
		case LSM_SVG_LENGTH_DIRECTION_HORIZONTAL:
			viewport = style->viewport.width;
			viewport_scale = style->viewport.horizontal_scale;
			break;
		case LSM_SVG_LENGTH_DIRECTION_VERTICAL:
			viewport = style->viewport.height;
			viewport_scale = style->viewport.vertical_scale;
			break;
		case LSM_SVG_LENGTH_DIRECTION_DIAGONAL:
			viewport = style->viewport.diagonal;
			viewport_scale = style->viewport.diagonal_scale;
			break;
		default:
			viewport = 0.0;
			viewport_scale = 0.0;
	}

	switch (length->type) {
		case LSM_SVG_LENGTH_TYPE_NUMBER:
		case LSM_SVG_LENGTH_TYPE_UNKNOWN:
			return length->value_unit;
		case LSM_SVG_LENGTH_TYPE_PERCENTAGE:
			return length->value_unit * viewport / 100.0;
		case LSM_SVG_LENGTH_TYPE_PX:
			return length->value_unit * 72.0 * viewport_scale / style->resolution_ppi;
		case LSM_SVG_LENGTH_TYPE_PT:
			return length->value_unit * viewport_scale;
		case LSM_SVG_LENGTH_TYPE_PC:
			return length->value_unit * 72.0 * viewport_scale / 6.0;
		case LSM_SVG_LENGTH_TYPE_CM:
			return length->value_unit * 72.0 * viewport_scale / 2.54;
		case LSM_SVG_LENGTH_TYPE_MM:
			return length->value_unit * 72.0 * viewport_scale / 25.4;
		case LSM_SVG_LENGTH_TYPE_IN:
			return length->value_unit * 72.0 * viewport_scale;
		case LSM_SVG_LENGTH_TYPE_EMS:
			return length->value_unit * style->text.font_size.value;
		case LSM_SVG_LENGTH_TYPE_EXS:
			return length->value_unit * style->text.font_size.value * 0.5;
	}

	g_warning ("[LsmSvgLength::compute] Invalid length property");

	return 0.0;
}

void
lsm_svg_length_attribute_parse (LsmSvgLengthAttribute *attribute,
				LsmSvgLength *default_value,
				const LsmSvgStyle *style,
				LsmSvgLengthDirection direction)
{
	const char *string;
	char *length_type_str;

	g_return_if_fail (attribute != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->length = *default_value;
	} else {
		attribute->length.value_unit = g_strtod (string, &length_type_str);
		attribute->length.type = lsm_svg_length_type_from_string (length_type_str);
		attribute->length.value = lsm_svg_length_compute (&attribute->length, style, direction);

		*default_value = attribute->length;
	}
}

void
lsm_svg_animated_length_attribute_parse (LsmSvgAnimatedLengthAttribute *attribute,
					 LsmSvgLength *default_value,
					 const LsmSvgStyle *style,
					 LsmSvgLengthDirection direction)
{
	const char *string;
	char *length_type_str;

	g_return_if_fail (attribute != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->length.base = *default_value;
		attribute->length.animated = *default_value;
	} else {
		attribute->length.base.value_unit = g_strtod (string, &length_type_str);
		attribute->length.base.type = lsm_svg_length_type_from_string (length_type_str);
		attribute->length.base.value = lsm_svg_length_compute (&attribute->length.base, style, direction);
		attribute->length.animated = attribute->length.base;

		*default_value = attribute->length.base;
	}
}

void
lsm_svg_dash_array_attribute_finalize (void *abstract)
{
	LsmSvgDashArrayAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	lsm_svg_dash_array_free (attribute->value);
	attribute->value = NULL;
}

void
lsm_svg_dash_array_attribute_parse (LsmSvgDashArrayAttribute *attribute,
				    LsmSvgDashArray **default_value,
				    const LsmSvgStyle *style,
				    LsmSvgLengthDirection direction)
{
	const char *string;

	g_return_if_fail (attribute != NULL);

	string = lsm_svg_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string == NULL) {
		lsm_svg_dash_array_free (attribute->value);
		attribute->value = lsm_svg_dash_array_duplicate (*default_value);
	} else {
		unsigned int n_dashes = 1;

		lsm_svg_dash_array_free (attribute->value);

		if (strcmp (string, "none") == 0) {
			attribute->value = (LsmSvgDashArray *) &lsm_svg_dash_array_null;
		} else {
			char *iter = (char *) string;
			unsigned int i;

			while (*iter != '\0') {
				if (*iter == ',')
					n_dashes++;
				iter++;
			}

			attribute->value = lsm_svg_dash_array_new (n_dashes);
			if (attribute->value != &lsm_svg_dash_array_null) {
				LsmSvgLength length;

				iter = (char *)string;
				lsm_svg_str_skip_spaces (&iter);

				for (i = 0; i < n_dashes; i++) {
					if (lsm_svg_str_parse_double (&iter, &length.value_unit)) {
						length.type = lsm_svg_length_type_from_string (iter);
						attribute->value->dashes[i] =
							lsm_svg_length_compute (&length, style, direction);
						while (*iter != '\0' && *iter != ' ' && *iter != ',')
							iter ++;
					} else
						attribute->value->dashes[i] = 0.0;
					lsm_svg_str_skip_comma_and_spaces (&iter);
				}
			}
		}

		lsm_svg_dash_array_free (*default_value);
		*default_value = lsm_svg_dash_array_duplicate (attribute->value);
	}
}

void
lsm_svg_fill_rule_attribute_parse (LsmDomEnumAttribute *attribute,
				   unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_svg_fill_rule_from_string);
}

void
lsm_svg_line_join_attribute_parse (LsmDomEnumAttribute *attribute,
				   unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_svg_line_join_from_string);
}

void
lsm_svg_line_cap_attribute_parse (LsmDomEnumAttribute *attribute,
				  unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_svg_line_cap_from_string);
}

void
lsm_svg_gradient_units_attribute_parse (LsmDomEnumAttribute *attribute,
					unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_svg_gradient_units_from_string);
}

void
lsm_svg_spread_method_attribute_parse (LsmDomEnumAttribute *attribute,
				       unsigned int *style_value)
{
	return lsm_dom_enum_attribute_parse (attribute, style_value, lsm_svg_spread_method_from_string);
}

static char *
_parse_color (char *string,
	      LsmSvgColor *svg_color,
	      gboolean *color_set,
	      const LsmSvgColor *current_color)
{
	unsigned int color = 0;
	*color_set = FALSE;

	lsm_svg_str_skip_spaces (&string);

	if (g_strcmp0 (string, "currentColor") == 0) {
		*svg_color = *current_color;
		*color_set = TRUE;

		string += 12; /* strlen ("current_color") */

		return string;
	}

	if (*string == '#') {
		int value, i;
		string++;

		for (i = 0; i < 6; i++) {
			if (*string >= '0' && *string <= '9')
				value = *string - '0';
			else if (*string >= 'A' && *string <= 'F')
				value = *string - 'A' + 10;
			else if (*string >= 'a' && *string <= 'f')
				value = *string - 'a' + 10;
			else
				break;

			color = (color << 4) + value;
			string++;
		}

		if (i == 3) {
			color = ((color & 0xf00) << 8) | ((color & 0x0f0) << 4) | (color & 0x00f);
			color |= color << 4;
		} else if (i != 6)
			color = 0;

		*color_set = TRUE;
	} else if (strncmp (string, "rgb(", 4) == 0) {
		int i;
		double value;


		string += 4; /* strlen ("rgb(") */

		for (i = 0; i < 3; i++) {
			if (!lsm_svg_str_parse_double (&string, &value))
				break;

			if (*string == '%') {
				value = value * 255.0 / 100.0;
				string++;
			}

			if (i < 2)
				lsm_svg_str_skip_comma_and_spaces (&string);

			color = (color << 8) + (int) (0.5 + CLAMP (value, 0.0, 255.0));
		}

		lsm_svg_str_skip_spaces (&string);

		if (*string != ')' || i != 3)
			color = 0;

		*color_set  = TRUE;
	} else if (g_strcmp0 (string, "none") == 0) {
		*color_set = FALSE;
	} else {
		color = lsm_svg_color_from_string (string);

		*color_set = TRUE;
	}

	svg_color->red = (double) ((color & 0xff0000) >> 16) / 255.0;
	svg_color->green = (double) ((color & 0x00ff00) >> 8) / 255.0;
	svg_color->blue = (double) (color & 0x0000ff) / 255.0;

	return string;
}

void
lsm_svg_paint_attribute_finalize (void *abstract)
{
	LsmSvgPaintAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->paint.uri);
	attribute->paint.uri = NULL;
}

void
lsm_svg_paint_attribute_parse (LsmSvgPaintAttribute *attribute,
			       LsmSvgPaint *default_value,
			       const LsmSvgColor *current_color)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) lsm_svg_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string == NULL) {
		g_free (attribute->paint.uri);
		if (default_value->uri != NULL)
			attribute->paint.uri = g_strdup (default_value->uri);
		else
			attribute->paint.uri = NULL;
		attribute->paint.color = default_value->color;
		attribute->paint.type = default_value->type;
	} else {
		gboolean color_set;

		g_free (attribute->paint.uri);

		if (strncmp (string, "url(#", 5) == 0) {
			unsigned int length;

			string += 5;
			length = 0;
			while (string[length] != ')')
				length++;
			length++;

			attribute->paint.uri = g_new (char, length);
			if (attribute->paint.uri != NULL) {
				memcpy (attribute->paint.uri, string, length - 1);
				attribute->paint.uri[length - 1] = '\0';
			}
			string += length;
		} else {
			attribute->paint.uri = NULL;
		}

		string = _parse_color (string, &attribute->paint.color, &color_set,
				       current_color);

		if (color_set)
			attribute->paint.type = attribute->paint.uri != NULL ?
				LSM_SVG_PAINT_TYPE_URI_RGB_COLOR :
				LSM_SVG_PAINT_TYPE_RGB_COLOR;
		else
			attribute->paint.type = attribute->paint.uri != NULL ?
				LSM_SVG_PAINT_TYPE_URI :
				LSM_SVG_PAINT_TYPE_NONE;

		g_free (default_value->uri);
		if (attribute->paint.uri != NULL)
			default_value->uri = g_strdup (attribute->paint.uri);
		else
			default_value->uri = NULL;
		default_value->type = attribute->paint.type;
		default_value->color = attribute->paint.color;
	}
}

void
lsm_svg_color_attribute_parse (LsmSvgColorAttribute *attribute,
			       LsmSvgColor *default_value,
			       const LsmSvgColor *current_color)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) lsm_svg_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string == NULL) {
		attribute->value = *default_value;
	} else {
		gboolean color_set;

		string = _parse_color (string, &attribute->value, &color_set, current_color);

		*default_value = attribute->value;
	}
}

void
lsm_svg_view_box_attribute_parse (LsmSvgViewBoxAttribute *attribute)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);

	if (string != NULL) {
		unsigned int i;
		double value[4];

		for (i = 0; i < 4 && *string != '\0'; i++) {
			lsm_svg_str_skip_semicolon_and_spaces (&string);

			if (!lsm_svg_str_parse_double (&string, &value[i]))
				break;
		}

		if (i == 4) {
			attribute->value.x = value[0];
			attribute->value.y = value[1];
			attribute->value.width = value[2];
			attribute->value.height = value[3];
		} else {
			attribute->value.x = 0;
			attribute->value.y = 0;
			attribute->value.width = 0;
			attribute->value.height = 0;
		}
	} else {
		attribute->value.x = 0;
		attribute->value.y = 0;
		attribute->value.width = 0;
		attribute->value.height = 0;
	}
}

static void
_init_matrix (LsmSvgMatrix *matrix, LsmSvgTransformType transform, unsigned int n_values, double values[])
{
	switch (transform) {
		case LSM_SVG_TRANSFORM_TYPE_SCALE:
			if (n_values == 1) {
				lsm_svg_matrix_init_scale (matrix, values[0], values[0]);
				return;
			} else if (n_values == 2) {
				lsm_svg_matrix_init_scale (matrix, values[0], values[1]);
				return;
			}
			break;
		case LSM_SVG_TRANSFORM_TYPE_TRANSLATE:
			if (n_values == 1) {
				lsm_svg_matrix_init_translate (matrix, values[0], values[0]);
				return;
			} else if (n_values == 2) {
				lsm_svg_matrix_init_translate (matrix, values[0], values[1]);
				return;
			}
			break;
		case LSM_SVG_TRANSFORM_TYPE_MATRIX:
			if (n_values == 6) {
				lsm_svg_matrix_init (matrix,
						  values[0], values[1],
						  values[2], values[3],
						  values[4], values[5]);
				return;
			}
			break;
		case LSM_SVG_TRANSFORM_TYPE_ROTATE:
			if (n_values == 1) {
				lsm_svg_matrix_init_rotate (matrix, values[0] * M_PI / 180.0);
				return;
			} else if (n_values == 3) {
				LsmSvgMatrix matrix_b;

				lsm_svg_matrix_init_translate (matrix, values[1], values[2]);
				lsm_svg_matrix_init_rotate (&matrix_b, values[0] * M_PI / 180.0);
				lsm_svg_matrix_multiply (matrix, &matrix_b, matrix);
				lsm_svg_matrix_init_translate (&matrix_b, -values[1], -values[2]);
				lsm_svg_matrix_multiply (matrix, &matrix_b, matrix);
				return;
			}
			break;
		case LSM_SVG_TRANSFORM_TYPE_SKEW_X:
			if (n_values == 1) {
				lsm_svg_matrix_init_skew_x (matrix, values[0] * M_PI / 180.0);
				return;
			}
			break;
		case LSM_SVG_TRANSFORM_TYPE_SKEW_Y:
			if (n_values == 1) {
				lsm_svg_matrix_init_skew_y (matrix, values[0] * M_PI / 180.0);
				return;
			}
		default:
			break;
	}

	lsm_svg_matrix_init_identity (matrix);
}

void
lsm_svg_transform_attribute_parse (LsmSvgTransformAttribute *attribute)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	lsm_svg_matrix_init_identity (&attribute->matrix);

	string = (char *) lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string != NULL) {
		while (*string != '\0') {
			LsmSvgTransformType transform;
			double values[6];

			lsm_svg_str_skip_spaces (&string);

			if (strncmp (string, "translate", 9) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_TRANSLATE;
				string += 9;
			} else if (strncmp (string, "scale", 5) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_SCALE;
				string += 5;
			} else if (strncmp (string, "rotate", 6) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_ROTATE;
				string += 6;
			} else if (strncmp (string, "matrix", 6) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_MATRIX;
				string += 6;
			} else if (strncmp (string, "skewX", 5) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_SKEW_X;
				string += 5;
			} else if (strncmp (string, "skewY", 5) == 0) {
				transform = LSM_SVG_TRANSFORM_TYPE_SKEW_Y;
				string += 5;
			} else
				break;

			lsm_svg_str_skip_spaces (&string);

			if (*string == '(') {
				unsigned int n_values = 0;

				string++;

				while (*string != ')' && *string != '\0' && n_values < 6) {
					lsm_svg_str_skip_comma_and_spaces (&string);

					if (!lsm_svg_str_parse_double (&string, &values[n_values]))
						break;

					n_values++;
				}

				lsm_svg_str_skip_comma_and_spaces (&string);

				if (*string == ')') {
					LsmSvgMatrix matrix;

					string++;

					_init_matrix (&matrix, transform, n_values, values);

					lsm_svg_matrix_multiply (&attribute->matrix, &matrix, &attribute->matrix);
				}
			}
		}
	}
}
