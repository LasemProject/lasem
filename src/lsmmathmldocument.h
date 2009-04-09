/* lsmmathmldocument.h
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

#ifndef LSM_MATHML_DOCUMENT_H
#define LSM_MATHML_DOCUMENT_H

#include <lsmmathml.h>
#include <lsmdomdocument.h>

G_BEGIN_DECLS

#define LSM_MATHML_TYPE_DOCUMENT             (lsm_mathml_document_get_type ())
#define LSM_MATHML_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_MATHML_TYPE_DOCUMENT, LsmMathmlDocument))
#define LSM_MATHML_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_MATHML_TYPE_DOCUMENT, LsmMathmlDocumentClass))
#define LSM_MATHML_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_MATHML_TYPE_DOCUMENT))
#define LSM_MATHML_IS_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_MATHML_TYPE_DOCUMENT))
#define LSM_MATHML_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_MATHML_TYPE_DOCUMENT, LsmMathmlDocumentClass))

typedef struct _GMathmlDocumentClass LsmMathmlDocumentClass;

struct _GMathmlDocument {
	LsmDomDocument	document;
};

struct _GMathmlDocumentClass {
	LsmDomDocumentClass  parent_class;
};

GType lsm_mathml_document_get_type (void);

LsmMathmlDocument *	lsm_mathml_document_new 			(void);
LsmMathmlMathElement * 	lsm_mathml_document_get_root_element 	(const LsmMathmlDocument *document);

LsmMathmlDocument * 	lsm_mathml_document_new_from_itex		(const char *itex);

G_END_DECLS

#endif

