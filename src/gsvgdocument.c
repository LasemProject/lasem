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

#include <gdomdebug.h>
#include <gsvgdocument.h>
#include <gsvgsvgelement.h>
#include <gsvggelement.h>
#include <gsvgrectelement.h>
#include <gsvgcircleelement.h>
#include <gsvgellipseelement.h>
#include <gsvglineelement.h>
#include <gsvgpolylineelement.h>
#include <gsvgpolygonelement.h>
#include <gsvgpathelement.h>
#include <gsvgtextelement.h>
#include <gsvgview.h>
#include <string.h>

/* GDomNode implementation */

static gboolean
gsvg_document_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GSVG_IS_SVG_ELEMENT (child));
}

/* GDomDocument implementation */

static GDomElement *
gsvg_document_create_element (GDomDocument *document, const char *tag_name)
{
	GDomNode *node = NULL;

	if (strcmp (tag_name, "svg") == 0)
		node = gsvg_svg_element_new ();
	else if (strcmp (tag_name, "g") == 0)
		node = gsvg_g_element_new ();
	else if (strcmp (tag_name, "rect") == 0)
		node = gsvg_rect_element_new ();
	else if (strcmp (tag_name, "circle") == 0)
		node = gsvg_circle_element_new ();
	else if (strcmp (tag_name, "ellipse") == 0)
		node = gsvg_ellipse_element_new ();
	else if (strcmp (tag_name, "path") == 0)
		node = gsvg_path_element_new ();
	else if (strcmp (tag_name, "line") == 0)
		node = gsvg_line_element_new ();
	else if (strcmp (tag_name, "polyline") == 0)
		node = gsvg_polyline_element_new ();
	else if (strcmp (tag_name, "polygon") == 0)
		node = gsvg_polygon_element_new ();
	else if (strcmp (tag_name, "text") == 0)
		node = gsvg_text_element_new ();

	return GDOM_ELEMENT (node);
}

static GDomView *
gsvg_document_create_view (GDomDocument *document)
{
	return GDOM_VIEW (gsvg_view_new (GSVG_DOCUMENT (document)));
}

/* GSvgDocument implementation */

GSvgDocument *
gsvg_document_new (void)
{
	return g_object_new (GSVG_TYPE_DOCUMENT, NULL);
}

static void
gsvg_document_init (GSvgDocument *document)
{
}

GSvgSvgElement *
gsvg_document_get_root_element (const GSvgDocument *document)
{
	return GSVG_SVG_ELEMENT (gdom_document_get_document_element (GDOM_DOCUMENT (document)));
}

/* GSvgDocument class */

static void
gsvg_document_class_init (GSvgDocumentClass *m_document_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_document_class);
	GDomDocumentClass *d_document_class = GDOM_DOCUMENT_CLASS (m_document_class);

	d_node_class->can_append_child = gsvg_document_can_append_child;

	d_document_class->create_element = gsvg_document_create_element;
	d_document_class->create_view = gsvg_document_create_view;
}

G_DEFINE_TYPE (GSvgDocument, gsvg_document, GDOM_TYPE_DOCUMENT)
