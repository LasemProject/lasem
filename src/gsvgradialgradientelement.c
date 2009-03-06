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

#include <gsvgradialgradientelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_radial_gradient_element_get_node_name (GDomNode *node)
{
	return "radialGradient";
}

/* GRadialGradientElement implementation */

static void
_radial_gradient_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgRadialGradientElement *radial = GSVG_RADIAL_GRADIENT_ELEMENT (self);
	GSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&radial->cx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&radial->cy, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&radial->r, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&radial->fx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = GSVG_LENGTH_TYPE_PX;
	gsvg_animated_length_attribute_parse (&radial->fy, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_radial_gradient_element_render (GSvgElement *self, GSvgView *view)
{
	GSvgRadialGradientElement *radial = GSVG_RADIAL_GRADIENT_ELEMENT (self);
	double cx, cy, fx, fy, r;

	cx = radial->cx.length.base.value;
	cy = radial->cx.length.base.value;
	r  = radial->r.length.base.value;
	fx = radial->fx.length.base.value;
	fy = radial->fy.length.base.value;

	gsvg_view_create_radial_pattern (view, cx, cy, r, fx, fy);

	GSVG_ELEMENT_CLASS (parent_class)->render (self, view);
}

/* GSvgRadialGradientElement implementation */

GDomNode *
gsvg_radial_gradient_element_new (void)
{
	return g_object_new (GSVG_TYPE_RADIAL_GRADIENT_ELEMENT, NULL);
}

static void
gsvg_radial_gradient_element_init (GSvgRadialGradientElement *self)
{

}

static void
gsvg_radial_gradient_element_finalize (GObject *object)
{
}

/* GSvgRadialGradientElement class */

static void
gsvg_radial_gradient_element_class_init (GSvgRadialGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_svg_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = gsvg_radial_gradient_element_finalize;

	d_node_class->get_node_name = gsvg_radial_gradient_element_get_node_name;

	s_element_class->update = _radial_gradient_element_update;
	s_element_class->render = _radial_gradient_element_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "cx",
					  offsetof (GSvgRadialGradientElement, cx));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "cy",
					  offsetof (GSvgRadialGradientElement, cy));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "r",
					  offsetof (GSvgRadialGradientElement, r));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "fx",
					  offsetof (GSvgRadialGradientElement, fx));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "fy",
					  offsetof (GSvgRadialGradientElement, fy));
}

G_DEFINE_TYPE (GSvgRadialGradientElement, gsvg_radial_gradient_element, GSVG_TYPE_GRADIENT_ELEMENT)
