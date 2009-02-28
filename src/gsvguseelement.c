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

#include <gsvguseelement.h>
#include <gdomdebug.h>
#include <gdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_use_element_get_node_name (GDomNode *node)
{
	return "use";
}

static gboolean
gsvg_use_can_append_child (GDomNode *node, GDomNode *child)
{
	return FALSE;
}

/* GUseElement implementation */

/* GSvgGraphic implementation */

static void
gsvg_use_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgUseElement *use_element;
	GDomDocument *document;
	GDomElement *element;
	const char *id;

	document = gdom_node_get_owner_document (GDOM_NODE (self));
	if (document == NULL)
		return;

	use_element = GSVG_USE_ELEMENT (self);

	id = use_element->href.value;
	if (id == NULL)
		return;

	element = gdom_document_get_element_by_id (document, id);
	if (!GSVG_IS_ELEMENT (element))
		return;

	gsvg_element_render (GSVG_ELEMENT (element), view);
}

/* GSvgUseElement implementation */

GDomNode *
gsvg_use_element_new (void)
{
	return g_object_new (GSVG_TYPE_USE_ELEMENT, NULL);
}

static void
gsvg_use_element_init (GSvgUseElement *self)
{
}

/* GSvgUseElement class */

static void
gsvg_use_element_class_init (GSvgUseElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (klass);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = gsvg_use_element_get_node_name;
	d_node_class->can_append_child = gsvg_use_can_append_child;

	s_graphic_class->graphic_render = gsvg_use_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (GSvgUseElement, x));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (GSvgUseElement, y));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (GSvgUseElement, width));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (GSvgUseElement, height));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (GSvgUseElement, href));
}

G_DEFINE_TYPE (GSvgUseElement, gsvg_use_element, GSVG_TYPE_GRAPHIC)
