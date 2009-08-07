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

#ifndef LSM_SVG_TRAITS_H
#define LSM_SVG_TRAITS_H

#include <lsmtraits.h>
#include <lsmsvglength.h>
#include <lsmsvgmatrix.h>

G_BEGIN_DECLS

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
	char *uri;
	LsmSvgColor color;
} LsmSvgPaint;

typedef struct {
	gboolean defer;
	LsmSvgAlign align;
	LsmSvgMeetOrSlice meet_or_slice;
} LsmSvgPreserveAspectRatio;

extern const LsmTraitClass lsm_svg_length_trait_class;
extern const LsmTraitClass lsm_svg_matrix_trait_class;
extern const LsmTraitClass lsm_svg_paint_trait_class;
extern const LsmTraitClass lsm_svg_fill_rule_trait_class;
extern const LsmTraitClass lsm_svg_line_join_trait_class;
extern const LsmTraitClass lsm_svg_line_cap_trait_class;
extern const LsmTraitClass lsm_svg_dash_array_trait_class;
extern const LsmTraitClass lsm_svg_color_trait_class;
extern const LsmTraitClass lsm_svg_pattern_units_trait_class;
extern const LsmTraitClass lsm_svg_preserve_aspect_ratio_trait_class;
extern const LsmTraitClass lsm_svg_spread_method_trait_class;

G_END_DECLS

#endif
