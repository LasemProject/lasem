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

#include <gsvgpatternelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
_pattern_element_get_node_name (GDomNode *node)
{
	return "pattern";
}

/* GSvgElement implementation */

static void
_pattern_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgPatternElement *pattern = GSVG_PATTERN_ELEMENT (self);
	GSvgGradientUnits units;
	GSvgLength length;

	units = GSVG_GRADIENT_UNITS_OBJECT_BOUNDING_BOX;
	gsvg_gradient_units_attribute_parse (&pattern->units, &units);

	units = GSVG_GRADIENT_UNITS_OBJECT_BOUNDING_BOX;
	gsvg_gradient_units_attribute_parse (&pattern->content_units, &units);

	gsvg_transform_attribute_parse (&pattern->transform);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&pattern->x, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&pattern->y, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&pattern->width, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&pattern->height, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_pattern_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgPatternElement *pattern = GSVG_PATTERN_ELEMENT (self);
	double width, height;

	width = pattern->width.length.base.value;
	height = pattern->height.length.base.value;

	if (width <= 0 || height <= 0)
		return;

	gsvg_view_create_surface_pattern (view, width, height);
/*                                          pattern->units.value,*/
/*                                          pattern->content_units.value,*/
/*                                          &pattern->transform.matrix);*/

	GSVG_GRAPHIC_CLASS (parent_class)->graphic_render (self, view);
}

/* GSvgPatternElement implementation */

GDomNode *
gsvg_pattern_element_new (void)
{
	return g_object_new (GSVG_TYPE_PATTERN_ELEMENT, NULL);
}

static void
gsvg_pattern_element_init (GSvgPatternElement *self)
{
}

/* GSvgPatternElement class */

static void
gsvg_pattern_element_class_init (GSvgPatternElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (klass);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _pattern_element_get_node_name;

	s_element_class->update = _pattern_element_update;
	s_element_class->render_paint = s_element_class->render;
	s_element_class->render_paint = NULL;
	s_element_class->render = NULL;
	s_graphic_class->graphic_render = _pattern_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (GSvgPatternElement, x));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (GSvgPatternElement, y));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (GSvgPatternElement, width));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (GSvgPatternElement, height));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "patternUnits",
					  offsetof (GSvgPatternElement, units));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "patternContentUnits",
					  offsetof (GSvgPatternElement, content_units));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "patternTransform",
					  offsetof (GSvgPatternElement, transform));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (GSvgPatternElement, href));
}

G_DEFINE_TYPE (GSvgPatternElement, gsvg_pattern_element, GSVG_TYPE_GRAPHIC)
