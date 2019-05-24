/* Lasem
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_SVG_TRAITS_H
#define LSM_SVG_TRAITS_H

#include <lsm.h>
#include <lsmsvglength.h>
#include <lsmsvgmatrix.h>

G_BEGIN_DECLS

typedef struct {
	unsigned int n_values;
	double *values;
} LsmSvgVector;

typedef struct {
	LsmSvgAngleType type;
	double angle;
} LsmSvgAngle;

typedef struct {
	double red;
	double green;
	double blue;
} LsmSvgColor;

extern const LsmSvgColor lsm_svg_color_null;

typedef struct {
	unsigned int n_dashes;
	LsmSvgLength *dashes;
} LsmSvgDashArray;

extern const LsmSvgDashArray lsm_svg_dash_array_null;

LsmSvgDashArray * 	lsm_svg_dash_array_new 		(unsigned int n_dashes);
void 			lsm_svg_dash_array_free 	(LsmSvgDashArray *array);
LsmSvgDashArray * 	lsm_svg_dash_array_duplicate 	(const LsmSvgDashArray *origin);
void			lsm_svg_dash_array_copy		(LsmSvgDashArray *to, const LsmSvgDashArray *from);

typedef struct {
	LsmSvgPaintType type;
	char *url;
	LsmSvgColor color;
} LsmSvgPaint;

typedef struct {
	gboolean defer;
	LsmSvgAlign align;
	LsmSvgMeetOrSlice meet_or_slice;
} LsmSvgPreserveAspectRatio;

typedef struct {
	double a;
	double b;
} LsmSvgOneOrTwoDouble;

typedef struct {
	int a;
	int b;
} LsmSvgOneOrTwoInteger;

extern const LsmTraitClass lsm_svg_angle_trait_class;
extern const LsmTraitClass lsm_svg_blending_mode_trait_class;
extern const LsmTraitClass lsm_svg_enable_background_trait_class;
extern const LsmTraitClass lsm_svg_channel_selector_trait_class;
extern const LsmTraitClass lsm_svg_color_trait_class;
extern const LsmTraitClass lsm_svg_color_filter_type_trait_class;
extern const LsmTraitClass lsm_svg_comp_op_trait_class;
extern const LsmTraitClass lsm_svg_dash_array_trait_class;
extern const LsmTraitClass lsm_svg_display_trait_class;
extern const LsmTraitClass lsm_svg_edge_mode_trait_class;
extern const LsmTraitClass lsm_svg_fill_rule_trait_class;
extern const LsmTraitClass lsm_svg_font_stretch_trait_class;
extern const LsmTraitClass lsm_svg_font_style_trait_class;
extern const LsmTraitClass lsm_svg_font_weight_trait_class;
extern const LsmTraitClass lsm_svg_one_or_two_integer_trait_class;
extern const LsmTraitClass lsm_svg_length_trait_class;
extern const LsmTraitClass lsm_svg_length_list_trait_class;
extern const LsmTraitClass lsm_svg_line_join_trait_class;
extern const LsmTraitClass lsm_svg_line_cap_trait_class;
extern const LsmTraitClass lsm_svg_marker_units_trait_class;
extern const LsmTraitClass lsm_svg_matrix_trait_class;
extern const LsmTraitClass lsm_svg_morphology_operator_trait_class;
extern const LsmTraitClass lsm_svg_one_or_two_double_trait_class;
extern const LsmTraitClass lsm_svg_overflow_trait_class;
extern const LsmTraitClass lsm_svg_paint_trait_class;
extern const LsmTraitClass lsm_svg_pattern_units_trait_class;
extern const LsmTraitClass lsm_svg_preserve_aspect_ratio_trait_class;
extern const LsmTraitClass lsm_svg_spread_method_trait_class;
extern const LsmTraitClass lsm_svg_stitch_tiles_trait_class;
extern const LsmTraitClass lsm_svg_text_anchor_trait_class;
extern const LsmTraitClass lsm_svg_turbulence_type_trait_class;
extern const LsmTraitClass lsm_svg_vector_trait_class;
extern const LsmTraitClass lsm_svg_visibility_trait_class;
extern const LsmTraitClass lsm_svg_writing_mode_trait_class;

G_END_DECLS

#endif
