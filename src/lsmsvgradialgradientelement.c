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

#include <lsmsvgradialgradientelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_radial_gradient_element_get_node_name (LsmDomNode *node)
{
	return "radialGradient";
}

/* GRadialGradientElement implementation */

static void
_radial_gradient_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgRadialGradientElement *radial = LSM_SVG_RADIAL_GRADIENT_ELEMENT (self);
	LsmSvgLength length;

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);

	length.value = 50.0;
	length.value_unit = 50.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&radial->cx, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);

	length.value = 50.0;
	length.value_unit = 50.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&radial->cy, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	length.value = 50.0;
	length.value_unit = 50.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&radial->r, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	length = radial->cx.length.base;
	lsm_svg_animated_length_attribute_parse (&radial->fx, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);

	length = radial->cy.length.base;
	lsm_svg_animated_length_attribute_parse (&radial->fy, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_VERTICAL);
}

static void
_radial_gradient_element_render_paint (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgRadialGradientElement *radial = LSM_SVG_RADIAL_GRADIENT_ELEMENT (self);
	double cx, cy, fx, fy, r;

	cx = radial->cx.length.base.value;
	cy = radial->cy.length.base.value;
	r  = radial->r.length.base.value;
	fx = radial->fx.length.base.value;
	fy = radial->fy.length.base.value;

	lsm_debug ("[LsmSvgRadialElement::render] cx = %g, cy = %g, r = %g, fx = %g, fy = %g",
		    cx, cy, r, fx, fy);

	lsm_svg_view_create_radial_gradient (view, cx, cy, r, fx, fy);

	LSM_SVG_ELEMENT_CLASS (parent_class)->render_paint (self, view);
}

/* LsmSvgRadialGradientElement implementation */

LsmDomNode *
lsm_svg_radial_gradient_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_RADIAL_GRADIENT_ELEMENT, NULL);
}

static void
lsm_svg_radial_gradient_element_init (LsmSvgRadialGradientElement *self)
{

}

static void
lsm_svg_radial_gradient_element_finalize (GObject *object)
{
}

/* LsmSvgRadialGradientElement class */

static void
lsm_svg_radial_gradient_element_class_init (LsmSvgRadialGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_svg_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = lsm_svg_radial_gradient_element_finalize;

	d_node_class->get_node_name = lsm_svg_radial_gradient_element_get_node_name;

	s_element_class->update = _radial_gradient_element_update;
	s_element_class->render_paint = _radial_gradient_element_render_paint;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cx",
					  offsetof (LsmSvgRadialGradientElement, cx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cy",
					  offsetof (LsmSvgRadialGradientElement, cy));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "r",
					  offsetof (LsmSvgRadialGradientElement, r));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "fx",
					  offsetof (LsmSvgRadialGradientElement, fx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "fy",
					  offsetof (LsmSvgRadialGradientElement, fy));
}

G_DEFINE_TYPE (LsmSvgRadialGradientElement, lsm_svg_radial_gradient_element, LSM_TYPE_SVG_GRADIENT_ELEMENT)
