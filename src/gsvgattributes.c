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

#include <gsvgattributes.h>
#include <gsvgcolors.h>
#include <gsvgutils.h>
#include <gdomdebug.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const GSvgColor gsvg_color_null = {0.0, 0.0, 0.0};

static double
gsvg_length_compute (const GSvgLength *length, double viewbox, double font_size)
{
	g_return_val_if_fail (length != NULL, 0.0);

	switch (length->type) {
		case GSVG_LENGTH_TYPE_PX:
		case GSVG_LENGTH_TYPE_PT:
			return length->value_unit;
		case GSVG_LENGTH_TYPE_PC:
			return length->value_unit * 72.0 / 6.0;
		case GSVG_LENGTH_TYPE_CM:
			return length->value_unit * 72.0 / 2.54;
		case GSVG_LENGTH_TYPE_MM:
			return length->value_unit * 72.0 / 25.4;
		case GSVG_LENGTH_TYPE_IN:
			return length->value_unit * 72.0;
		case GSVG_LENGTH_TYPE_EMS:
			return length->value_unit * font_size;
		case GSVG_LENGTH_TYPE_EXS:
			return length->value_unit * font_size * 0.5;
		case GSVG_LENGTH_TYPE_PERCENTAGE:
			return viewbox * length->value_unit / 100.0;
		case GSVG_LENGTH_TYPE_NUMBER:
		case GSVG_LENGTH_TYPE_UNKNOWN:
			return length->value_unit;
	}

	return 0.0;
}

void
gsvg_length_attribute_parse (GSvgLengthAttribute *attribute,
			     GSvgLength *default_value,
			     double font_size)
{
	const char *string;
	char *length_type_str;

	g_return_if_fail (attribute != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->length = *default_value;
	} else {
		attribute->length.value_unit = g_strtod (string, &length_type_str);
		attribute->length.type = gsvg_length_type_from_string (length_type_str);
		attribute->length.value = gsvg_length_compute (&attribute->length,
							       default_value->value, font_size);

		*default_value = attribute->length;
	}
}

void
gsvg_animated_length_attribute_parse (GSvgAnimatedLengthAttribute *attribute,
				      GSvgLength *default_value,
				      double font_size)
{
	const char *string;
	char *length_type_str;

	g_return_if_fail (attribute != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->length.base = *default_value;
		attribute->length.animated = *default_value;
	} else {
		attribute->length.base.value_unit = g_strtod (string, &length_type_str);
		attribute->length.base.type = gsvg_length_type_from_string (length_type_str);
		attribute->length.base.value = gsvg_length_compute (&attribute->length.base,
								    default_value->value, font_size);
		attribute->length.animated = attribute->length.base;

		*default_value = attribute->length.base;
	}
}

void
gsvg_fill_rule_attribute_parse (GDomEnumAttribute *attribute,
				unsigned int *style_value)
{
	return gdom_enum_attribute_parse (attribute, style_value, gsvg_fill_rule_from_string);
}

void
gsvg_gradient_units_attribute_parse (GDomEnumAttribute *attribute,
				     unsigned int *style_value)
{
	return gdom_enum_attribute_parse (attribute, style_value, gsvg_gradient_units_from_string);
}

void
gsvg_spread_method_attribute_parse (GDomEnumAttribute *attribute,
				    unsigned int *style_value)
{
	return gdom_enum_attribute_parse (attribute, style_value, gsvg_spread_method_from_string);
}

static char *
_parse_color (char *string, GSvgColor *svg_color, gboolean *color_set)
{
	unsigned int color = 0;
	*color_set = FALSE;

	gsvg_str_skip_spaces (&string);

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


		string += 4;

		for (i = 0; i < 3; i++) {
			if (!gsvg_str_parse_double (&string, &value))
				break;

			if (*string == '%') {
				value = value * 255.0 / 100.0;
				string++;
			}

			if (i < 2)
				gsvg_str_skip_comma_and_spaces (&string);

			color = (color << 8) + (int) (0.5 + CLAMP (value, 0.0, 255.0));
		}

		gsvg_str_skip_spaces (&string);

		if (*string != ')' || i != 3)
			color = 0;

		*color_set  = TRUE;
	} else if (strcmp (string, "none") == 0) {
		*color_set = FALSE;
	} else {
		color = gsvg_color_from_string (string);

		*color_set = TRUE;
	}

	svg_color->red = (double) ((color & 0xff0000) >> 16) / 255.0;
	svg_color->green = (double) ((color & 0x00ff00) >> 8) / 255.0;
	svg_color->blue = (double) (color & 0x0000ff) / 255.0;

	return string;
}

void
gsvg_paint_attribute_finalize (void *abstract)
{
	GSvgPaintAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->paint.uri);
	attribute->paint.uri = NULL;
}

