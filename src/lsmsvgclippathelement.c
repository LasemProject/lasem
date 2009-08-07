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
lsm_svg_clip_path_element_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgClipPathElement *clip = LSM_SVG_CLIP_PATH_ELEMENT (self);
	gboolean is_object_bounding_box;
	LsmSvgStyle *style;

	if (!clip->enable_rendering) {
		lsm_debug ("[LsmSvgClipPathElement::render] Direct rendering not allowed");
		return;
	} else {
		clip->enable_rendering = FALSE;
	}

	style = lsm_svg_style_new_inherited (NULL, &self->property_bag);
	lsm_svg_view_push_style (view, style);

	is_object_bounding_box = (clip->units.value == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX);

	if (is_object_bounding_box) {
		LsmSvgMatrix matrix;
		const LsmBox *viewbox;

		viewbox = lsm_svg_view_get_clip_extents (view);
		lsm_svg_matrix_init_translate (&matrix, viewbox->x, viewbox->y);
		lsm_svg_matrix_scale (&matrix, viewbox->width, viewbox->height);
		lsm_svg_view_push_viewbox (view, viewbox);
		lsm_svg_view_push_matrix (view, &matrix);
	}

	LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);

	if (is_object_bounding_box) {
		lsm_svg_view_pop_matrix (view);
		lsm_svg_view_pop_viewbox (view);
	}

	lsm_svg_view_pop_style (view);
	lsm_svg_style_free (style);
}

static void
lsm_svg_clip_path_element_enable_rendering (LsmSvgElement *element)
{
	LSM_SVG_CLIP_PATH_ELEMENT (element)->enable_rendering  = TRUE;
}

LsmDomNode *
lsm_svg_clip_path_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_CLIP_PATH_ELEMENT, NULL);
}

static const LsmSvgPatternUnits units_default = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;

static void
lsm_svg_clip_path_element_init (LsmSvgClipPathElement *self)
{
	self->enable_rendering = FALSE;
	self->units.value = units_default;
}

/* LsmSvgClipPathElement class */

static const LsmAttributeInfos lsm_svg_clip_path_element_attribute_infos[] = {
	{
		.name = "clipPathUnits",
		.trait_class = &lsm_svg_pattern_units_trait_class,
		.attribute_offset = offsetof (LsmSvgClipPathElement, units),
		.trait_default = &units_default
	}
};

static void
lsm_svg_clip_path_element_class_init (LsmSvgClipPathElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _clip_path_element_get_node_name;

	s_element_class->render = lsm_svg_clip_path_element_render;
	s_element_class->enable_rendering = lsm_svg_clip_path_element_enable_rendering;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_clip_path_element_attribute_infos),
					      lsm_svg_clip_path_element_attribute_infos);
}

G_DEFINE_TYPE (LsmSvgClipPathElement, lsm_svg_clip_path_element, LSM_TYPE_SVG_ELEMENT)
