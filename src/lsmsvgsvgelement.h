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

#ifndef LSM_SVG_SVG_ELEMENT_H
#define LSM_SVG_SVG_ELEMENT_H

#include <lsmsvg.h>
#include <lsmsvggraphic.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_SVG_ELEMENT             (lsm_svg_svg_element_get_type ())
#define LSM_SVG_SVG_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_SVG_ELEMENT, LsmSvgSvgElement))
#define LSM_SVG_SVG_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_SVG_ELEMENT, LsmSvgSvgElementClass))
#define LSM_IS_SVG_SVG_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_SVG_ELEMENT))
#define LSM_IS_SVG_SVG_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_SVG_ELEMENT))
#define LSM_SVG_SVG_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_SVG_ELEMENT, LsmSvgSvgElementClass))

typedef struct _LsmSvgSvgElementClass LsmSvgSvgElementClass;

struct _LsmSvgSvgElement {
	LsmSvgGraphic graphic;

	LsmSvgStyle *default_style;

	LsmSvgLengthAttribute	x;
	LsmSvgLengthAttribute	y;
	LsmSvgLengthAttribute	width;
	LsmSvgLengthAttribute	height;

	LsmSvgViewboxAttribute			viewbox;
	LsmSvgPreserveAspectRatioAttribute	preserve_aspect_ratio;

	LsmBox svg_box;
};

struct _LsmSvgSvgElementClass {
	LsmSvgGraphicClass  parent_class;
};

GType lsm_svg_svg_element_get_type (void);

LsmDomNode *		lsm_svg_svg_element_new 		(void);

LsmSvgStyle * 		lsm_svg_svg_element_get_default_style 	(LsmSvgSvgElement *svg_element);
void 			lsm_svg_svg_element_update 		(LsmSvgSvgElement *svg_element);
void 			lsm_svg_svg_element_measure 		(LsmSvgSvgElement *self, double *width, double *height);
void 			lsm_svg_svg_element_render 		(LsmSvgSvgElement *svg_element, LsmSvgView *view);

G_END_DECLS

#endif
