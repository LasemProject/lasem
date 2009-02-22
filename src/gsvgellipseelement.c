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

#include <gsvgellipseelement.h>
#include <gsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_ellipse_element_get_node_name (GDomNode *node)
{
	return "ellipse";
}

/* GSvgElement implementation */

static void
gsvg_ellipse_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgEllipseElement *ellipse = GSVG_ELLIPSE_ELEMENT (self);
	GSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_animated_length_attribute_parse (&ellipse->cx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_animated_length_attribute_parse (&ellipse->cy, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_animated_length_attribute_parse (&ellipse->rx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_animated_length_attribute_parse (&ellipse->ry, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* GSvgGraphic implementation */

static void
gsvg_ellipse_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgEllipseElement *ellipse = GSVG_ELLIPSE_ELEMENT (self);

	gsvg_view_show_ellipse (view,
				ellipse->cx.length.base.value,
				ellipse->cy.length.base.value,
				ellipse->rx.length.base.value,
				ellipse->ry.length.base.value);
}

/* GSvgEllipseElement implementation */

GDomNode *
gsvg_ellipse_element_new (void)
{
	return g_object_new (GSVG_TYPE_ELLIPSE_ELEMENT, NULL);
}

static void
gsvg_ellipse_element_init (GSvgEllipseElement *self)
{
}

static void
gsvg_ellipse_element_finalize (GObject *object)
{
}

/* GSvgEllipseElement class */

static void
gsvg_ellipse_element_class_init (GSvgEllipseElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_rect_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_rect_class);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = gsvg_ellipse_element_finalize;

	d_node_class->get_node_name = gsvg_ellipse_element_get_node_name;

	s_element_class->update = gsvg_ellipse_element_update;

	s_graphic_class->graphic_render = gsvg_ellipse_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "cx",
					  offsetof (GSvgEllipseElement, cx));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "cy",
					  offsetof (GSvgEllipseElement, cy));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "rx",
					  offsetof (GSvgEllipseElement, rx));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "ry",
					  offsetof (GSvgEllipseElement, ry));
}

G_DEFINE_TYPE (GSvgEllipseElement, gsvg_ellipse_element, GSVG_TYPE_GRAPHIC)
