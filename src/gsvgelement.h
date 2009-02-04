/* gsvgelement.h
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef GSVG_ELEMENT_H
#define GSVG_ELEMENT_H

#include <gsvg.h>
#include <gsvgview.h>
#include <gsvgattributes.h>
#include <gdomelement.h>
#include <cairo.h>

G_BEGIN_DECLS

typedef struct {
	char *		font_family;
} GSvgStyle;

#define GSVG_TYPE_ELEMENT             (gsvg_element_get_type ())
#define GSVG_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_ELEMENT, GSvgElement))
#define GSVG_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_ELEMENT, GSvgElementClass))
#define GSVG_IS_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_ELEMENT))
#define GSVG_IS_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_ELEMENT))
#define GSVG_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_ELEMENT, GSvgElementClass))

typedef struct _GSvgElementClass GSvgElementClass;

struct _GSvgElement {
	GDomElement	element;

	GDomAttribute	*id;

	/* View */

	gboolean need_update;
};

struct _GSvgElementClass {
	GDomElementClass  parent_class;

	GDomAttributeMap *attributes;

	void			(*update)		(GSvgElement *element, GSvgStyle *style);
	void 			(*render)		(GSvgElement *element, GSvgView *view);
};

GType gsvg_element_get_type (void);

gboolean		gsvg_element_update 		(GSvgElement *element, const GSvgStyle *style);
void 			gsvg_element_render 		(GSvgElement *element, GSvgView *view);


G_END_DECLS

#endif
