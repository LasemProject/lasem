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

#ifndef GSVG_GRAPHIC_H
#define GSVG_GRAPHIC_H

#include <gsvg.h>
#include <gsvgelement.h>

G_BEGIN_DECLS

#define GSVG_TYPE_GRAPHIC             (gsvg_graphic_get_type ())
#define GSVG_GRAPHIC(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_GRAPHIC, GSvgGraphic))
#define GSVG_GRAPHIC_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_GRAPHIC, GSvgGraphicClass))
#define GSVG_IS_GRAPHIC(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_GRAPHIC))
#define GSVG_IS_GRAPHIC_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_GRAPHIC))
#define GSVG_GRAPHIC_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_GRAPHIC, GSvgGraphicClass))

typedef struct _GSvgGraphicClass GSvgGraphicClass;

struct _GSvgGraphic {
	GSvgElement	element;

	GDomAttribute	class_name;
	GDomAttribute	style;

	GSvgFillAttributeBag *fill;
	GSvgStrokeAttributeBag *stroke;
	GSvgTransformAttributeBag *transform;
	GSvgTextAttributeBag *text;
};

struct _GSvgGraphicClass {
	GSvgElementClass  parent_class;

	void 			(*graphic_render)	(GSvgElement *element, GSvgView *view);
};

GType gsvg_graphic_get_type (void);

G_END_DECLS

#endif

