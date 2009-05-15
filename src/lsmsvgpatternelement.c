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
_pattern_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgPatternElement *pattern = LSM_SVG_PATTERN_ELEMENT (self);
	LsmSvgPatternUnits units;
	LsmSvgLength length;

	units = LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
	lsm_svg_pattern_units_attribute_parse (&pattern->units, &units);

	units = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;
	lsm_svg_pattern_units_attribute_parse (&pattern->content_units, &units);

	lsm_svg_transform_attribute_parse (&pattern->transform);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&pattern->x, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&pattern->y, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&pattern->width, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&pattern->height, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_pattern_element_render_paint (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgPatternElement *pattern = LSM_SVG_PATTERN_ELEMENT (self);
	double x, y, width, height;
	gboolean is_object_bounding_box;

	is_object_bounding_box = (pattern->units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox viewbox = {.x = 0.0, .y = .0, .width = 1.0, .height = 1.0};

		lsm_svg_view_push_viewbox (view, &viewbox);
	}

	x      = lsm_svg_view_normalize_length (view, &pattern->x.length.base,
						LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y      = lsm_svg_view_normalize_length (view, &pattern->y.length.base,
						LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	width  = lsm_svg_view_normalize_length (view, &pattern->width.length.base,
						LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	height = lsm_svg_view_normalize_length (view, &pattern->height.length.base,
						LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	if (is_object_bounding_box)
		lsm_svg_view_pop_viewbox (view);

	if (width <= 0.0 || height <= 0.0)
		return;

	lsm_debug ("[LsmSvgPatternElement::render_paint] Create pattern x = %g, y = %g, w = %g, h = %g",
		   x, y, width, height);

	lsm_svg_view_create_surface_pattern (view, width, height,
					     pattern->units.value,
					     pattern->content_units.value,
					     &pattern->transform.matrix);

	is_object_bounding_box = (pattern->content_units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmBox extents;
		LsmSvgMatrix matrix;

		extents = lsm_svg_view_get_extents (view);
		lsm_svg_matrix_init_scale (&matrix, extents.width, extents.height);
		lsm_svg_view_push_viewbox (view, &extents);
		lsm_svg_view_push_transform (view, &matrix);
	}

	LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);

	if (is_object_bounding_box) {
		lsm_svg_view_pop_transform (view);
		lsm_svg_view_pop_viewbox (view);
	}
}

/* LsmSvgPatternElement implementation */

LsmDomNode *
lsm_svg_pattern_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_PATTERN_ELEMENT, NULL);
}

static void
lsm_svg_pattern_element_init (LsmSvgPatternElement *self)
{
	/* Hack - Force the creation of the attribute bags,
	   making sure the properties will be inherited from the
	   pattern element ancestor, not the referencing one. */

	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "fill", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "transform", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "font-family", NULL);
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stop-color", NULL);
}

/* LsmSvgPatternElement class */

static void
lsm_svg_pattern_element_class_init (LsmSvgPatternElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _pattern_element_get_node_name;

	s_element_class->update = _pattern_element_update;
	s_element_class->render_paint = _pattern_element_render_paint;
	s_element_class->render = NULL;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgPatternElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgPatternElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (LsmSvgPatternElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (LsmSvgPatternElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "patternUnits",
					  offsetof (LsmSvgPatternElement, units));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "patternContentUnits",
					  offsetof (LsmSvgPatternElement, content_units));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "patternTransform",
					  offsetof (LsmSvgPatternElement, transform));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (LsmSvgPatternElement, href));
}

G_DEFINE_TYPE (LsmSvgPatternElement, lsm_svg_pattern_element, LSM_TYPE_SVG_GRAPHIC)
