/*
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

#ifndef GMATHML_STRING_ELEMENT_H
#define GMATHML_STRING_ELEMENT_H

#include <gmathml.h>
#include <gmathmlpresentationtoken.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_STRING_ELEMENT             (gmathml_string_element_get_type ())
#define GMATHML_STRING_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_STRING_ELEMENT, GMathmlStringElement))
#define GMATHML_STRING_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_STRING_ELEMENT, GMathmlStringElementClass))
#define GMATHML_IS_STRING_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_STRING_ELEMENT))
#define GMATHML_IS_STRING_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_STRING_ELEMENT))
#define GMATHML_STRING_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_STRING_ELEMENT, GMathmlStringElementClass))

typedef struct _GMathmlStringElementClass GMathmlStringElementClass;

struct _GMathmlStringElement {
	GMathmlPresentationToken presentation_token;

	GDomStringAttribute left_quote;
	GDomStringAttribute right_quote;
};

struct _GMathmlStringElementClass {
	GMathmlPresentationTokenClass parent_class;
};

GType gmathml_string_element_get_type (void);

GDomNode *	gmathml_string_element_new 	(void);

G_END_DECLS

#endif
