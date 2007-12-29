/* gmathmldocument.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmldocument.h>
#include <gmathmlmathelement.h>

static gboolean
gmathml_document_can_add_node (GDomNode *self, GDomNode *child)
{
	return (GMATHML_IS_MATH_ELEMENT (child));
}

GDomNode *
gmathml_document_new (void)
{
	return g_object_new (GMATHML_TYPE_DOCUMENT, NULL);
}

static void
gmathml_document_init (GMathmlDocument *document)
{
}

static void
gmathml_document_class_init (GMathmlDocumentClass *document_class)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (document_class);

	node_class->can_add_node = gmathml_document_can_add_node;
}

G_DEFINE_TYPE (GMathmlDocument, gmathml_document, GDOM_TYPE_DOCUMENT)
