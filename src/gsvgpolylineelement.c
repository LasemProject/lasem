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

#include <gsvgpolylineelement.h>
#include <gsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_polyline_element_get_node_name (GDomNode *node)
{
	return "polyline";
}

/* GSvgElement implementation */

/* GSvgGraphic implementation */

static void
gsvg_polyline_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgPolylineElement *polyline = GSVG_POLYLINE_ELEMENT (self);

	gsvg_view_show_polyline (view, polyline->points.value);
}

/* GSvgPolylineElement implementation */

GDomNode *
gsvg_polyline_element_new (void)
{
	return g_object_new (GSVG_TYPE_POLYLINE_ELEMENT, NULL);
}

static void
gsvg_polyline_element_init (GSvgPolylineElement *self)
{
}

/* GSvgPolylineElement class */

static void
gsvg_polyline_element_class_init (GSvgPolylineElementClass *s_rect_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_rect_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_rect_class);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	d_node_class->get_node_name = gsvg_polyline_element_get_node_name;

	s_graphic_class->graphic_render = gsvg_polyline_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "points",
					  offsetof (GSvgPolylineElement, points));
}

G_DEFINE_TYPE (GSvgPolylineElement, gsvg_polyline_element, GSVG_TYPE_GRAPHIC)
