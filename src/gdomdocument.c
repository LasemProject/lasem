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

#include <gdomdocument.h>
#include <gdomelement.h>
#include <gdomtext.h>

/* GDomNode implementation */

static const char *
gdom_document_get_node_name (GDomNode *node)
{
	return "#document";
}

static GDomNodeType
gdom_document_get_node_type (GDomNode *node)
{
	return GDOM_NODE_TYPE_DOCUMENT_NODE;
}

/* GDomDocument implementation */

GDomElement *
gdom_document_get_document_element (GDomDocument *self)
{
	g_return_val_if_fail (GDOM_IS_DOCUMENT (self), NULL);

	return GDOM_ELEMENT (gdom_node_get_first_child (GDOM_NODE (self)));
}

GDomElement *
gdom_document_create_element (GDomDocument *document, const char *tag_name)
{
	GDomDocumentClass *document_class;

	g_return_val_if_fail (GDOM_IS_DOCUMENT (document), NULL);

	document_class = GDOM_DOCUMENT_GET_CLASS (document);
	if (document_class->create_element != NULL)
		return document_class->create_element (document, tag_name);

	return NULL;
}

GDomText *
gdom_document_create_text_node_base (GDomDocument *document, const char *data)
{
	return GDOM_TEXT (gdom_text_new (data));
}

GDomText *
gdom_document_create_text_node (GDomDocument *document, const char *data)
{
	g_return_val_if_fail (GDOM_IS_DOCUMENT (document), NULL);

	return GDOM_DOCUMENT_GET_CLASS (document)->create_text_node (document, data);
}

GDomView*
gdom_document_create_view (GDomDocument *self)
{
	g_return_val_if_fail (GDOM_IS_DOCUMENT (self), NULL);

	return GDOM_DOCUMENT_GET_CLASS (self)->create_view (self);
}

static void
gdom_document_init (GDomDocument *document)
{
}

/* GDomDocument class */

static void
gdom_document_class_init (GDomDocumentClass *klass)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (klass);

	node_class->get_node_name = gdom_document_get_node_name;
	node_class->get_node_type = gdom_document_get_node_type;

	klass->create_text_node = gdom_document_create_text_node_base;
}

G_DEFINE_ABSTRACT_TYPE (GDomDocument, gdom_document, GDOM_TYPE_NODE)

