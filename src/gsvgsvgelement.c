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

#include <gsvgsvgelement.h>
#include <gsvgstyle.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_svg_element_get_node_name (GDomNode *node)
{
	return "svg";
}

/* GSvgElement implementation */

static void
_svg_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgSvgElement *svg = GSVG_SVG_ELEMENT (self);
	GSvgLength length;
	GSvgViewBox view_box = {0,0,0,0};

	gsvg_view_box_attribute_parse (&svg->view_box, &view_box);

	length.value = svg->view_box.value.x;
	length.value_unit = svg->view_box.value.x;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_length_attribute_parse (&svg->x, &length, 0.0);

	length.value = svg->view_box.value.y;
	length.value_unit = svg->view_box.value.y;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_length_attribute_parse (&svg->y, &length, 0.0);

	length.value = svg->view_box.value.width;
	length.value_unit = svg->view_box.value.width;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_length_attribute_parse (&svg->width, &length, 0.0);

	length.value = svg->view_box.value.height;
	length.value_unit = svg->view_box.value.height;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_length_attribute_parse (&svg->height, &length, 0.0);

	gdom_debug ("[GSvgSvgElement::update] height = %g, width = %g",
		    svg->height.length.value,
		    svg->width.length.value);

	gdom_debug ("[GSvgSvgElement::update] view_bbox = %g, %g, %g, %g\n",
		    svg->view_box.value.x,
		    svg->view_box.value.y,
		    svg->view_box.value.width,
		    svg->view_box.value.height);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

void
gsvg_svg_element_measure (GSvgSvgElement *self, double *width, double *height)
{
	g_return_if_fail (GSVG_IS_SVG_ELEMENT (self));

	if (width != NULL)
		*width = self->width.length.value;
	if (height != NULL)
		*height = self->height.length.value;
}

/* GSvgGraphic implementation */

static void
gsvg_svg_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgSvgElement *svg = GSVG_SVG_ELEMENT (self);
	GSvgMatrix matrix;

	if (svg->view_box.value.width <= 0.0 ||
	    svg->view_box.value.height <= 0.0)
		return;

	gsvg_matrix_init (&matrix,
			  svg->width.length.value / svg->view_box.value.width,
			  0, 0,
			  svg->height.length.value / svg->view_box.value.height,
			  0, 0);
	gsvg_view_push_transform (view, &matrix);

	GSVG_GRAPHIC_CLASS (parent_class)->graphic_render (self, view);

	gsvg_view_pop_transform (view);
}

/* GSvgSvgElement implementation */

GSvgStyle *
gsvg_svg_element_get_default_style (GSvgSvgElement *svg_element)
{
	g_return_val_if_fail (GSVG_IS_SVG_ELEMENT (svg_element), NULL);

	return svg_element->default_style;
}

void
gsvg_svg_element_update (GSvgSvgElement *svg_element)
{
	gsvg_element_update (GSVG_ELEMENT (svg_element),
			     gsvg_svg_element_get_default_style (svg_element));
}

GDomNode *
gsvg_svg_element_new (void)
{
	return g_object_new (GSVG_TYPE_SVG_ELEMENT, NULL);
}

static void
gsvg_svg_element_init (GSvgSvgElement *self)
{
	GSvgStyle *style;

	style = gsvg_style_new ();
	self->default_style = style;
	g_return_if_fail (style != NULL);

	gdom_element_set_attribute (GDOM_ELEMENT (self), "fill", "black");
	gdom_element_set_attribute (GDOM_ELEMENT (self), "fill-opacity", "1");
	gdom_element_set_attribute (GDOM_ELEMENT (self), "stroke", "black");
	gdom_element_set_attribute (GDOM_ELEMENT (self), "stroke-width", "1px");
	gdom_element_set_attribute (GDOM_ELEMENT (self), "stroke-opacity", "1");
}

static void
gsvg_svg_element_finalize (GObject *object)
{
	GSvgSvgElement *svg_element = GSVG_SVG_ELEMENT (object);

	gsvg_style_free (svg_element->default_style);
}

/* GSvgSvgElement class */

static void
gsvg_svg_element_class_init (GSvgSvgElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_svg_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_svg_class);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = gsvg_svg_element_finalize;

	d_node_class->get_node_name = gsvg_svg_element_get_node_name;

	s_element_class->update = _svg_element_update;

	s_graphic_class->graphic_render = gsvg_svg_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (GSvgSvgElement, x));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (GSvgSvgElement, y));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (GSvgSvgElement, width));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (GSvgSvgElement, height));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "viewBox",
					  offsetof (GSvgSvgElement, view_box));
}

G_DEFINE_TYPE (GSvgSvgElement, gsvg_svg_element, GSVG_TYPE_GRAPHIC)
