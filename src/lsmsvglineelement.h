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

#ifndef LSM_SVG_LINE_ELEMENT_H
#define LSM_SVG_LINE_ELEMENT_H

#include <lsmsvg.h>
#include <lsmsvggraphic.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_LINE_ELEMENT             (lsm_svg_line_element_get_type ())
#define LSM_SVG_LINE_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_LINE_ELEMENT, LsmSvgLineElement))
#define LSM_SVG_LINE_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_LINE_ELEMENT, LsmSvgLineElementClass))
#define LSM_IS_SVG_LINE_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_LINE_ELEMENT))
#define LSM_IS_SVG_LINE_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_LINE_ELEMENT))
#define LSM_SVG_LINE_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_LINE_ELEMENT, LsmSvgLineElementClass))

typedef struct _LsmSvgLineElementClass LsmSvgLineElementClass;

struct _LsmSvgLineElement {
	LsmSvgGraphic graphic;

	LsmSvgAnimatedLengthAttribute	x1;
	LsmSvgAnimatedLengthAttribute	y1;
	LsmSvgAnimatedLengthAttribute	x2;
	LsmSvgAnimatedLengthAttribute	y2;
};

struct _LsmSvgLineElementClass {
	LsmSvgGraphicClass  parent_class;
};

GType lsm_svg_line_element_get_type (void);

LsmDomNode *		lsm_svg_line_element_new 		(void);

G_END_DECLS

#endif
