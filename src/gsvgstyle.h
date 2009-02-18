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

#ifndef GSVG_STYLE_H
#define GSVG_STYLE_H

#include <gsvg.h>
#include <gsvgattributebags.h>

G_BEGIN_DECLS

struct _GSvgStyle {
	struct {
		GSvgPaint paint;
		GSvgFillRule rule;
		double opacity;
	} fill;

	struct {
		GSvgPaint paint;
		GSvgLength width;
	} stroke;
};

GSvgStyle * 	gsvg_style_new 		(void);
void 		gsvg_style_free 	(GSvgStyle *style);
GSvgStyle *	gsvg_style_duplicate 	(const GSvgStyle *style);

G_END_DECLS

#endif
