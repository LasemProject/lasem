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

#ifndef GSVG_ENUMS_H
#define GSVG_ENUMS_H

#include <gsvg.h>

G_BEGIN_DECLS

typedef enum {
	GSVG_PAINT_TYPE_UNKNOWN = 0,
	GSVG_PAINT_TYPE_RGB_COLOR,
	GSVG_PAINT_TYPE_RGB_COLOR_ICC_COLOR,
	GSVG_PAINT_TYPE_NONE = 101,
	GSVG_PAINT_TYPE_CURRENT_COLOR,
	GSVG_PAINT_TYPE_URI_NONE,
	GSVG_PAINT_TYPE_URI_CURRENT_COLOR,
	GSVG_PAINT_TYPE_URI_RGB_COLOR,
	GSVG_PAINT_TYPE_URI_RGB_COLOR_ICC_COLOR,
	GSVG_PAINT_TYPE_URI
} GSvgPaintType;

typedef enum {
	GSVG_LENGTH_TYPE_UNKNOWN,
	GSVG_LENGTH_TYPE_NUMBER,
	GSVG_LENGTH_TYPE_PERCENTAGE,
	GSVG_LENGTH_TYPE_EMS,
	GSVG_LENGTH_TYPE_EXS,
	GSVG_LENGTH_TYPE_PX,
	GSVG_LENGTH_TYPE_CM,
	GSVG_LENGTH_TYPE_MM,
	GSVG_LENGTH_TYPE_IN,
	GSVG_LENGTH_TYPE_PT,
	GSVG_LENGTH_TYPE_PC
} GSvgLengthType;

const char * 		gsvg_length_type_to_string 		(GSvgLengthType length_type);
GSvgLengthType 		gsvg_length_type_from_string 		(const char *string);

typedef enum {
	GSVG_FILL_RULE_NON_ZERO,
	GSVG_FILL_RULE_EVEN_ODD
} GSvgFillRule;

const char * 		gsvg_fill_rule_to_string 		(GSvgFillRule fill_rule);
GSvgFillRule 		gsvg_fill_rule_from_string 		(const char *string);

G_END_DECLS

#endif
