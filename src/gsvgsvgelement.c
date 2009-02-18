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
#include <gdomdebug.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_svg_element_get_node_name (GDomNode *node)
{
	return "svg";
}

/* GSvgElement implementation */

void
gsvg_svg_element_measure (GSvgSvgElement *self, double *width, double *height)
{
	GSvgLength length;

	g_return_if_fail (GSVG_IS_SVG_ELEMENT (self));

	length.value = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_length_attribute_parse (&self->width, &length);
	length.value = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_length_attribute_parse (&self->height, &length);

	if (width != NULL)
		*width = self->width.length.value;
	if (height != NULL)
		*height = self->height.length.value;
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

	style->fill.paint.type = GSVG_PAINT_TYPE_NONE;
	style->fill.paint.uri = NULL;
	style->fill.paint.color.red = 0.0;
	style->fill.paint.color.green = 0.0;
	style->fill.paint.color.blue = 0.0;
	style->fill.opacity = 1.0;

	style->stroke.paint.type = GSVG_PAINT_TYPE_RGB_COLOR;
	style->stroke.paint.uri = NULL;
	style->stroke.paint.uri = NULL;
	style->stroke.paint.color.red = 0.0;
	style->stroke.paint.color.green = 0.0;
	style->stroke.paint.color.blue = 0.0;
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

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = gsvg_svg_element_finalize;

	d_node_class->get_node_name = gsvg_svg_element_get_node_name;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (GSvgSvgElement, x));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (GSvgSvgElement, y));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (GSvgSvgElement, width));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (GSvgSvgElement, height));
}

G_DEFINE_TYPE (GSvgSvgElement, gsvg_svg_element, GSVG_TYPE_GRAPHIC)
