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

#include <lsmsvgpatternelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
_pattern_element_get_node_name (LsmDomNode *node)
{
	return "pattern";
}

/* LsmSvgElement implementation */

static void
lsm_svg_pattern_element_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgPatternElement *pattern = LSM_SVG_PATTERN_ELEMENT (self);
	gboolean is_object_bounding_box;
	gboolean is_viewbox_defined;
	LsmBox viewport;
	LsmBox image_box;
	const LsmBox *pattern_extents;
	LsmSvgStyle *style;

	if (!pattern->enable_rendering) {
		lsm_debug ("[LsmSvgPatternElement::render] Direct rendering not allowed");
		return;
	} else {
		pattern->enable_rendering = FALSE;
	}


	style = lsm_svg_style_new_inherited (NULL, &self->property_bag);
	lsm_svg_view_push_style (view, style);

	pattern_extents = lsm_svg_view_get_pattern_extents (view);

	is_object_bounding_box = (pattern->units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_view_push_viewbox (view, &viewbox);
	}

	viewport.x      = lsm_svg_view_normalize_length (view, &pattern->x.length,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.y      = lsm_svg_view_normalize_length (view, &pattern->y.length,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	viewport.width  = lsm_svg_view_normalize_length (view, &pattern->width.length,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.height = lsm_svg_view_normalize_length (view, &pattern->height.length,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	image_box = viewport;

	if (is_object_bounding_box) {
		lsm_svg_view_pop_viewbox (view);

		image_box.x *= pattern_extents->width;
		image_box.y *= pattern_extents->height;
		image_box.width *= pattern_extents->width;
		image_box.height *= pattern_extents->height;
	}

	if (viewport.width <= 0.0 || viewport.height <= 0.0)
		return;

	lsm_debug ("[LsmSvgPatternElement::render] Create pattern x = %g, y = %g, w = %g, h = %g",
		   viewport.x, viewport.y, viewport.width, viewport.height);

	lsm_svg_view_create_surface_pattern (view, &image_box,
					     pattern->units.value,
					     pattern->content_units.value,
					     &pattern->transform.matrix,
					     LSM_SVG_VIEW_SURFACE_TYPE_AUTO);

	is_object_bounding_box = (pattern->content_units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmSvgMatrix matrix;
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_matrix_init_scale (&matrix, pattern_extents->width, pattern_extents->height);
		lsm_svg_matrix_translate (&matrix, -pattern_extents->x, -pattern_extents->y);
		lsm_svg_view_push_viewbox (view, &viewbox);
		lsm_svg_view_push_matrix (view, &matrix);

		lsm_debug ("[LsmSvgPatternElement::render] object_bounding_box"
			   " x_scale = %g, y_scale = %g, x_offset = %g, y_offset = %g",
			   pattern_extents->width, pattern_extents->height,
			   pattern_extents->x,     pattern_extents->y);
	}

	is_viewbox_defined = lsm_attribute_is_defined (&pattern->viewbox.base);

	if (!(is_viewbox_defined) ||
	    (is_viewbox_defined && pattern->viewbox.value.width > 0.0 && pattern->viewbox.value.height > 0.0)) {

		lsm_svg_view_push_viewport (view, &viewport, is_viewbox_defined ? &pattern->viewbox.value : NULL,
					    &pattern->preserve_aspect_ratio.value);

		LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);

		lsm_svg_view_pop_viewport (view);
	}

	if (is_object_bounding_box) {
		lsm_svg_view_pop_matrix (view);
		lsm_svg_view_pop_viewbox (view);
	}

	lsm_svg_view_pop_style (view);
	lsm_svg_style_free (style);
}

static void
lsm_svg_pattern_element_enable_rendering (LsmSvgElement *element)
{
	LSM_SVG_PATTERN_ELEMENT (element)->enable_rendering  = TRUE;
}

/* LsmSvgPatternElement implementation */

LsmDomNode *
lsm_svg_pattern_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_PATTERN_ELEMENT, NULL);
}

static const LsmSvgLength length_default = 	{ .value_unit =   0.0, .type = LSM_SVG_LENGTH_TYPE_PX};
static const LsmSvgMatrix matrix_default = 	{1.0, 0.0, 0.0, 1.0, 0.0, 0.0, LSM_SVG_MATRIX_FLAGS_IDENTITY};
static const LsmBox viewbox_default =		{0.0, 0.0, 0.0, 0.0};
static const LsmSvgPatternUnits units_default =  	LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
static const LsmSvgPatternUnits content_units_default = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;
static const LsmSvgPreserveAspectRatio preserve_aspect_ratio_default = {
	.defer = FALSE,
	.align = LSM_SVG_ALIGN_X_MID_Y_MID,
	.meet_or_slice = LSM_SVG_MEET_OR_SLICE_MEET
};

static void
lsm_svg_pattern_element_init (LsmSvgPatternElement *self)
{
	self->enable_rendering = FALSE;
	self->x.length = length_default;
	self->y.length = length_default;
	self->width.length = length_default;
	self->height.length = length_default;
	self->transform.matrix = matrix_default;
	self->units.value = units_default;
	self->content_units.value = content_units_default;
	self->preserve_aspect_ratio.value = preserve_aspect_ratio_default;
}

/* LsmSvgPatternElement class */

static const LsmAttributeInfos lsm_svg_pattern_element_attribute_infos[] = {
	{
		.name = "x",
		.attribute_offset = offsetof (LsmSvgPatternElement, x),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "y",
		.attribute_offset = offsetof (LsmSvgPatternElement, y),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "width",
		.attribute_offset = offsetof (LsmSvgPatternElement, width),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "height",
		.attribute_offset = offsetof (LsmSvgPatternElement, height),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "patternUnits",
		.attribute_offset = offsetof (LsmSvgPatternElement, units),
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.trait_default = &units_default
	},
	{
		.name = "patternContentUnits",
		.attribute_offset = offsetof (LsmSvgPatternElement, content_units),
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.trait_default = &content_units_default
	},
	{
		.name = "patternTransform",
		.attribute_offset = offsetof (LsmSvgPatternElement, transform),
		.trait_class = &lsm_svg_matrix_trait_class,
		.trait_default = &matrix_default
	},
	{
		.name = "xlink:href",
		.attribute_offset = offsetof (LsmSvgPatternElement, href),
		.trait_class = &lsm_null_trait_class
	},
	{
		.name = "viewBox",
		.attribute_offset = offsetof (LsmSvgPatternElement, viewbox),
		.trait_class = &lsm_box_trait_class,
		.trait_default = &viewbox_default
	},
	{
		.name = "preserveAspectRatio",
		.attribute_offset = offsetof (LsmSvgPatternElement, preserve_aspect_ratio),
		.trait_class = &lsm_svg_preserve_aspect_ratio_trait_class,
		.trait_default = &preserve_aspect_ratio_default
	}
};

static void
lsm_svg_pattern_element_class_init (LsmSvgPatternElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _pattern_element_get_node_name;

	s_element_class->render = lsm_svg_pattern_element_render;
	s_element_class->enable_rendering = lsm_svg_pattern_element_enable_rendering;
	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_pattern_element_attribute_infos),
					      lsm_svg_pattern_element_attribute_infos);
}

G_DEFINE_TYPE (LsmSvgPatternElement, lsm_svg_pattern_element, LSM_TYPE_SVG_ELEMENT)
