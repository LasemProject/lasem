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

#include <gsvgtextelement.h>
#include <gsvgview.h>
#include <gdomtext.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_text_element_get_node_name (GDomNode *node)
{
	return "text";
}

static gboolean
gsvg_text_element_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GDOM_IS_TEXT (child));
}

/* GSvgElement implementation */

static void
gsvg_text_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgTextElement *text = GSVG_TEXT_ELEMENT (self);
	GSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&text->x, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&text->y, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* GSvgGraphic implementation */

static void
gsvg_text_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgTextElement *text = GSVG_TEXT_ELEMENT (self);
	GDomNode *node = GDOM_NODE (self);
	const char *string;

	if (node->first_child == NULL)
		return;

	string = gdom_node_get_node_value (node->first_child);

	gsvg_view_show_text (view, string, text->x.length.base.value, text->y.length.base.value);
}

/* GSvgTextElement implementation */

GDomNode *
gsvg_text_element_new (void)
{
	return g_object_new (GSVG_TYPE_TEXT_ELEMENT, NULL);
}

static void
gsvg_text_element_init (GSvgTextElement *self)
{
}

static void
gsvg_text_element_finalize (GObject *object)
{
}

/* GSvgTextElement class */

static void
gsvg_text_element_class_init (GSvgTextElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_rect_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_rect_class);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = gsvg_text_element_finalize;

	d_node_class->get_node_name = gsvg_text_element_get_node_name;
	d_node_class->can_append_child = gsvg_text_element_can_append_child;

	s_element_class->update = gsvg_text_element_update;

	s_graphic_class->graphic_render = gsvg_text_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (GSvgTextElement, x));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (GSvgTextElement, y));
}

G_DEFINE_TYPE (GSvgTextElement, gsvg_text_element, GSVG_TYPE_GRAPHIC)
