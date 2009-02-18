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

#include <gsvgpathelement.h>
#include <gsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_path_element_get_node_name (GDomNode *node)
{
	return "path";
}

/* GSvgElement implementation */

static void
gsvg_path_element_render (GSvgElement *self, GSvgView *view)
{
	GSvgPathElement *path;

	path = GSVG_PATH_ELEMENT (self);

	gsvg_view_show_path (view, gdom_attribute_get_value (&path->d));
}

/* GSvgPathElement implementation */

GDomNode *
gsvg_path_element_new (void)
{
	return g_object_new (GSVG_TYPE_PATH_ELEMENT, NULL);
}

static void
gsvg_path_element_init (GSvgPathElement *self)
{
}

static void
gsvg_path_element_finalize (GObject *object)
{
}

/* GSvgPathElement class */

static void
gsvg_path_element_class_init (GSvgPathElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_rect_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = gsvg_path_element_finalize;

	d_node_class->get_node_name = gsvg_path_element_get_node_name;

	s_element_class->render = gsvg_path_element_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "d",
					  offsetof (GSvgPathElement, d));
}

G_DEFINE_TYPE (GSvgPathElement, gsvg_path_element, GSVG_TYPE_GRAPHIC)
