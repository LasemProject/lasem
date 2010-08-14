/*
 * Copyright © 2010 Emmanuel Pacaud
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

#include <lsmsvgfilterelement.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_element_get_node_name (LsmDomNode *node)
{
	return "filter";
}

static gboolean
lsm_svg_filter_element_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return (LSM_IS_SVG_ELEMENT (child) &&
		lsm_svg_element_get_category (LSM_SVG_ELEMENT (child)) == LSM_SVG_ELEMENT_CATEGORY_FILTER_PRIMITIVE);
}

/* LsmSvgElement implementation */

/* LsmSvgGraphic implementation */

/* LsmSvgFilterElement implementation */

LsmDomNode *
lsm_svg_filter_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_ELEMENT, NULL);
}

static const LsmSvgLength x_y_default = 	 { .value_unit = -10.0, .type = LSM_SVG_LENGTH_TYPE_PERCENTAGE};
static const LsmSvgLength width_height_default = { .value_unit = 120.0, .type = LSM_SVG_LENGTH_TYPE_PERCENTAGE};
static const LsmSvgPatternUnits units_default =  	  LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
static const LsmSvgPatternUnits primitive_units_default = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;

static void
lsm_svg_filter_element_init (LsmSvgFilterElement *self)
{
	self->x.length = x_y_default;
	self->y.length = x_y_default;
	self->width.length = width_height_default;
	self->height.length = width_height_default;
	self->units.value = units_default;
	self->primitive_units.value = primitive_units_default;
}

static void
lsm_svg_filter_element_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterElement class */

static const LsmAttributeInfos lsm_svg_filter_element_attribute_infos[] = {
	{
		.name = "x",
		.attribute_offset = offsetof (LsmSvgFilterElement, x),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x_y_default
	},
	{
		.name = "y",
		.attribute_offset = offsetof (LsmSvgFilterElement, y),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &x_y_default
	},
	{
		.name = "width",
		.attribute_offset = offsetof (LsmSvgFilterElement, width),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &width_height_default
	},
	{
		.name = "height",
		.attribute_offset = offsetof (LsmSvgFilterElement, height),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &width_height_default
	},
	{
		.name = "filterUnits",
		.attribute_offset = offsetof (LsmSvgFilterElement, units),
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.trait_default = &units_default
	},
	{
		.name = "primitiveUnits",
		.attribute_offset = offsetof (LsmSvgFilterElement, primitive_units),
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.trait_default = &primitive_units_default
	}
};

static void
lsm_svg_filter_element_class_init (LsmSvgFilterElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_filter_element_finalize;

	d_node_class->get_node_name = lsm_svg_filter_element_get_node_name;
	d_node_class->can_append_child = lsm_svg_filter_element_can_append_child;

	s_element_class->category = LSM_SVG_ELEMENT_CATEGORY_NONE;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_element_attribute_infos),
					      lsm_svg_filter_element_attribute_infos);
}

G_DEFINE_TYPE (LsmSvgFilterElement, lsm_svg_filter_element, LSM_TYPE_SVG_ELEMENT)
