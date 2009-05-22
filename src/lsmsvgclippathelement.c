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

#include <lsmsvgclippathelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
_clip_path_element_get_node_name (LsmDomNode *node)
{
	return "clipPath";
}

/* LsmSvgElement implementation */

static void
_clip_path_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgClipPathElement *pattern = LSM_SVG_CLIP_PATH_ELEMENT (self);
	LsmSvgPatternUnits units;

	units = LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX;
	lsm_svg_pattern_units_attribute_parse (&pattern->units, &units);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

LsmDomNode *
lsm_svg_clip_path_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_CLIP_PATH_ELEMENT, NULL);
}

static void
lsm_svg_clip_path_element_init (LsmSvgClipPathElement *self)
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

/* LsmSvgClipPathElement class */

static void
lsm_svg_clip_path_element_class_init (LsmSvgClipPathElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _clip_path_element_get_node_name;

	s_element_class->update = _clip_path_element_update;
	s_element_class->render_clip = s_element_class->render;
	s_element_class->render = NULL;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "clipPathUnits",
					     offsetof (LsmSvgClipPathElement, units));
}

G_DEFINE_TYPE (LsmSvgClipPathElement, lsm_svg_clip_path_element, LSM_TYPE_SVG_GRAPHIC)
