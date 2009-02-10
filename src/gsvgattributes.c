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
#include <gdomdebug.h>
#include <string.h>

void
gsvg_length_attribute_parse (GSvgLengthAttribute *attribute,
			     GSvgLength *default_value)
{
	const char *string;
	char *length_type_str;

	g_return_if_fail (attribute != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		g_return_if_fail (default_value != NULL);

		attribute->length = *default_value;
	} else {
		attribute->length.value = g_strtod (string, &length_type_str);
		attribute->length.type = gsvg_length_type_from_string (length_type_str);

		*default_value = attribute->length;
	}
}

void
gsvg_animated_length_attribute_parse (GSvgAnimatedLengthAttribute *attribute,
				      GSvgLength *default_value)
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
		attribute->length.base.value = g_strtod (string, &length_type_str);
		attribute->length.base.type = gsvg_length_type_from_string (length_type_str);
		attribute->length.animated = attribute->length.base;

		*default_value = attribute->length.base;
	}
}

void
gsvg_paint_attribute_finalize (GSvgPaintAttribute *attribute)
{
}

void
gsvg_paint_attribute_parse (GSvgPaintAttribute *attribute,
			    GSvgPaint *default_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		g_free (attribute->paint.uri);
		if (default_value->uri != NULL)
			attribute->paint.uri = g_strdup (default_value->uri);
		else
			attribute->paint.uri = NULL;
		attribute->paint.color = default_value->color;
		attribute->paint.type = default_value->type;
	} else {
		unsigned int color = 0;

		g_free (attribute->paint.uri);

		if (g_str_has_prefix (string, "url(#")) {
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
		} else
			attribute->paint.uri = NULL;

		while (*string == ' ')
			string++;

		if (*string == '#') {
			int value, i;
			string++;

			for (i = 0; i < 6; i++) {
				if (*string >= '0' && *string <= '9')
					value = *string - '0';
				else if (*string >= 'A' && *string <= 'F')
					value = *string - 'A' + 10;
				else if (*string >= 'a' && *string <= 'a')
					value = *string - 'a' + 10;
				else {
					color = 0;
					break;
				}
				color = (color << 4) + value;
				string++;
			}

			if (i == 3) {
				color = ((color & 0xf00) << 8) | ((color & 0x0f0) << 4) | (color & 0x00f);
				color |= color << 4;
			}
		} else if (g_str_has_prefix (string, "rgb(")) {
			int i;
			double value;
			char *end_ptr;

			string += 4;

			for (i = 0; i < 3; i++) {
				value = g_ascii_strtod (string, &end_ptr);
				if (*end_ptr == '%') {
					value /= 100.0;
					end_ptr++;
				} else
					value /= 255.0;
				string = end_ptr;
				if ((i < 2 && *string != ',') ||
				    (i == 2 && *string != ')')) {
					color = 0;
					break;
				}

				color = (color << 8) + (int) (0.5 + CLAMP (value, 0.0, 255.0));
			}
		} else
			color = gsvg_color_from_string (string);

		attribute->paint.color.red = (double) ((color & 0xff0000) >> 16) / 255.0;
		attribute->paint.color.green = (double) ((color & 0x00ff00) >> 8) / 255.0;
		attribute->paint.color.blue = (double) (color & 0x0000ff) / 255.0;

		if (attribute->paint.uri != NULL)
			attribute->paint.type = GSVG_PAINT_TYPE_URI_RGB_COLOR;
		else
			attribute->paint.type = GSVG_PAINT_TYPE_RGB_COLOR;

		g_free (default_value->uri);
		if (attribute->paint.uri != NULL)
			default_value->uri = g_strdup (attribute->paint.uri);
		else
			default_value->uri = NULL;
		default_value->type = attribute->paint.type;
		default_value->color = attribute->paint.color;
	}
}

