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

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&mask->x, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&mask->y, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&mask->width, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&mask->height, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_mask_element_render_paint (LsmSvgElement *self, LsmSvgView *view)
{
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
