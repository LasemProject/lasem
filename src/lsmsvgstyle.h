/* Lasem - SVG and Mathml library
 *
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

#ifndef LSM_SVG_STYLE_H
#define LSM_SVG_STYLE_H

#include <lsmsvgtraits.h>
#include <lsm.h>

G_BEGIN_DECLS

typedef struct {
	LsmProperty base;
	double value;
} LsmSvgDoubleProperty;

typedef struct {
	LsmProperty base;
	LsmSvgLength length;
} LsmSvgLengthProperty;

typedef struct {
	LsmProperty base;
	LsmSvgPaint paint;
} LsmSvgPaintProperty;

typedef struct {
	LsmProperty base;
	LsmSvgColor value;
} LsmSvgColorProperty;

typedef struct {
	LsmProperty base;
	LsmSvgMatrix matrix;
} LsmSvgTransformProperty;

typedef struct {
	LsmProperty base;
	LsmSvgFillRule value;
} LsmSvgFillRuleProperty;

typedef struct {
	LsmProperty base;
	LsmSvgFontStretch value;
} LsmSvgFontStretchProperty;

typedef struct {
	LsmProperty base;
	LsmSvgFontStyle value;
} LsmSvgFontStyleProperty;

typedef struct {
	LsmProperty base;
	LsmSvgFontWeight value;
} LsmSvgFontWeightProperty;

typedef struct {
	LsmProperty base;
	LsmSvgLineJoin value;
} LsmSvgLineJoinProperty;

typedef struct {
	LsmProperty base;
	LsmSvgLineCap value;
} LsmSvgLineCapProperty;

typedef struct {
	LsmProperty base;
	LsmSvgDashArray value;
} LsmSvgDashArrayProperty;

typedef struct {
	LsmProperty base;
	LsmSvgTextAnchor value;
} LsmSvgTextAnchorProperty;

typedef struct {
	LsmProperty base;
	LsmSvgDisplay value;
} LsmSvgDisplayProperty;

typedef struct {
	LsmProperty base;
	LsmSvgVisibility value;
} LsmSvgVisibilityProperty;

struct _LsmSvgStyle {
	/* Not inherited */

	LsmProperty *			alignment_baseline;
	LsmProperty *			baseline_shift;
	LsmProperty *	 		clip;
	LsmProperty *	 		clip_path;
	LsmProperty *	 		dominant_baseline;
	LsmProperty *	 		enable_background;
	LsmProperty *	 		filter;
	LsmProperty *	 		flood_color;
	LsmProperty *	 		flood_opacity;
	LsmProperty *			lighting_color;
	LsmProperty *			mask;
	LsmSvgDoubleProperty *		opacity;
	LsmProperty *			overflow;
	LsmSvgColorProperty *		stop_color;
	LsmSvgDoubleProperty *		stop_opacity;
	LsmProperty *			text_decoration;
	LsmProperty *			unicode_bidi;

	/* Inherited */

	LsmSvgFillRuleProperty * 	clip_rule;
	LsmSvgColorProperty *		color;
	LsmProperty *			color_interpolation;
	LsmProperty *			color_interpolation_filters;
	LsmProperty *			color_profile;
	LsmProperty *			color_rendering;
	LsmProperty *			cursor;
	LsmProperty *			direction;
	LsmSvgDisplayProperty *		display;
	LsmSvgPaintProperty *		fill;
	LsmSvgDoubleProperty *		fill_opacity;
	LsmSvgFillRuleProperty *	fill_rule;
	LsmProperty *			font;
	LsmProperty *			font_family;
	LsmSvgLengthProperty *		font_size;
	LsmProperty *			font_size_adjust;
	LsmSvgFontStretchProperty *	font_stretch;
	LsmSvgFontStyleProperty *	font_style;
	LsmProperty *			font_variant;
	LsmSvgFontWeightProperty *	font_weight;
	LsmProperty *			glyph_orientation_horizontal;
	LsmProperty *			glyph_orientation_vertical;
	LsmProperty *			image_rendering;
	LsmProperty *			kerning;
	LsmProperty *			letter_spacing;
	LsmProperty *			marker;
	LsmProperty *			marker_end;
	LsmProperty *			marker_mid;
	LsmProperty *			marker_start;
	LsmProperty *			pointer_events;
	LsmProperty *			shape_rendering;
	LsmSvgPaintProperty *	 	stroke;
	LsmSvgDashArrayProperty	*	stroke_dash_array;
	LsmSvgLengthProperty *		stroke_dash_offset;
	LsmSvgLineCapProperty *		stroke_line_cap;
	LsmSvgLineJoinProperty *	stroke_line_join;
	LsmSvgDoubleProperty *		stroke_miter_limit;
	LsmSvgDoubleProperty *		stroke_opacity;
	LsmSvgLengthProperty *		stroke_width;
	LsmSvgTextAnchorProperty *	text_anchor;
	LsmProperty *			text_rendering;
	LsmSvgVisibilityProperty *	visibility;
	LsmProperty *			word_spacing;
	LsmProperty *			writing_mode;
};

void 		lsm_svg_property_bag_set_property 	(LsmPropertyBag *property_bag,
							 const char *name, const char *value);
const char *	lsm_svg_property_bag_get_property	(LsmPropertyBag *property_bag,
							 const char *name);
void 		lsm_svg_property_bag_clean 		(LsmPropertyBag *property_bag);
char * 		lsm_svg_property_bag_serialize 		(LsmPropertyBag *property_bag);

LsmSvgStyle * 		lsm_svg_style_new 			(void);
LsmSvgStyle *		lsm_svg_style_ref			(LsmSvgStyle *style);
void			lsm_svg_style_unref			(LsmSvgStyle *style);
LsmSvgStyle *		lsm_svg_style_new_inherited 		(const LsmSvgStyle *parent_style,
								 LsmPropertyBag *property_bag);

G_END_DECLS

#endif
