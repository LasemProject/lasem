/* Lasem - SVG and Mathml library
 *
 * Copyright © 2007-2008 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_DOM_ELEMENT_H
#define LSM_DOM_ELEMENT_H

#include <lsmdom.h>
#include <lsmdomnode.h>

G_BEGIN_DECLS

#define LSM_TYPE_DOM_ELEMENT             (lsm_dom_element_get_type ())
#define LSM_DOM_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_ELEMENT, LsmDomElement))
#define LSM_DOM_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_ELEMENT, LsmDomElementClass))
#define LSM_IS_DOM_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_ELEMENT))
#define LSM_IS_DOM_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_ELEMENT))
#define LSM_DOM_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_ELEMENT, LsmDomElementClass))

typedef struct _LsmDomElementClass LsmDomElementClass;

struct _LsmDomElement {
	LsmDomNode node;
};

struct _LsmDomElementClass {
	LsmDomNodeClass parent_class;

	const char* 	(*get_attribute) (LsmDomElement *self, const char *name);
	void 		(*set_attribute) (LsmDomElement *self, const char *name, const char *attribute_value);
	char *		(*get_serialized_attributes)	(LsmDomElement *self);
};

GType lsm_dom_element_get_type (void);

const char * 	lsm_dom_element_get_tag_name 	(LsmDomElement *self);
const char* 	lsm_dom_element_get_attribute 	(LsmDomElement* self, const char* name);
void 		lsm_dom_element_set_attribute 	(LsmDomElement* self, const char* name, const char* attribute_value);

G_END_DECLS

#endif

