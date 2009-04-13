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

#ifndef LSM_SVG_GRAPHIC_H
#define LSM_SVG_GRAPHIC_H

#include <lsmsvg.h>
#include <lsmsvgelement.h>

G_BEGIN_DECLS

#define LSM_SVG_TYPE_GRAPHIC             (lsm_svg_graphic_get_type ())
#define LSM_SVG_GRAPHIC(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_SVG_TYPE_GRAPHIC, LsmSvgGraphic))
#define LSM_SVG_GRAPHIC_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_SVG_TYPE_GRAPHIC, LsmSvgGraphicClass))
#define LSM_SVG_IS_GRAPHIC(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_SVG_TYPE_GRAPHIC))
#define LSM_SVG_IS_GRAPHIC_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_SVG_TYPE_GRAPHIC))
#define LSM_SVG_GRAPHIC_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_SVG_TYPE_GRAPHIC, LsmSvgGraphicClass))

typedef struct _GSvgGraphicClass LsmSvgGraphicClass;

struct _GSvgGraphic {
	LsmSvgElement	element;

	LsmDomAttribute	class_name;
	LsmDomAttribute	style;

	LsmSvgColorAttribute color;

	LsmSvgFillAttributeBag *fill;
	LsmSvgStrokeAttributeBag *stroke;
	LsmSvgTransformAttributeBag *transform;
	LsmSvgTextAttributeBag *text;
	LsmSvgStopAttributeBag *stop;
};

struct _GSvgGraphicClass {
	LsmSvgElementClass  parent_class;

	void 			(*graphic_render)	(LsmSvgElement *element, LsmSvgView *view);
};

GType lsm_svg_graphic_get_type (void);

G_END_DECLS

#endif

