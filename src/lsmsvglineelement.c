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

#include <lsmsvglineelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_line_element_get_node_name (LsmDomNode *node)
{
	return "line";
}

/* LsmSvgElement implementation */

static void
lsm_svg_line_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgLineElement *line = LSM_SVG_LINE_ELEMENT (self);
	LsmSvgLength length;

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&line->x1, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&line->y1, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&line->x2, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&line->y2, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_line_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgLineElement *line = LSM_SVG_LINE_ELEMENT (self);
	double x1, y1, x2, y2;

	x1 = lsm_svg_view_normalize_length (view, &line->x1.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y1 = lsm_svg_view_normalize_length (view, &line->y1.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	x2 = lsm_svg_view_normalize_length (view, &line->x2.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y2 = lsm_svg_view_normalize_length (view, &line->y2.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	lsm_debug ("[LsmSvgLineElement::graphic_render] %g, %g, %g, %g", x1, y1, x2, y2);

	lsm_svg_view_show_line (view, x1, y1, x2, y2);
}

/* LsmSvgLineElement implementation */

LsmDomNode *
lsm_svg_line_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_LINE_ELEMENT, NULL);
}

static void
lsm_svg_line_element_init (LsmSvgLineElement *self)
{
}

/* LsmSvgLineElement class */

static void
lsm_svg_line_element_class_init (LsmSvgLineElementClass *s_rect_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	d_node_class->get_node_name = lsm_svg_line_element_get_node_name;

	s_element_class->update = lsm_svg_line_element_update;

	s_graphic_class->graphic_render = lsm_svg_line_element_graphic_render;
	s_graphic_class->late_opacity_handling = TRUE;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x1",
					  offsetof (LsmSvgLineElement, x1));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y1",
					  offsetof (LsmSvgLineElement, y1));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x2",
					  offsetof (LsmSvgLineElement, x2));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y2",
					  offsetof (LsmSvgLineElement, y2));
}

G_DEFINE_TYPE (LsmSvgLineElement, lsm_svg_line_element, LSM_TYPE_SVG_GRAPHIC)
