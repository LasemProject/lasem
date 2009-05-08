/* lsmdomdocument.h
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

#ifndef LSM_DOM_DOCUMENT_H
#define LSM_DOM_DOCUMENT_H

#include <lsmdom.h>
#include <lsmdomnode.h>
#include <lsmdomview.h>

G_BEGIN_DECLS

#define LSM_DOM_DOCUMENT_DEFAULT_RESOLUTION 		 72.0
#define LSM_DOM_DOCUMENT_DEFAULT_VIEWPORT_WIDTH		320.0
#define LSM_DOM_DOCUMENT_DEFAULT_VIEWPORT_HEIGHT 	200.0

#define LSM_TYPE_DOM_DOCUMENT             (lsm_dom_document_get_type ())
#define LSM_DOM_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_DOCUMENT, LsmDomDocument))
#define LSM_DOM_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_DOCUMENT, LsmDomDocumentClass))
#define LSM_IS_DOM_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_DOCUMENT))
#define LSM_IS_DOM_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_DOCUMENT))
#define LSM_DOM_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_DOCUMENT, LsmDomDocumentClass))

typedef struct _GDomDocumentClass LsmDomDocumentClass;

struct _GDomDocument {
	LsmDomNode node;

	GHashTable *		ids;
	GHashTable *		elements;

	/* Not really a document property, but that simplifies things greatly */
	double resolution_ppi;
	double viewport_width;
	double viewport_height;
};

struct _GDomDocumentClass {
	LsmDomNodeClass parent_class;

	LsmDomElement *	(*get_document_element) (LsmDomDocument* self);
	LsmDomElement *	(*create_element) 	(LsmDomDocument* self, const char *tag_name);
	LsmDomText * 	(*create_text_node) 	(LsmDomDocument* self, const char *data);

	LsmDomView*	(*create_view) 		(LsmDomDocument *self);
};

GType lsm_dom_document_get_type (void);

LsmDomElement* 	lsm_dom_document_get_document_element 	(LsmDomDocument* self);
LsmDomElement* 	lsm_dom_document_create_element 	(LsmDomDocument* self, const char *tag_name);
LsmDomText* 	lsm_dom_document_create_text_node 	(LsmDomDocument* self, const char *data);
LsmDomElement *	lsm_dom_document_get_element_by_id 	(LsmDomDocument *self, const char *id);

void 		lsm_dom_document_register_element 	(LsmDomDocument *self, LsmDomElement *element, const char *id);

LsmDomView*	lsm_dom_document_create_view		(LsmDomDocument *self);
double		lsm_dom_document_get_resolution		(LsmDomDocument *self);
void		lsm_dom_document_set_resolution		(LsmDomDocument *self, double ppi);
void 		lsm_dom_document_set_viewport 		(LsmDomDocument *self, double width, double height);
void 		lsm_dom_document_get_viewport 		(LsmDomDocument *self, double *width, double *height);
void 		lsm_dom_document_set_viewport_px 	(LsmDomDocument *self, double width, double height);

G_END_DECLS

#endif
