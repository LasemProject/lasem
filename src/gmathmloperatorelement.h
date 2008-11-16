/* gmathmloperatorelement.h
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

#ifndef GMATHML_OPERATOR_ELEMENT_H
#define GMATHML_OPERATOR_ELEMENT_H

#include <gmathml.h>
#include <gmathmlpresentationtoken.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_OPERATOR_ELEMENT             (gmathml_operator_element_get_type ())
#define GMATHML_OPERATOR_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_OPERATOR_ELEMENT, GMathmlOperatorElement))
#define GMATHML_OPERATOR_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_OPERATOR_ELEMENT, GMathmlOperatorElementClass))
#define GMATHML_IS_OPERATOR_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_OPERATOR_ELEMENT))
#define GMATHML_IS_OPERATOR_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_OPERATOR_ELEMENT))
#define GMATHML_OPERATOR_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_OPERATOR_ELEMENT, GMathmlOperatorElementClass))

typedef struct _GMathmlOperatorElementClass GMathmlOperatorElementClass;

struct _GMathmlOperatorElement {
	GMathmlPresentationToken presentation_token;

	GMathmlAttributeNamed form;
	GMathmlAttributeBoolean fence;
	GMathmlAttributeBoolean separator;
	GMathmlAttributeSpace left_space;
	GMathmlAttributeSpace right_space;
	GMathmlAttributeBoolean stretchy;
	GMathmlAttributeBoolean symmetric;
	GMathmlAttributeSpace max_size;
	GMathmlAttributeSpace min_size;
	GMathmlAttributeBoolean large_op;
	GMathmlAttributeBoolean movable_limits;
	GMathmlAttributeBoolean accent;

	gboolean is_large_op;
};

struct _GMathmlOperatorElementClass {
	GMathmlPresentationTokenClass parent_class;
};

GType gmathml_operator_element_get_type (void);

GDomNode *	gmathml_operator_element_new 	(void);

double 		gmathml_operator_get_slant 	(const GMathmlOperatorElement *operator_element,
						 GMathmlView *view);

G_END_DECLS

#endif
