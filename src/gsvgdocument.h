/*
 * Copyright © 2009 Emmanuel Pacaud
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

#ifndef GSVG_DOCUMENT_H
#define GSVG_DOCUMENT_H

#include <gsvg.h>
#include <gdomdocument.h>

G_BEGIN_DECLS

#define GSVG_TYPE_DOCUMENT             (gsvg_document_get_type ())
#define GSVG_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_DOCUMENT, GSvgDocument))
#define GSVG_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_DOCUMENT, GSvgDocumentClass))
#define GSVG_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_DOCUMENT))
#define GSVG_IS_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_DOCUMENT))
#define GSVG_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_DOCUMENT, GSvgDocumentClass))

typedef struct _GSvgDocumentClass GSvgDocumentClass;

struct _GSvgDocument {
	GDomDocument	document;
};

struct _GSvgDocumentClass {
	GDomDocumentClass  parent_class;
};

GType gsvg_document_get_type (void);

GDomNode *		gsvg_document_new 		(void);
GSvgSvgElement * 	gsvg_document_get_svg_element 	(const GSvgDocument *document);

G_END_DECLS

#endif

