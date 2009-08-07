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
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static gboolean
lsm_svg_gradient_element_can_append_child (LsmDomNode *parent, LsmDomNode *child)
{
	return LSM_IS_SVG_STOP_ELEMENT (child);
}

/* LsmSvgElement implementation */

static void
lsm_svg_gradient_element_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgGradientElement *gradient = LSM_SVG_GRADIENT_ELEMENT (self);
	LsmSvgGradientElementClass *gradient_class = LSM_SVG_GRADIENT_ELEMENT_GET_CLASS (self);

	if (!gradient->enable_rendering)
		return;

	gradient->enable_rendering = FALSE;

	g_return_if_fail (gradient_class->create_gradient != NULL);

	gradient_class->create_gradient (self, view);

	lsm_svg_view_set_gradient_properties (view,
					      gradient->spread_method.value,
					      gradient->units.value,
					      &gradient->transform.matrix);

	LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);
}

static void
lsm_svg_gradient_element_enable_rendering (LsmSvgElement *element)
{
	LSM_SVG_GRADIENT_ELEMENT (element)->enable_rendering = TRUE;
}

/* LsmSvgGradientElement implementation */

static const LsmSvgPatternUnits units_default = LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
static const LsmSvgSpreadMethod spread_method_default = LSM_SVG_SPREAD_METHOD_PAD;
static const LsmSvgMatrix matrix_default = {1.0, 0.0, 0.0, 1.0, 0.0, 0.0, LSM_SVG_MATRIX_FLAGS_IDENTITY};

static void
lsm_svg_gradient_element_init (LsmSvgGradientElement *self)
{
	self->enable_rendering = FALSE;
	self->units.value = units_default;
	self->spread_method.value = spread_method_default;
	lsm_svg_matrix_init_identity (&self->transform.matrix);
}

/* LsmSvgGradientElement class */

static const LsmAttributeInfos lsm_svg_gradient_element_attribute_infos[] = {
	{
		.name = "gradientUnits",
		.attribute_offset = offsetof (LsmSvgGradientElement, units),
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.trait_default = &units_default
	},
	{
		.name = "gradientTransform",
		.attribute_offset = offsetof (LsmSvgGradientElement, transform),
		.trait_class = &lsm_svg_matrix_trait_class,
		.trait_default = &matrix_default
	},
	{
		.name = "spreadMethod",
		.attribute_offset = offsetof (LsmSvgGradientElement, spread_method),
		.trait_class = &lsm_svg_spread_method_trait_class,
		.trait_default = &spread_method_default
	},
	{
		.name = "xlink:href",
		.attribute_offset = offsetof (LsmSvgGradientElement, href),
		.trait_class = &lsm_null_trait_class
	}
};

static void
lsm_svg_gradient_element_class_init (LsmSvgGradientElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->can_append_child = lsm_svg_gradient_element_can_append_child;

	s_element_class->render = lsm_svg_gradient_element_render;
	s_element_class->enable_rendering = lsm_svg_gradient_element_enable_rendering;
	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_gradient_element_attribute_infos),
					      lsm_svg_gradient_element_attribute_infos);
}

G_DEFINE_ABSTRACT_TYPE (LsmSvgGradientElement, lsm_svg_gradient_element, LSM_TYPE_SVG_ELEMENT)
