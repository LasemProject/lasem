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

static const char *lsm_svg_line_join_strings[] = {
	"miter",
	"round",
	"bevel"
};

const char *
lsm_svg_line_join_to_string (LsmSvgLineJoin line_join)
{
	return lsm_svg_line_join_strings[CLAMP (line_join, 0, LSM_SVG_LINE_JOIN_BEVEL)];
}

LsmSvgLineJoin
lsm_svg_line_join_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_line_join_strings,
					  G_N_ELEMENTS (lsm_svg_line_join_strings));
}

static const char *lsm_svg_line_cap_strings[] = {
	"butt",
	"round",
	"square"
};

const char *
lsm_svg_line_cap_to_string (LsmSvgLineCap line_cap)
{
	return lsm_svg_line_cap_strings[CLAMP (line_cap, 0, LSM_SVG_LINE_CAP_SQUARE)];
}

LsmSvgLineCap
lsm_svg_line_cap_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_line_cap_strings,
					  G_N_ELEMENTS (lsm_svg_line_cap_strings));
}

static const char *lsm_svg_pattern_units_strings[] = {
	"userSpaseOnUse",
	"objectBoundingBox"
};

const char *
lsm_svg_pattern_units_to_string (LsmSvgPatternUnits units)
{
	return lsm_svg_pattern_units_strings[CLAMP (units, 0, LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX)];
}

LsmSvgPatternUnits
lsm_svg_pattern_units_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_pattern_units_strings,
					  G_N_ELEMENTS (lsm_svg_pattern_units_strings));
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

static const char *lsm_svg_align_strings[] = {
	"",
	"none",
	"xMinYMin",
	"xMidYMin",
	"xMaxYMin",
	"xMinYMid",
	"xMidYMid",
	"xMaxYMid",
	"xMinYMax",
	"xMidYMax",
	"xMaxYMax"
};

const char *
lsm_svg_align_to_string (LsmSvgAlign align)
{
	return lsm_svg_align_strings[CLAMP (align, 0, LSM_SVG_ALIGN_X_MAX_Y_MAX)];
}

LsmSvgAlign
lsm_svg_align_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_align_strings,
					  G_N_ELEMENTS (lsm_svg_align_strings));
}

static const char *lsm_svg_meet_or_slice_strings[] = {
	"",
	"meet",
	"slice"
};

const char *
lsm_svg_meet_or_slice_to_string (LsmSvgMeetOrSlice meet_or_slice)
{
	return lsm_svg_meet_or_slice_strings[CLAMP (meet_or_slice, 0, LSM_SVG_MEET_OR_SLICE_SLICE)];
}

LsmSvgMeetOrSlice
lsm_svg_meet_or_slice_from_string (const char *string)
{
	return lsm_svg_value_from_string (string, lsm_svg_meet_or_slice_strings,
					  G_N_ELEMENTS (lsm_svg_meet_or_slice_strings));
}
