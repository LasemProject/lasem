/* gdomelement.h
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

#ifndef GDOM_ELEMENT_H
#define GDOM_ELEMENT_H

#include <gdom.h>
#include <gdomnode.h>

G_BEGIN_DECLS

#define GDOM_TYPE_ELEMENT             (gdom_element_get_type ())
#define GDOM_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_ELEMENT, GDomElement))
#define GDOM_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_ELEMENT, GDomElementClass))
#define GDOM_IS_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_ELEMENT))
#define GDOM_IS_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_ELEMENT))
#define GDOM_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_ELEMENT, GDomElementClass))

typedef struct _GDomElementClass GDomElementClass;

struct _GDomElement {
	GDomNode node;
};

struct _GDomElementClass {
	GDomNodeClass parent_class;

	const char* 	(*get_attribute) (GDomElement* self, const char* name);
	void 		(*set_attribute) (GDomElement* self, const char* name, const char* attribute_value);
};

GType gdom_element_get_type (void);

const char* 	gdom_element_get_attribute 	(GDomElement* self, const char* name);
void 		gdom_element_set_attribute 	(GDomElement* self, const char* name, const char* attribute_value);

G_END_DECLS

#endif

