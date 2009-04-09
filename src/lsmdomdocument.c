/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#include <lsmdomdocument.h>
#include <lsmdomelement.h>
#include <lsmdomdebug.h>
#include <lsmdomtext.h>

/* LsmDomNode implementation */

static const char *
lsm_dom_document_get_node_name (LsmDomNode *node)
{
	return "#document";
}

static LsmDomNodeType
lsm_dom_document_get_node_type (LsmDomNode *node)
{
	return LSM_DOM_NODE_TYPE_DOCUMENT_NODE;
}

/* LsmDomDocument implementation */

LsmDomElement *
lsm_dom_document_get_document_element (LsmDomDocument *self)
{
	g_return_val_if_fail (LSM_DOM_IS_DOCUMENT (self), NULL);

	return LSM_DOM_ELEMENT (lsm_dom_node_get_first_child (LSM_DOM_NODE (self)));
}

LsmDomElement *
lsm_dom_document_create_element (LsmDomDocument *document, const char *tag_name)
{
	LsmDomDocumentClass *document_class;

	g_return_val_if_fail (LSM_DOM_IS_DOCUMENT (document), NULL);

	document_class = LSM_DOM_DOCUMENT_GET_CLASS (document);
	if (document_class->create_element != NULL)
		return document_class->create_element (document, tag_name);

	return NULL;
}

LsmDomText *
lsm_dom_document_create_text_node_base (LsmDomDocument *document, const char *data)
{
	return LSM_DOM_TEXT (lsm_dom_text_new (data));
}

LsmDomText *
lsm_dom_document_create_text_node (LsmDomDocument *document, const char *data)
{
	g_return_val_if_fail (LSM_DOM_IS_DOCUMENT (document), NULL);

	return LSM_DOM_DOCUMENT_GET_CLASS (document)->create_text_node (document, data);
}

LsmDomView *
lsm_dom_document_create_view (LsmDomDocument *self)
{
	g_return_val_if_fail (LSM_DOM_IS_DOCUMENT (self), NULL);

	return LSM_DOM_DOCUMENT_GET_CLASS (self)->create_view (self);
}

LsmDomElement *
lsm_dom_document_get_element_by_id (LsmDomDocument *self, const char *id)
{
	g_return_val_if_fail (LSM_DOM_IS_DOCUMENT (self), NULL);
	g_return_val_if_fail (id != NULL, NULL);

	lsm_dom_debug ("[LsmDomDocument::get_element_by_id] Lookup '%s'", id);

	return g_hash_table_lookup (self->ids, id);
}

void
lsm_dom_document_register_element (LsmDomDocument *self, LsmDomElement *element, const char *id)
{
	char *old_id;

	g_return_if_fail (LSM_DOM_IS_DOCUMENT (self));

	old_id = g_hash_table_lookup (self->elements, element);
	if (old_id != NULL) {
		lsm_dom_debug ("[LsmDomDocument::register_element] Unregister '%s'", old_id);

		g_hash_table_remove (self->elements, element);
		g_hash_table_remove (self->ids, old_id);
	}

	if (id != NULL) {
		char *new_id = g_strdup (id);

		lsm_dom_debug ("[LsmDomDocument::register_element] Register '%s'", id);

		g_hash_table_replace (self->ids, new_id, element);
		g_hash_table_replace (self->elements, element, new_id);
	}
}

static void
lsm_dom_document_init (LsmDomDocument *document)
{
	document->ids = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
	document->elements = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, NULL);
}

static void
lsm_dom_document_finalize (GObject *object)
{
	LsmDomDocument *document = LSM_DOM_DOCUMENT (object);

	g_hash_table_unref (document->elements);
	g_hash_table_unref (document->ids);
}

/* LsmDomDocument class */

static void
lsm_dom_document_class_init (LsmDomDocumentClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *node_class = LSM_DOM_NODE_CLASS (klass);

	object_class->finalize = lsm_dom_document_finalize;

	node_class->get_node_name = lsm_dom_document_get_node_name;
	node_class->get_node_type = lsm_dom_document_get_node_type;

	klass->create_text_node = lsm_dom_document_create_text_node_base;
}

G_DEFINE_ABSTRACT_TYPE (LsmDomDocument, lsm_dom_document, LSM_DOM_TYPE_NODE)

