/* gmathmlelement.h
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#ifndef GMATHML_ELEMENT_H
#define GMATHML_ELEMENT_H

#include <gmathml.h>
#include <gmathmlutils.h>
#include <gmathmlattributes.h>
#include <gdomelement.h>
#include <cairo.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_ELEMENT             (gmathml_element_get_type ())
#define GMATHML_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_ELEMENT, GMathmlElement))
#define GMATHML_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_ELEMENT, GMathmlElementClass))
#define GMATHML_IS_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_ELEMENT))
#define GMATHML_IS_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_ELEMENT))
#define GMATHML_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_ELEMENT, GMathmlElementClass))

typedef struct _GMathmlElementClass GMathmlElementClass;

struct _GMathmlElement {
	GDomElement	element;

	GMathmlElementAttributes element_attrs;

	/* View */

	gboolean measure_done;

	double x, y;
	GMathmlBbox bbox;
};

struct _GMathmlElementClass {
	GDomElementClass  parent_class;

	GMathmlAttributeMap *attributes;

	void			(*update_attributes)	(GMathmlElement *element);

	const GMathmlBbox * 	(*measure) 		(GMathmlElement *element, GMathmlView *view);
	void 			(*layout) 		(GMathmlElement *element, GMathmlView *view,
							 double x, double y, const GMathmlBbox *bbox);
	void 			(*render)		(GMathmlElement *element, GMathmlView *view);
};

GType gmathml_element_get_type (void);

void 			gmathml_element_update_attributes 	(GMathmlElement *element);

const GMathmlBbox *	gmathml_element_measure		(GMathmlElement *element, GMathmlView *view);
void 			gmathml_element_layout 		(GMathmlElement *element, GMathmlView *view,
							 double x, double y, const GMathmlBbox *bbox);
void 			gmathml_element_render 		(GMathmlElement *element, GMathmlView *view);

void 			gmathml_element_class_add_element_attributes 	(GMathmlElementClass *m_element_class);

G_END_DECLS

#endif
