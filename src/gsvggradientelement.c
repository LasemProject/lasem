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

#include <gsvggradientelement.h>
#include <gsvgstopelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static gboolean
gsvg_gradient_element_can_append_child (GDomNode *parent, GDomNode *child)
{
	return GSVG_IS_STOP_ELEMENT (child);
}

/* GSvgElement implementation */

static void
_gradient_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgGradientElement *gradient = GSVG_GRADIENT_ELEMENT (self);
	GSvgGradientUnits units;
	GSvgSpreadMethod method;

	units = GSVG_GRADIENT_UNITS_OBJECT_BOUNDING_BOX;
	method = GSVG_SPREAD_METHOD_PAD;

	gsvg_gradient_units_attribute_parse (&gradient->units, &units);
	gsvg_spread_method_attribute_parse (&gradient->spread_method, &method);
	gsvg_transform_attribute_parse (&gradient->transform);
}

static void
_gradient_element_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgGradientElement *gradient = GSVG_GRADIENT_ELEMENT (self);
	GDomNode *iter;

	for (iter = GDOM_NODE (self)->first_child; iter != NULL; iter = iter->next_sibling) {
		if (GSVG_IS_STOP_ELEMENT (iter)) {
			GSvgStopElement *stop;
			const GSvgColor *color;
			double offset;
			double opacity;

			stop = GSVG_STOP_ELEMENT (iter);

			offset = gsvg_stop_element_get_offset (stop);
			color = gsvg_stop_element_get_color (stop);
			opacity = gsvg_stop_element_get_opacity (stop);

			gdom_debug ("[GSvgGradientElement::render] Add stop at %g (%g,%g,%g,%g)",
				    offset, color->red, color->green, color->blue, opacity);

			gsvg_view_add_color_stop (view, offset, color, opacity);
		}
	}

	gsvg_view_set_gradient_properties (view,
					   gradient->spread_method.value,
					   gradient->units.value,
					   &gradient->transform.matrix);
}

/* GSvgGradientElement implementation */

static void
gsvg_gradient_element_init (GSvgGradientElement *self)
{
}

/* GSvgGradientElement class */

static void
gsvg_gradient_element_class_init (GSvgGradientElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (klass);
	GSvgGraphicClass *s_graphic_class = GSVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->can_append_child = gsvg_gradient_element_can_append_child;

	s_element_class->update = _gradient_element_update;
	s_graphic_class->graphic_render = _gradient_element_graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "gradientUnits",
					  offsetof (GSvgGradientElement, units));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "gradientTransform",
					  offsetof (GSvgGradientElement, transform));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "spreadMethod",
					  offsetof (GSvgGradientElement, spread_method));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (GSvgGradientElement, href));
}

G_DEFINE_ABSTRACT_TYPE (GSvgGradientElement, gsvg_gradient_element, GSVG_TYPE_GRAPHIC)
