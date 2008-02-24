/* gdomdocument.h
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

#ifndef GDOM_DOCUMENT_H
#define GDOM_DOCUMENT_H

#include <gdom.h>
#include <gdomnode.h>

G_BEGIN_DECLS

#define GDOM_TYPE_DOCUMENT             (gdom_document_get_type ())
#define GDOM_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_DOCUMENT, GDomDocument))
#define GDOM_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_DOCUMENT, GDomDocumentClass))
#define GDOM_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_DOCUMENT))
#define GDOM_IS_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_DOCUMENT))
#define GDOM_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_DOCUMENT, GDomDocumentClass))

typedef struct _GDomDocumentClass GDomDocumentClass;

struct _GDomDocument {
	GDomNode node;
};

struct _GDomDocumentClass {
	GDomNodeClass parent_class;

	GDomElement* 	(*get_document_element) (GDomDocument* self);
	GDomElement* 	(*create_element) (GDomDocument* self, const char* tag_name);
	GDomText* 	(*create_text_node) (GDomDocument* self, const char* data);
};

GType gdom_document_get_type (void);

GDomElement* 	gdom_document_get_document_element 	(GDomDocument* self);
GDomElement* 	gdom_document_create_element 		(GDomDocument* self, const char* tag_name);
GDomText* 	gdom_document_create_text_node 		(GDomDocument* self, const char* data);

G_END_DECLS

#endif
