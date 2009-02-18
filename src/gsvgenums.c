/*
 * Copyright © 2009 Emmanuel Pacaud
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

#include <gsvgenums.h>
#include <string.h>

static unsigned int
gsvg_value_from_string (const char *string, const char **strings, unsigned int n_strings)
{
	int i;

	if (string == NULL)
		return 0;

	for (i = 0; i < n_strings; i++)
		if (strcmp (string, strings[i]) == 0)
			return i;

	return 0;
}

static const char *gsvg_length_type_strings[] = {
	"",
	"",
	"%",
	"em",
	"ex",
	"px",
	"cm",
	"mm",
	"in",
	"pt",
	"pc"
};

const char *
gsvg_length_type_to_string (GSvgLengthType length_type)
{
	return gsvg_length_type_strings[CLAMP (length_type, 0, GSVG_LENGTH_TYPE_PC)];
}

GSvgLengthType
gsvg_length_type_from_string (const char *string)
{
	return gsvg_value_from_string (string, gsvg_length_type_strings,
				       G_N_ELEMENTS (gsvg_length_type_strings));
}

static const char *gsvg_fill_rule_strings[] = {
	"nonzero",
	"evenodd"
};

const char *
gsvg_fill_rule_to_string (GSvgFillRule fill_rule)
{
	return gsvg_fill_rule_strings[CLAMP (fill_rule, 0, GSVG_FILL_RULE_EVEN_ODD)];
}

GSvgFillRule
gsvg_fill_rule_from_string (const char *string)
{
	return gsvg_value_from_string (string, gsvg_fill_rule_strings,
				       G_N_ELEMENTS (gsvg_fill_rule_strings));
}
