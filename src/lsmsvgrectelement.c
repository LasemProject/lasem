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

#include <lsmsvgrectelement.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_rect_element_get_node_name (LsmDomNode *node)
{
	return "rect";
}

/* LsmSvgElement implementation */

static void
lsm_svg_rect_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgRectElement *rect = LSM_SVG_RECT_ELEMENT (self);
	LsmSvgLength length;

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->x, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->y, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->width, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->height, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->rx, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&rect->ry, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_rect_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgRectElement *rect = LSM_SVG_RECT_ELEMENT (self);
	double x, y;
	double rx, ry;
	double w, h;

	x = lsm_svg_view_normalize_length (view, &rect->x.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y = lsm_svg_view_normalize_length (view, &rect->y.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	rx = lsm_svg_view_normalize_length (view, &rect->rx.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	ry = lsm_svg_view_normalize_length (view, &rect->ry.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	w = lsm_svg_view_normalize_length (view, &rect->width.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	h = lsm_svg_view_normalize_length (view, &rect->height.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	/* SVG specification is so weird sometimes ... */
	if (w == 0.0 || h == 0.0)
		return;

	if (!lsm_dom_attribute_is_defined (&rect->rx.attr))
		rx = ry;
	else if (!lsm_dom_attribute_is_defined (&rect->ry.attr))
		ry = rx;

	lsm_svg_view_show_rectangle (view, x, y, w, h, rx, ry);
}

static void
lsm_svg_rect_element_graphic_get_extents (LsmSvgElement *self, LsmSvgView *view, LsmExtents *extents)
{
	LsmSvgRectElement *rect = LSM_SVG_RECT_ELEMENT (self);
	double x, y;
	double w, h;

	x = lsm_svg_view_normalize_length (view, &rect->x.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y = lsm_svg_view_normalize_length (view, &rect->y.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	w = lsm_svg_view_normalize_length (view, &rect->width.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	h = lsm_svg_view_normalize_length (view, &rect->height.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	extents->x1 = x;
	extents->y1 = y;
	extents->x2 = x + w;
	extents->y2 = y + h;
}

/* LsmSvgRectElement implementation */

LsmDomNode *
lsm_svg_rect_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_RECT_ELEMENT, NULL);
}

static void
lsm_svg_rect_element_init (LsmSvgRectElement *self)
{
}

static void
lsm_svg_rect_element_finalize (GObject *object)
{
}

/* LsmSvgRectElement class */

static void
lsm_svg_rect_element_class_init (LsmSvgRectElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_rect_element_finalize;

	d_node_class->get_node_name = lsm_svg_rect_element_get_node_name;

	s_element_class->update = lsm_svg_rect_element_update;

	s_graphic_class->graphic_render = lsm_svg_rect_element_graphic_render;
	s_graphic_class->graphic_get_extents = lsm_svg_rect_element_graphic_get_extents;
	s_graphic_class->late_opacity_handling = TRUE;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgRectElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgRectElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (LsmSvgRectElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (LsmSvgRectElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "rx",
					  offsetof (LsmSvgRectElement, rx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "ry",
					  offsetof (LsmSvgRectElement, ry));
}

G_DEFINE_TYPE (LsmSvgRectElement, lsm_svg_rect_element, LSM_TYPE_SVG_GRAPHIC)
