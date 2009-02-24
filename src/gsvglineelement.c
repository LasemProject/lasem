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

#include <gsvglineelement.h>
#include <gsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_line_element_get_node_name (GDomNode *node)
{
	return "line";
}

/* GSvgElement implementation */

static void
gsvg_line_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgLineElement *line = GSVG_LINE_ELEMENT (self);
	GSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&line->x1, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&line->y1, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&line->x2, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&line->y2, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* GSvgGraphic implementation */

static void
gsvg_line_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgLineElement *line = GSVG_LINE_ELEMENT (self);

	gsvg_view_show_line (view,line->x1.length.base.value,
				  line->y1.length.base.value,
				  line->x2.length.base.value,
				  line->y2.length.base.value);
}

/* GSvgLineElement implementation */

GDomNode *
gsvg_line_element_new (void)
{
	return g_object_new (GSVG_TYPE_LINE_ELEMENT, NULL);
}

static void
gsvg_line_element_init (GSvgLineElement *self)
{
}

/* GSvgLineElement class */

static void
gsvg_line_element_class_init (GSvgLineElementClass *s_rect_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_rect_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_rect_class);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	d_node_class->get_node_name = gsvg_line_element_get_node_name;

	s_element_class->update = gsvg_line_element_update;

	s_graphic_class->graphic_render = gsvg_line_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x1",
					  offsetof (GSvgLineElement, x1));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y1",
					  offsetof (GSvgLineElement, y1));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "x2",
					  offsetof (GSvgLineElement, x2));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y2",
					  offsetof (GSvgLineElement, y2));
}

G_DEFINE_TYPE (GSvgLineElement, gsvg_line_element, GSVG_TYPE_GRAPHIC)
