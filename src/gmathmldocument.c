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
#include <gmathmlradicalelement.h>
#include <gmathmlscriptelement.h>
#include <gmathmlfractionelement.h>
#include <gmathmloperatorelement.h>
#include <gmathmlrowelement.h>
#include <gmathmlstyleelement.h>
#include <string.h>

/* GDomNode implementation */

static gboolean
gmathml_document_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GMATHML_IS_MATH_ELEMENT (child));
}

/* GDomDocument implementation */

static GDomElement *
gmathml_document_create_element (GDomDocument *document, const char *tag_name)
{
	GDomNode *node = NULL;

	if (strcmp (tag_name, "math") == 0)
		node = gmathml_math_element_new ();
	else if (strcmp (tag_name, "msqrt") == 0)
		node = gmathml_sqrt_element_new ();
	else if (strcmp (tag_name, "mroot") == 0)
		node = gmathml_root_element_new ();
	else if (strcmp (tag_name, "msub") == 0)
		node = gmathml_sub_element_new ();
	else if (strcmp (tag_name, "msup") == 0)
		node = gmathml_sup_element_new ();
	else if (strcmp (tag_name, "msubsup") == 0)
		node = gmathml_sub_sup_element_new ();
	else if (strcmp (tag_name, "mfrac") == 0)
		node = gmathml_fraction_element_new ();
	else if (strcmp (tag_name, "mo") == 0)
		node = gmathml_operator_element_new ();
	else if (strcmp (tag_name, "mrow") == 0)
		node = gmathml_row_element_new ();
	else if (strcmp (tag_name, "mn") == 0)
		node = gmathml_number_element_new ();
	else if (strcmp (tag_name, "mi") == 0)
		node = gmathml_identifier_element_new ();
	else if (strcmp (tag_name, "mtext") == 0)
		node = gmathml_text_element_new ();
	else if (strcmp (tag_name, "mstyle") == 0)
		node = gmathml_style_element_new ();

	return GDOM_ELEMENT (node);
}

/* GMathmlDocument implementation */

GDomNode *
gmathml_document_new (void)
{
	return g_object_new (GMATHML_TYPE_DOCUMENT, NULL);
}

static void
gmathml_document_init (GMathmlDocument *document)
{
}

/* GMathmlDocument class */

static void
gmathml_document_class_init (GMathmlDocumentClass *m_document_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_document_class);
	GDomDocumentClass *d_document_class = GDOM_DOCUMENT_CLASS (m_document_class);

	d_node_class->can_append_child = gmathml_document_can_append_child;

	d_document_class->create_element = gmathml_document_create_element;
}

G_DEFINE_TYPE (GMathmlDocument, gmathml_document, GDOM_TYPE_DOCUMENT)
