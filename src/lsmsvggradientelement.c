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

#include <lsmsvggradientelement.h>
#include <lsmsvgstopelement.h>
#include <lsmsvgview.h>
#include <lsmdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static gboolean
lsm_svg_gradient_element_can_append_child (LsmDomNode *parent, LsmDomNode *child)
{
	return LSM_SVG_IS_STOP_ELEMENT (child);
}

/* LsmSvgElement implementation */

static void
_gradient_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgGradientElement *gradient = LSM_SVG_GRADIENT_ELEMENT (self);
	LsmSvgGradientUnits units;
	LsmSvgSpreadMethod method;

	units = LSM_SVG_GRADIENT_UNITS_OBJECT_BOUNDING_BOX;
	method = LSM_SVG_SPREAD_METHOD_PAD;

	lsm_svg_gradient_units_attribute_parse (&gradient->units, &units);
	lsm_svg_spread_method_attribute_parse (&gradient->spread_method, &method);
	lsm_svg_transform_attribute_parse (&gradient->transform);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_gradient_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgGradientElement *gradient = LSM_SVG_GRADIENT_ELEMENT (self);
	LsmDomNode *iter;

	for (iter = LSM_DOM_NODE (self)->first_child; iter != NULL; iter = iter->next_sibling) {
		if (LSM_SVG_IS_STOP_ELEMENT (iter)) {
			LsmSvgStopElement *stop;
			const LsmSvgColor *color;
			double offset;
			double opacity;

			stop = LSM_SVG_STOP_ELEMENT (iter);

			offset = lsm_svg_stop_element_get_offset (stop);
			color = lsm_svg_stop_element_get_color (stop);
			opacity = lsm_svg_stop_element_get_opacity (stop);

			lsm_dom_debug ("[LsmSvgGradientElement::render] Add stop at %g (%g,%g,%g,%g)",
				    offset, color->red, color->green, color->blue, opacity);

			lsm_svg_view_add_gradient_color_stop (view, offset, color, opacity);
		}
	}

	lsm_svg_view_set_gradient_properties (view,
					   gradient->spread_method.value,
					   gradient->units.value,
					   &gradient->transform.matrix);
}

/* LsmSvgGradientElement implementation */

static void
lsm_svg_gradient_element_init (LsmSvgGradientElement *self)
{
}

/* LsmSvgGradientElement class */

static void
lsm_svg_gradient_element_class_init (LsmSvgGradientElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->can_append_child = lsm_svg_gradient_element_can_append_child;

	s_element_class->update = _gradient_element_update;
	s_element_class->render_paint = s_element_class->render;
	s_element_class->render = NULL;
	s_graphic_class->graphic_render = _gradient_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "gradientUnits",
					  offsetof (LsmSvgGradientElement, units));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "gradientTransform",
					  offsetof (LsmSvgGradientElement, transform));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "spreadMethod",
					  offsetof (LsmSvgGradientElement, spread_method));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (LsmSvgGradientElement, href));
}

G_DEFINE_ABSTRACT_TYPE (LsmSvgGradientElement, lsm_svg_gradient_element, LSM_SVG_TYPE_GRAPHIC)
