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

#ifndef LSM_SVG_STYLE_H
#define LSM_SVG_STYLE_H

#include <lsmsvg.h>
#include <lsmsvgattributebags.h>

G_BEGIN_DECLS

struct _LsmSvgStyle {
	LsmSvgColor color;

	double resolution_ppi;

	struct {
		double width;
		double height;
		double diagonal;
		double horizontal_scale;
		double vertical_scale;
		double diagonal_scale;
	} viewport;

	struct {
		LsmSvgPaint paint;
		LsmSvgFillRule rule;
		double opacity;
	} fill;

	struct {
		LsmSvgPaint paint;
		LsmSvgLength width;
		LsmSvgLineJoin line_join;
		LsmSvgLineCap line_cap;
		double opacity;
		double miter_limit;
		LsmSvgDashArray *dash_array;
		LsmSvgLength dash_offset;
	} stroke;

	struct {
		char *font_family;
		LsmSvgLength font_size;
	} text;

	struct {
		LsmSvgColor color;
		double opacity;
	} stop;
};

LsmSvgStyle * 	lsm_svg_style_new 		(void);
void 		lsm_svg_style_free 		(LsmSvgStyle *style);
LsmSvgStyle *	lsm_svg_style_duplicate 	(const LsmSvgStyle *style);

G_END_DECLS

#endif
