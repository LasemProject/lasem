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

#ifndef LSM_MATHML_STRING_ELEMENT_H
#define LSM_MATHML_STRING_ELEMENT_H

#include <lsmmathml.h>
#include <lsmmathmlpresentationtoken.h>

G_BEGIN_DECLS

#define LSM_MATHML_TYPE_STRING_ELEMENT             (lsm_mathml_string_element_get_type ())
#define LSM_MATHML_STRING_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_MATHML_TYPE_STRING_ELEMENT, LsmMathmlStringElement))
#define LSM_MATHML_STRING_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_MATHML_TYPE_STRING_ELEMENT, LsmMathmlStringElementClass))
#define LSM_MATHML_IS_STRING_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_MATHML_TYPE_STRING_ELEMENT))
#define LSM_MATHML_IS_STRING_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_MATHML_TYPE_STRING_ELEMENT))
#define LSM_MATHML_STRING_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_MATHML_TYPE_STRING_ELEMENT, LsmMathmlStringElementClass))

typedef struct _GMathmlStringElementClass LsmMathmlStringElementClass;

struct _GMathmlStringElement {
	LsmMathmlPresentationToken presentation_token;

	LsmDomStringAttribute left_quote;
	LsmDomStringAttribute right_quote;
};

struct _GMathmlStringElementClass {
	LsmMathmlPresentationTokenClass parent_class;
};

GType lsm_mathml_string_element_get_type (void);

LsmDomNode *	lsm_mathml_string_element_new 	(void);

G_END_DECLS

#endif
