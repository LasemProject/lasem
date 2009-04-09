/* lsmmathmlenums.c
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

#ifndef LSM_SVG_ATTRIBUTES_H
#define LSM_SVG_ATTRIBUTES_H

#include <lsmdomattributes.h>
#include <lsmsvg.h>
#include <lsmsvgenums.h>
#include <lsmsvgmatrix.h>

G_BEGIN_DECLS

typedef struct {
	double red;
	double green;
	double blue;
} LsmSvgColor;

extern const LsmSvgColor lsm_svg_color_null;

typedef struct {
	double value;
	double value_unit;
	LsmSvgLengthType type;
} LsmSvgLength;

typedef struct {
	LsmSvgLength base;
	LsmSvgLength animated;
} LsmSvgAnimatedLength;

typedef struct {
	LsmSvgPaintType type;
	char *uri;
	LsmSvgColor color;
} LsmSvgPaint;

typedef struct {
	double x;
	double y;
	double width;
	double height;
} LsmSvgViewBox;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgLength length;
	double value;
} LsmSvgLengthAttribute;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgAnimatedLength length;
	double value;
} LsmSvgAnimatedLengthAttribute;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgPaint paint;
} LsmSvgPaintAttribute;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgColor value;
} LsmSvgColorAttribute;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgViewBox value;
} LsmSvgViewBoxAttribute;

typedef struct {
	LsmDomAttribute attr;
	LsmSvgMatrix matrix;
} LsmSvgTransformAttribute;

void		lsm_svg_length_attribute_parse 		(LsmSvgLengthAttribute *attribute,
							 LsmSvgLength *default_value,
							 double font_size);
void 		lsm_svg_animated_length_attribute_parse 	(LsmSvgAnimatedLengthAttribute *attribute,
							 LsmSvgLength *default_value,
							 double font_size);
void 		lsm_svg_fill_rule_attribute_parse 		(LsmDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_svg_gradient_units_attribute_parse 	(LsmDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_svg_spread_method_attribute_parse 	(LsmDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_svg_paint_attribute_finalize 		(void *abstract);
void 		lsm_svg_paint_attribute_parse 		(LsmSvgPaintAttribute *attribute,
							 LsmSvgPaint *default_value);
void 		lsm_svg_color_attribute_parse 		(LsmSvgColorAttribute *attribute,
							 LsmSvgColor *default_value);
void		lsm_svg_view_box_attribute_parse		(LsmSvgViewBoxAttribute *attribute,
							 LsmSvgViewBox *default_value);
void		lsm_svg_transform_attribute_parse		(LsmSvgTransformAttribute *attribute);

G_END_DECLS

#endif
