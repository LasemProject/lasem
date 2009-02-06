/* gmathmldocument.h
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

#ifndef GMATHML_DOCUMENT_H
#define GMATHML_DOCUMENT_H

#include <gmathml.h>
#include <gdomdocument.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_DOCUMENT             (gmathml_document_get_type ())
#define GMATHML_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_DOCUMENT, GMathmlDocument))
#define GMATHML_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_DOCUMENT, GMathmlDocumentClass))
#define GMATHML_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_DOCUMENT))
#define GMATHML_IS_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_DOCUMENT))
#define GMATHML_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_DOCUMENT, GMathmlDocumentClass))

typedef struct _GMathmlDocumentClass GMathmlDocumentClass;

struct _GMathmlDocument {
	GDomDocument	document;
};

struct _GMathmlDocumentClass {
	GDomDocumentClass  parent_class;
};

GType gmathml_document_get_type (void);

GMathmlDocument *	gmathml_document_new 			(void);
GMathmlMathElement * 	gmathml_document_get_math_element 	(const GMathmlDocument *document);

GMathmlDocument * 	gmathml_document_new_from_itex		(const char *itex);

G_END_DECLS

#endif

