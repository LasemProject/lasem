/* gdomdocument.c
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

#include <gdomdocument.h>
#include <gdomtext.h>

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

static void
gdom_document_init (GDomDocument *document)
{
}

static void
gdom_document_class_init (GDomDocumentClass *klass)
{
	klass->create_text_node = gdom_document_create_text_node_base;
}

G_DEFINE_ABSTRACT_TYPE (GDomDocument, gdom_document, GDOM_TYPE_NODE)