void
gsvg_paint_attribute_parse (GSvgPaintAttribute *attribute,
			    GSvgPaint *default_value)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) gdom_attribute_get_value ((GDomAttribute *) attribute);
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

		string = _parse_color (string, &attribute->paint.color, &color_set);

		if (color_set)
			attribute->paint.type = attribute->paint.uri != NULL ?
				GSVG_PAINT_TYPE_URI_RGB_COLOR :
				GSVG_PAINT_TYPE_RGB_COLOR;
		else
			attribute->paint.type = attribute->paint.uri != NULL ?
				GSVG_PAINT_TYPE_URI :
				GSVG_PAINT_TYPE_NONE;

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
gsvg_color_attribute_parse (GSvgColorAttribute *attribute,
			    GSvgColor *default_value)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *default_value;
	} else {
		gboolean color_set;

		string = _parse_color (string, &attribute->value, &color_set);

		*default_value = attribute->value;
	}
}

void
gsvg_view_box_attribute_parse (GSvgViewBoxAttribute *attribute,
			       GSvgViewBox *default_value)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	string = (char *) gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->value = *default_value;
	} else {
		unsigned int i;
		double value[4];

		for (i = 0; i < 4 && *string != '\0'; i++) {
			gsvg_str_skip_semicolon_and_spaces (&string);

			if (!gsvg_str_parse_double (&string, &value[i]))
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

		*default_value = attribute->value;
	}
}

static void
_init_matrix (GSvgMatrix *matrix, GSvgTransformType transform, unsigned int n_values, double values[])
{
	switch (transform) {
		case GSVG_TRANSFORM_TYPE_SCALE:
			if (n_values == 1) {
				gsvg_matrix_init_scale (matrix, values[0], values[0]);
				return;
			} else if (n_values == 2) {
				gsvg_matrix_init_scale (matrix, values[0], values[1]);
				return;
			}
			break;
		case GSVG_TRANSFORM_TYPE_TRANSLATE:
			if (n_values == 1) {
				gsvg_matrix_init_translate (matrix, values[0], values[0]);
				return;
			} else if (n_values == 2) {
				gsvg_matrix_init_translate (matrix, values[0], values[1]);
				return;
			}
			break;
		case GSVG_TRANSFORM_TYPE_MATRIX:
			if (n_values == 6) {
				gsvg_matrix_init (matrix,
						  values[0], values[1],
						  values[2], values[3],
						  values[4], values[5]);
				return;
			}
			break;
		case GSVG_TRANSFORM_TYPE_ROTATE:
			if (n_values == 1) {
				gsvg_matrix_init_rotate (matrix, values[0] * M_PI / 180.0);
				return;
			} else if (n_values == 3)
				g_warning ("[GSvgTransformAttribute::parse] Rotation center not handled");
			break;
		case GSVG_TRANSFORM_TYPE_SKEW_X:
			if (n_values == 1) {
				gsvg_matrix_init_skew_x (matrix, values[0] * M_PI / 180.0);
				return;
			}
			break;
		case GSVG_TRANSFORM_TYPE_SKEW_Y:
			if (n_values == 1) {
				gsvg_matrix_init_skew_y (matrix, values[0] * M_PI / 180.0);
				return;
			}
		default:
			break;
	}

	gsvg_matrix_init_identity (matrix);
}

void
gsvg_transform_attribute_parse (GSvgTransformAttribute *attribute)
{
	char *string;

	g_return_if_fail (attribute != NULL);

	gsvg_matrix_init_identity (&attribute->matrix);

	string = (char *) gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string != NULL) {
		while (*string != '\0') {
			GSvgTransformType transform;
			double values[6];

			printf ("transform = '%s'\n", string);

			gsvg_str_skip_spaces (&string);

			if (strncmp (string, "translate", 9) == 0) {
				transform = GSVG_TRANSFORM_TYPE_TRANSLATE;
				string += 9;
			} else if (strncmp (string, "scale", 5) == 0) {
				transform = GSVG_TRANSFORM_TYPE_SCALE;
				string += 5;
			} else if (strncmp (string, "rotate", 6) == 0) {
				transform = GSVG_TRANSFORM_TYPE_ROTATE;
				string += 6;
			} else if (strncmp (string, "matrix", 6) == 0) {
				transform = GSVG_TRANSFORM_TYPE_MATRIX;
				string += 6;
			} else if (strncmp (string, "skewX", 5) == 0) {
				transform = GSVG_TRANSFORM_TYPE_SKEW_X;
				string += 5;
			} else if (strncmp (string, "skewY", 5) == 0) {
				transform = GSVG_TRANSFORM_TYPE_SKEW_Y;
				string += 5;
			} else
				break;

			gsvg_str_skip_spaces (&string);

			if (*string == '(') {
				unsigned int n_values = 0;

				string++;

				while (*string != ')' && *string != '\0' && n_values < 6) {
					gsvg_str_skip_comma_and_spaces (&string);

					if (!gsvg_str_parse_double (&string, &values[n_values]))
						break;

					n_values++;
				}

				gsvg_str_skip_comma_and_spaces (&string);

				if (*string == ')') {
					GSvgMatrix matrix;

					string++;

					_init_matrix (&matrix, transform, n_values, values);

					gsvg_matrix_multiply (&attribute->matrix, &matrix, &attribute->matrix);
				}
			}
		}
	}
}
