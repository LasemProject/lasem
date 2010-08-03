/*
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgradialgradientelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>
#include <math.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_radial_gradient_element_get_node_name (LsmDomNode *node)
{
	return "radialGradient";
}

/* LsmSvgRadialGradientElement implementation */

static void
lsm_svg_radial_gradient_element_create_gradient (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgRadialGradientElement *radial = LSM_SVG_RADIAL_GRADIENT_ELEMENT (self);
	gboolean is_object_bounding_box;
	double cx, cy, fx, fy, r;
	double gradient_radius;

	is_object_bounding_box = (LSM_SVG_GRADIENT_ELEMENT (self)->units.value ==
				  LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_view_push_viewbox (view, &viewbox);
	}

	cx = lsm_svg_view_normalize_length (view, &radial->cx.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	cy = lsm_svg_view_normalize_length (view, &radial->cy.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	r  = lsm_svg_view_normalize_length (view, &radial->r.length,  LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	if (lsm_attribute_is_defined (&radial->fx.base))
		fx = lsm_svg_view_normalize_length (view, &radial->fx.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	else
		fx = cx;

	if (lsm_attribute_is_defined (&radial->fy.base))
		fy = lsm_svg_view_normalize_length (view, &radial->fy.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	else
		fy = cy;

	gradient_radius = sqrt ((fx - cx) * (fx - cx) + (fy - cy) * (fy - cy));
	if (gradient_radius > r) {
		if (gradient_radius > 0.0) {
			fx = cx + ((fx - cx) * r / gradient_radius);
			fy = cy + ((fy - cy) * r / gradient_radius);
		} else {
			fx = cx;
			fy = cy;
		}
	}

	if (is_object_bounding_box)
		lsm_svg_view_pop_viewbox (view);

	lsm_debug ("render", "[LsmSvgRadialElement::render] cx = %g, cy = %g, r = %g, fx = %g, fy = %g",
		    cx, cy, r, fx, fy);

	lsm_svg_view_create_radial_gradient (view, cx, cy, r, fx, fy);
}

/* LsmSvgRadialGradientElement implementation */

LsmDomNode *
lsm_svg_radial_gradient_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_RADIAL_GRADIENT_ELEMENT, NULL);
}

static const LsmSvgLength length_default = {.value_unit = 50.0, .type = LSM_SVG_LENGTH_TYPE_PERCENTAGE};
static const LsmSvgLength unset_default  = {.value_unit =  0.0, .type = LSM_SVG_LENGTH_TYPE_ERROR};

static void
lsm_svg_radial_gradient_element_init (LsmSvgRadialGradientElement *self)
{
	self->cx.length = length_default;
	self->cy.length = length_default;
	self->r.length = length_default;
	self->fx.length = unset_default;
	self->fy.length = unset_default;
}

static void
lsm_svg_radial_gradient_element_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgRadialGradientElement class */

static const LsmAttributeInfos lsm_svg_radial_gradient_element_attribute_infos[] = {
	{
		.name = "cx",
		.attribute_offset = offsetof (LsmSvgRadialGradientElement, cx),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "cy",
		.attribute_offset = offsetof (LsmSvgRadialGradientElement, cy),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "r",
		.attribute_offset = offsetof (LsmSvgRadialGradientElement, r),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "fx",
		.attribute_offset = offsetof (LsmSvgRadialGradientElement, fx),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &unset_default
	},
	{
		.name = "fy",
		.attribute_offset = offsetof (LsmSvgRadialGradientElement, fy),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &unset_default
	}
};

static void
lsm_svg_radial_gradient_element_class_init (LsmSvgRadialGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_svg_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_svg_class);
	LsmSvgGradientElementClass *s_gradient_class = LSM_SVG_GRADIENT_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = lsm_svg_radial_gradient_element_finalize;

	d_node_class->get_node_name = lsm_svg_radial_gradient_element_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_radial_gradient_element_attribute_infos),
					      lsm_svg_radial_gradient_element_attribute_infos);

	s_gradient_class->create_gradient = lsm_svg_radial_gradient_element_create_gradient;
}

G_DEFINE_TYPE (LsmSvgRadialGradientElement, lsm_svg_radial_gradient_element, LSM_TYPE_SVG_GRADIENT_ELEMENT)
