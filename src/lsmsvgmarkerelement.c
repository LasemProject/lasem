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

#include <lsmsvgmarkerelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <lsmdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_marker_element_get_node_name (LsmDomNode *node)
{
	return "marker";
}

/* LsmSvgElement implementation */

/* LsmSvgGraphic implementation */

static void
lsm_svg_marker_element_render (LsmSvgElement *self, LsmSvgView *view)
{
}

/* LsmSvgMarkerElement implementation */

LsmDomNode *
lsm_svg_marker_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_MARKER_ELEMENT, NULL);
}

static const LsmSvgMarkerUnits units_default =   LSM_SVG_MARKER_UNITS_STROKE_WIDTH;
static const LsmSvgLength length_default = 	 { .value_unit =   0.0, .type = LSM_SVG_LENGTH_TYPE_NUMBER};
static const LsmSvgLength width_default = 	 { .value_unit =   3.0, .type = LSM_SVG_LENGTH_TYPE_NUMBER};
static const LsmSvgAngle orientation_default =	 { .angle =        0.0, .type = LSM_SVG_ANGLE_TYPE_FIXED};

static void
lsm_svg_marker_element_init (LsmSvgMarkerElement *self)
{
	self->ref_x.length = length_default;
	self->ref_y.length = length_default;
	self->width.length = width_default;
	self->height.length = width_default;
}

/* LsmSvgMarkerElement class */

static const LsmAttributeInfos lsm_svg_marker_element_attribute_infos[] = {
	{
		.name = "markerUnits",
		.attribute_offset = offsetof (LsmSvgMarkerElement, units),
		.trait_class = &lsm_svg_marker_units_trait_class,
		.trait_default = &units_default
	},
	{
		.name = "refX",
		.attribute_offset = offsetof (LsmSvgMarkerElement, ref_x),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "refY",
		.attribute_offset = offsetof (LsmSvgMarkerElement, ref_y),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "width",
		.attribute_offset = offsetof (LsmSvgMarkerElement, width),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &width_default
	},
	{
		.name = "height",
		.attribute_offset = offsetof (LsmSvgMarkerElement, height),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &width_default
	},
	{
		.name = "orient",
		.attribute_offset = offsetof (LsmSvgMarkerElement, orientation),
		.trait_class = &lsm_svg_angle_trait_class,
		.trait_default = &orientation_default
	}
};

static void
lsm_svg_marker_element_class_init (LsmSvgMarkerElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_svg_marker_element_get_node_name;

	s_element_class->render = lsm_svg_marker_element_render;
	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_marker_element_attribute_infos),
					      lsm_svg_marker_element_attribute_infos);
}

G_DEFINE_TYPE (LsmSvgMarkerElement, lsm_svg_marker_element, LSM_TYPE_SVG_ELEMENT)
