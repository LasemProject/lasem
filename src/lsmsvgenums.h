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

#ifndef LSM_SVG_ENUMS_H
#define LSM_SVG_ENUMS_H

#include <lsmsvg.h>

G_BEGIN_DECLS

typedef enum {
	LSM_SVG_PAINT_TYPE_UNKNOWN = 0,
	LSM_SVG_PAINT_TYPE_RGB_COLOR,
	LSM_SVG_PAINT_TYPE_RGB_COLOR_ICC_COLOR,
	LSM_SVG_PAINT_TYPE_NONE = 101,
	LSM_SVG_PAINT_TYPE_CURRENT_COLOR,
	LSM_SVG_PAINT_TYPE_URI_NONE,
	LSM_SVG_PAINT_TYPE_URI_CURRENT_COLOR,
	LSM_SVG_PAINT_TYPE_URI_RGB_COLOR,
	LSM_SVG_PAINT_TYPE_URI_RGB_COLOR_ICC_COLOR,
	LSM_SVG_PAINT_TYPE_URI
} LsmSvgPaintType;

typedef enum {
	LSM_SVG_LENGTH_DIRECTION_HORIZONTAL,
	LSM_SVG_LENGTH_DIRECTION_VERTICAL,
	LSM_SVG_LENGTH_DIRECTION_DIAGONAL
} LsmSvgLengthDirection;

typedef enum {
	LSM_SVG_LENGTH_TYPE_UNKNOWN,
	LSM_SVG_LENGTH_TYPE_NUMBER,
	LSM_SVG_LENGTH_TYPE_PERCENTAGE,
	LSM_SVG_LENGTH_TYPE_EMS,
	LSM_SVG_LENGTH_TYPE_EXS,
	LSM_SVG_LENGTH_TYPE_PX,
	LSM_SVG_LENGTH_TYPE_CM,
	LSM_SVG_LENGTH_TYPE_MM,
	LSM_SVG_LENGTH_TYPE_IN,
	LSM_SVG_LENGTH_TYPE_PT,
	LSM_SVG_LENGTH_TYPE_PC
} LsmSvgLengthType;

const char * 		lsm_svg_length_type_to_string 		(LsmSvgLengthType length_type);
LsmSvgLengthType	lsm_svg_length_type_from_string 	(const char *string);

typedef enum {
	LSM_SVG_FILL_RULE_NON_ZERO,
	LSM_SVG_FILL_RULE_EVEN_ODD
} LsmSvgFillRule;

const char * 		lsm_svg_fill_rule_to_string 		(LsmSvgFillRule fill_rule);
LsmSvgFillRule 		lsm_svg_fill_rule_from_string 		(const char *string);

typedef enum {
	LSM_SVG_LINE_JOIN_MITER,
	LSM_SVG_LINE_JOIN_ROUND,
	LSM_SVG_LINE_JOIN_BEVEL
} LsmSvgLineJoin;

const char * 		lsm_svg_line_join_to_string 		(LsmSvgLineJoin line_join);
LsmSvgLineJoin 		lsm_svg_line_join_from_string 		(const char *string);

typedef enum {
	LSM_SVG_LINE_CAP_BUTT,
	LSM_SVG_LINE_CAP_ROUND,
	LSM_SVG_LINE_CAP_SQUARE
} LsmSvgLineCap;

const char * 		lsm_svg_line_cap_to_string 		(LsmSvgLineCap line_cap);
LsmSvgLineCap		lsm_svg_line_cap_from_string 		(const char *string);

typedef enum {
	LSM_SVG_TRANSFORM_TYPE_UNKNOWN,
	LSM_SVG_TRANSFORM_TYPE_MATRIX,
	LSM_SVG_TRANSFORM_TYPE_TRANSLATE,
	LSM_SVG_TRANSFORM_TYPE_SCALE,
	LSM_SVG_TRANSFORM_TYPE_ROTATE,
	LSM_SVG_TRANSFORM_TYPE_SKEW_X,
	LSM_SVG_TRANSFORM_TYPE_SKEW_Y
} LsmSvgTransformType;

typedef enum {
	LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE,
	LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX
} LsmSvgPatternUnits;

const char * 		lsm_svg_pattern_units_to_string 		(LsmSvgPatternUnits units);
LsmSvgPatternUnits	lsm_svg_pattern_units_from_string		(const char *string);

typedef enum {
	LSM_SVG_SPREAD_METHOD_PAD,
	LSM_SVG_SPREAD_METHOD_REFLECT,
	LSM_SVG_SPREAD_METHOD_REPEAT
} LsmSvgSpreadMethod;

const char * 		lsm_svg_spread_method_to_string 		(LsmSvgSpreadMethod method);
LsmSvgSpreadMethod	lsm_svg_spread_method_from_string		(const char *string);

G_END_DECLS

#endif
