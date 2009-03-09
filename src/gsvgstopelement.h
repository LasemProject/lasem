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

#ifndef GSVG_STOP_ELEMENT_H
#define GSVG_STOP_ELEMENT_H

#include <gsvg.h>
#include <gsvggraphic.h>

G_BEGIN_DECLS

#define GSVG_TYPE_STOP_ELEMENT             (gsvg_stop_element_get_type ())
#define GSVG_STOP_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_STOP_ELEMENT, GSvgStopElement))
#define GSVG_STOP_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_STOP_ELEMENT, GSvgStopElementClass))
#define GSVG_IS_STOP_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_STOP_ELEMENT))
#define GSVG_IS_STOP_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_STOP_ELEMENT))
#define GSVG_STOP_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_STOP_ELEMENT, GSvgStopElementClass))

typedef struct _GSvgStopElementClass GSvgStopElementClass;

struct _GSvgStopElement {
	GSvgGraphic graphic;

	GSvgLengthAttribute offset;
};

struct _GSvgStopElementClass {
	GSvgGraphicClass  parent_class;
};

GType gsvg_stop_element_get_type (void);

GDomNode * gsvg_stop_element_new (void);

double 			gsvg_stop_element_get_offset 	(GSvgStopElement *self);
const GSvgColor *	gsvg_stop_element_get_color 	(GSvgStopElement *self);
double 			gsvg_stop_element_get_opacity 	(GSvgStopElement *self);

G_END_DECLS

#endif
