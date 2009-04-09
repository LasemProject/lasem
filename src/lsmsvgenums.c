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

#include <lsmsvgenums.h>
#include <string.h>

static unsigned int
lsm_svg_value_from_string (const char *string, const char **strings, unsigned int n_strings)
{
	int i;

	if (string == NULL)
		return 0;

	for (i = 0; i < n_strings; i++)
		if (strcmp (string, strings[i]) == 0)
			return i;

	return 0;
}

static const char *lsm_svg_length_type_strings[] = {
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
lsm_svg_length_type_to_string (LsmSvgLengthType length_type)
{
	return lsm_svg_length_type_strings[CLAMP (length_type, 0, LSM_SVG_LENGTH_TYPE_PC)];
}

LsmSvgLengthType
lsm_svg_length_type_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_length_type_strings,
				       G_N_ELEMENTS (lsm_svg_length_type_strings));
}

static const char *lsm_svg_fill_rule_strings[] = {
	"nonzero",
	"evenodd"
};

const char *
lsm_svg_fill_rule_to_string (LsmSvgFillRule fill_rule)
{
	return lsm_svg_fill_rule_strings[CLAMP (fill_rule, 0, LSM_SVG_FILL_RULE_EVEN_ODD)];
}

LsmSvgFillRule
lsm_svg_fill_rule_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_fill_rule_strings,
				       G_N_ELEMENTS (lsm_svg_fill_rule_strings));
}

static const char *lsm_svg_gradient_units_strings[] = {
	"userSpaseOnUse",
	"objectBoundingBox"
};

const char *
lsm_svg_gradient_units_to_string (LsmSvgGradientUnits units)
{
	return lsm_svg_gradient_units_strings[CLAMP (units, 0, LSM_SVG_GRADIENT_UNITS_OBJECT_BOUNDING_BOX)];
}

LsmSvgGradientUnits
lsm_svg_gradient_units_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_gradient_units_strings,
				       G_N_ELEMENTS (lsm_svg_gradient_units_strings));
}

static const char *lsm_svg_spread_method_strings[] = {
	"pad",
	"reflect",
	"repeat"
};

const char *
lsm_svg_spread_method_to_string (LsmSvgSpreadMethod units)
{
	return lsm_svg_spread_method_strings[CLAMP (units, 0, LSM_SVG_SPREAD_METHOD_REPEAT)];
}

LsmSvgSpreadMethod
lsm_svg_spread_method_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_spread_method_strings,
				       G_N_ELEMENTS (lsm_svg_spread_method_strings));
}
