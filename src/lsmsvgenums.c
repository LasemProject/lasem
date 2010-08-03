/*
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgenums.h>
#include <lsmtraits.h>
#include <string.h>

static const char *lsm_svg_length_type_strings[] = {
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
	if (length_type < 0 || length_type > LSM_SVG_LENGTH_TYPE_PC)
		return NULL;

	return lsm_svg_length_type_strings[length_type];
}

LsmSvgLengthType
lsm_svg_length_type_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_length_type_strings,
					   G_N_ELEMENTS (lsm_svg_length_type_strings));
}

static const char *lsm_svg_fill_rule_strings[] = {
	"nonzero",
	"evenodd"
};

const char *
lsm_svg_fill_rule_to_string (LsmSvgFillRule fill_rule)
{
	if (fill_rule < 0 || fill_rule > LSM_SVG_FILL_RULE_EVEN_ODD)
		return NULL;

	return lsm_svg_fill_rule_strings[fill_rule];
}

LsmSvgFillRule
lsm_svg_fill_rule_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_fill_rule_strings,
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
	if (line_join < 0 || line_join > LSM_SVG_LINE_JOIN_BEVEL)
		return NULL;

	return lsm_svg_line_join_strings[line_join];
}

LsmSvgLineJoin
lsm_svg_line_join_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_line_join_strings,
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
	if (line_cap < 0 || line_cap > LSM_SVG_LINE_CAP_SQUARE)
		return NULL;

	return lsm_svg_line_cap_strings[line_cap];
}

LsmSvgLineCap
lsm_svg_line_cap_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_line_cap_strings,
					   G_N_ELEMENTS (lsm_svg_line_cap_strings));
}

static const char *lsm_svg_pattern_units_strings[] = {
	"userSpaceOnUse",
	"objectBoundingBox"
};

const char *
lsm_svg_pattern_units_to_string (LsmSvgPatternUnits units)
{
	if (units < 0 || units > LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX)
		return NULL;

	return lsm_svg_pattern_units_strings[units];
}

LsmSvgPatternUnits
lsm_svg_pattern_units_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_pattern_units_strings,
					   G_N_ELEMENTS (lsm_svg_pattern_units_strings));
}

static const char *lsm_svg_marker_units_strings[] = {
	"userSpaceOnUse",
	"strokeWidth"
};

const char *
lsm_svg_marker_units_to_string (LsmSvgMarkerUnits units)
{
	if (units < 0 || units > LSM_SVG_MARKER_UNITS_STROKE_WIDTH)
		return NULL;

	return lsm_svg_marker_units_strings[units];
}

LsmSvgMarkerUnits
lsm_svg_marker_units_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_marker_units_strings,
					   G_N_ELEMENTS (lsm_svg_marker_units_strings));
}

static const char *lsm_svg_spread_method_strings[] = {
	"pad",
	"reflect",
	"repeat"
};

const char *
lsm_svg_spread_method_to_string (LsmSvgSpreadMethod units)
{
	if (units < 0 || units > LSM_SVG_SPREAD_METHOD_REPEAT)
		return NULL;

	return lsm_svg_spread_method_strings[units];
}

LsmSvgSpreadMethod
lsm_svg_spread_method_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_spread_method_strings,
					 G_N_ELEMENTS (lsm_svg_spread_method_strings));
}

static const char *lsm_svg_align_strings[] = {
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
	if (align < 0 || align > LSM_SVG_ALIGN_X_MAX_Y_MAX)
		return NULL;

	return lsm_svg_align_strings[align];
}

LsmSvgAlign
lsm_svg_align_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_align_strings,
					   G_N_ELEMENTS (lsm_svg_align_strings));
}

static const char *lsm_svg_meet_or_slice_strings[] = {
	"meet",
	"slice"
};

const char *
lsm_svg_meet_or_slice_to_string (LsmSvgMeetOrSlice meet_or_slice)
{
	if (meet_or_slice < 0 || meet_or_slice > LSM_SVG_MEET_OR_SLICE_SLICE)
		return NULL;

	return lsm_svg_meet_or_slice_strings[meet_or_slice];
}

LsmSvgMeetOrSlice
lsm_svg_meet_or_slice_from_string (const char *string)
{
	return lsm_enum_value_from_string (string, lsm_svg_meet_or_slice_strings,
					  G_N_ELEMENTS (lsm_svg_meet_or_slice_strings));
}
