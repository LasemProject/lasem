/* gmathmlenums.c
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

#include <gsvgattributes.h>
#include <gdomdebug.h>

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
