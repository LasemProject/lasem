/* gmathmlunderoverelement.h
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

#ifndef GMATHML_UNDER_OVER_ELEMENT_H
#define GMATHML_UNDER_OVER_ELEMENT_H

#include <gmathml.h>
#include <gmathmlelement.h>

G_BEGIN_DECLS

typedef enum {
	GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER,
	GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER,
	GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER
} GMathmlUnderOverElementType;

#define GMATHML_TYPE_UNDER_OVER_ELEMENT             (gmathml_under_over_element_get_type ())
#define GMATHML_UNDER_OVER_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_UNDER_OVER_ELEMENT, GMathmlUnderOverElement))
#define GMATHML_UNDER_OVER_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_UNDER_OVER_ELEMENT, GMathmlUnderOverElementClass))
#define GMATHML_IS_UNDER_OVER_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_UNDER_OVER_ELEMENT))
#define GMATHML_IS_UNDER_OVER_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_UNDER_OVER_ELEMENT))
#define GMATHML_UNDER_OVER_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_UNDER_OVER_ELEMENT, GMathmlUnderOverElementClass))

typedef struct _GMathmlUnderOverElementClass GMathmlUnderOverElementClass;

struct _GMathmlUnderOverElement {
	GMathmlElement	element;

	GMathmlAttributeBoolean accent;
	GMathmlAttributeBoolean accent_under;

	GMathmlUnderOverElementType type;

	GMathmlElement *base;
	GMathmlElement *underscript;
	GMathmlElement *overscript;

	double under_space;
	double over_space;

	gboolean as_script;
	double subscript_offset;
	double superscript_offset;
};

struct _GMathmlUnderOverElementClass {
	GMathmlElementClass  parent_class;
};

GType gmathml_under_over_element_get_type (void);

GDomNode * 	gmathml_under_element_new 	(void);
GDomNode * 	gmathml_over_element_new 	(void);
GDomNode * 	gmathml_under_over_element_new 	(void);

G_END_DECLS

#endif

