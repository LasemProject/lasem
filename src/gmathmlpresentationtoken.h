/* gmathmlpresentationtoken.h
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

#ifndef GMATHML_PRESENTATION_TOKEN_H
#define GMATHML_PRESENTATION_TOKEN_H

#include <gmathml.h>
#include <gmathmlelement.h>

G_BEGIN_DECLS

typedef enum {
	GMATHML_PRESENTATION_TOKEN_TYPE_NUMBER,
	GMATHML_PRESENTATION_TOKEN_TYPE_IDENTIFIER,
	GMATHML_PRESENTATION_TOKEN_TYPE_TEXT
} GMathmlPresentationTokenType;

#define GMATHML_TYPE_PRESENTATION_TOKEN             (gmathml_presentation_token_get_type ())
#define GMATHML_PRESENTATION_TOKEN(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_PRESENTATION_TOKEN, GMathmlPresentationToken))
#define GMATHML_PRESENTATION_TOKEN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_PRESENTATION_TOKEN, GMathmlPresentationTokenClass))
#define GMATHML_IS_PRESENTATION_TOKEN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_PRESENTATION_TOKEN))
#define GMATHML_IS_PRESENTATION_TOKEN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_PRESENTATION_TOKEN))
#define GMATHML_PRESENTATION_TOKEN_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_PRESENTATION_TOKEN, GMathmlPresentationTokenClass))

typedef struct _GMathmlPresentationTokenClass GMathmlPresentationTokenClass;

struct _GMathmlPresentationToken {
	GMathmlElement	element;

	GMathmlPresentationTokenType type;

	GDomAttributeString	math_family;
	GDomAttributeNamed	math_variant;
	GMathmlAttributeLength	math_size;
	GMathmlAttributeColor	math_color;
	GMathmlAttributeColor	math_background;

	/* Deprecated style attributes */

	GDomAttributeNamed font_weight;
	GDomAttributeNamed font_style;
};

struct _GMathmlPresentationTokenClass {
	GMathmlElementClass  parent_class;

	char * (*get_text)	(GMathmlPresentationToken *self);
};

GType gmathml_presentation_token_get_type (void);

GDomNode *	gmathml_number_element_new (void);
GDomNode *	gmathml_identifier_element_new (void);
GDomNode *	gmathml_text_element_new (void);

/* Extra functions */

char *		gmathml_presentation_token_get_text 	(GMathmlPresentationToken *self);

G_END_DECLS

#endif

