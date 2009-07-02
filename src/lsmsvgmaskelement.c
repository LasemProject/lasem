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

#include <lsmsvgmaskelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
_mask_element_get_node_name (LsmDomNode *node)
{
	return "mask";
}

/* LsmSvgElement implementation */

static void
_mask_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgMaskElement *mask = LSM_SVG_MASK_ELEMENT (self);
	LsmSvgPatternUnits units;
	LsmSvgLength length;

	units = LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
	lsm_svg_pattern_units_attribute_parse (&mask->units, &units);

	units = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;
	lsm_svg_pattern_units_attribute_parse (&mask->content_units, &units);

	length.value_unit = -10.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&mask->x, &length);

	length.value_unit = -10.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&mask->y, &length);

	length.value_unit = 120.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&mask->width, &length);

	length.value_unit = 120.0;
	length.type = LSM_SVG_LENGTH_TYPE_PERCENTAGE;
	lsm_svg_animated_length_attribute_parse (&mask->height, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_mask_element_render_paint (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgMaskElement *mask = LSM_SVG_MASK_ELEMENT (self);
	gboolean is_object_bounding_box;
	LsmBox viewport;
	const LsmBox *mask_extents;

	mask_extents = lsm_svg_view_get_pattern_extents (view);

	is_object_bounding_box = (mask->units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_view_push_viewbox (view, &viewbox);
	}

	viewport.x      = lsm_svg_view_normalize_length (view, &mask->x.length.base,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.y      = lsm_svg_view_normalize_length (view, &mask->y.length.base,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	viewport.width  = lsm_svg_view_normalize_length (view, &mask->width.length.base,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.height = lsm_svg_view_normalize_length (view, &mask->height.length.base,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	if (is_object_bounding_box) {
		lsm_svg_view_pop_viewbox (view);

		viewport.x = viewport.x * mask_extents->width + mask_extents->x;
		viewport.y = viewport.y * mask_extents->height + mask_extents->y;
		viewport.width *= mask_extents->width;
		viewport.height *= mask_extents->height;
	}

	if (viewport.width <= 0.0 || viewport.height <= 0.0)
		return;

	lsm_debug ("[LsmSvgMaskElement::render_paint] Create mask x = %g, y = %g, w = %g, h = %g",
		   viewport.x, viewport.y, viewport.width, viewport.height);

	lsm_svg_view_create_surface_pattern (view, &viewport,
					     mask->units.value,
					     mask->content_units.value, NULL,
					     LSM_SVG_VIEW_SURFACE_TYPE_IMAGE);

	is_object_bounding_box = (mask->content_units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmSvgMatrix matrix;
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_matrix_init_translate (&matrix, +mask_extents->x, +mask_extents->y);
		lsm_svg_matrix_scale (&matrix, mask_extents->width, mask_extents->height);
		lsm_svg_view_push_viewbox (view, &viewbox);
		lsm_svg_view_push_matrix (view, &matrix);

		lsm_debug ("[LsmSvgMaskElement::render_paint] object_bounding_box"
			   " x_scale = %g, y_scale = %g, x_offset = %g, y_offset = %g",
			   mask_extents->width, mask_extents->height,
			   mask_extents->x, mask_extents->y);
	}

	LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);

	if (is_object_bounding_box) {
		lsm_svg_view_pop_matrix (view);
		lsm_svg_view_pop_viewbox (view);
	}
}

/* LsmSvgMaskElement implementation */

LsmDomNode *
lsm_svg_mask_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_MASK_ELEMENT, NULL);
}

static void
lsm_svg_mask_element_init (LsmSvgMaskElement *self)
{
	/* Hack - Force the creation of the attribute bags,
	   making sure the properties will be inherited from the
	   mask element ancestor, not the referencing one. */

	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "fill", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "transform", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "font-family", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stop-color", NULL);
}

/* LsmSvgMaskElement class */

static void
lsm_svg_mask_element_class_init (LsmSvgMaskElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _mask_element_get_node_name;

	s_element_class->update = _mask_element_update;
	s_element_class->render_paint = _mask_element_render_paint;
	s_element_class->render = NULL;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					     offsetof (LsmSvgMaskElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					     offsetof (LsmSvgMaskElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					     offsetof (LsmSvgMaskElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					     offsetof (LsmSvgMaskElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "maskUnits",
					     offsetof (LsmSvgMaskElement, units));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "maskContentUnits",
					     offsetof (LsmSvgMaskElement, content_units));
}

G_DEFINE_TYPE (LsmSvgMaskElement, lsm_svg_mask_element, LSM_TYPE_SVG_GRAPHIC)
