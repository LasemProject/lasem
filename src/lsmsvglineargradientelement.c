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

#include <lsmsvglineargradientelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_linear_gradient_element_get_node_name (LsmDomNode *node)
{
	return "linearGradient";
}

/* GLinearGradientElement implementation */

static void
lsm_svg_linear_gradient_element_create_gradient (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgLinearGradientElement *linear = LSM_SVG_LINEAR_GRADIENT_ELEMENT (self);
	gboolean is_object_bounding_box;
	double x1, x2, y1, y2;

	is_object_bounding_box = (LSM_SVG_GRADIENT_ELEMENT (self)->units.value ==
				  LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_view_push_viewbox (view, &viewbox);
	}

	x1 = lsm_svg_view_normalize_length (view, &linear->x1.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y1 = lsm_svg_view_normalize_length (view, &linear->y1.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	x2 = lsm_svg_view_normalize_length (view, &linear->x2.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y2 = lsm_svg_view_normalize_length (view, &linear->y2.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	lsm_debug ("[LsmSvgLinearGradientElement::render] Create linear %g, %g, %g, %g",
		    x1, y1, x2, y2);

	if (is_object_bounding_box)
		lsm_svg_view_pop_viewbox (view);

	lsm_svg_view_create_linear_gradient (view, x1, y1, x2, y2);
}

/* LsmSvgLinearGradientElement implementation */

LsmDomNode *
lsm_svg_linear_gradient_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_LINEAR_GRADIENT_ELEMENT, NULL);
}

static const LsmSvgLength x1_y1_y2_default = 	{ .value_unit =   0.0, .type = LSM_SVG_LENGTH_TYPE_PERCENTAGE};
static const LsmSvgLength x2_default =		{ .value_unit = 100.0, .type = LSM_SVG_LENGTH_TYPE_PERCENTAGE};

static void
lsm_svg_linear_gradient_element_init (LsmSvgLinearGradientElement *self)
{
	self->x1.length = x1_y1_y2_default;
	self->y1.length = x1_y1_y2_default;
	self->x2.length = x2_default;
	self->y2.length = x1_y1_y2_default;
}

static void
lsm_svg_linear_gradient_element_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgLinearGradientElement class */

static const LsmAttributeInfos lsm_svg_linear_gradient_element_attribute_infos[] = {
	{
		.name = "x1",
		.attribute_offset = offsetof (LsmSvgLinearGradientElement, x1),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x1_y1_y2_default
	},
	{
		.name = "y1",
		.attribute_offset = offsetof (LsmSvgLinearGradientElement, y1),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x1_y1_y2_default
	},
	{
		.name = "x2",
		.attribute_offset = offsetof (LsmSvgLinearGradientElement, x2),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x2_default
	},
	{
		.name = "y2",
		.attribute_offset = offsetof (LsmSvgLinearGradientElement, y2),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x1_y1_y2_default
	}
};

static void
lsm_svg_linear_gradient_element_class_init (LsmSvgLinearGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_svg_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_svg_class);
	LsmSvgGradientElementClass *s_gradient_class = LSM_SVG_GRADIENT_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = lsm_svg_linear_gradient_element_finalize;

	d_node_class->get_node_name = lsm_svg_linear_gradient_element_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_linear_gradient_element_attribute_infos),
					      lsm_svg_linear_gradient_element_attribute_infos);

	s_gradient_class->create_gradient = lsm_svg_linear_gradient_element_create_gradient;
}

G_DEFINE_TYPE (LsmSvgLinearGradientElement, lsm_svg_linear_gradient_element, LSM_TYPE_SVG_GRADIENT_ELEMENT)
