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

#include <lsmsvgcircleelement.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_circle_element_get_node_name (LsmDomNode *node)
{
	return "circle";
}

/* LsmSvgElement implementation */

static void
lsm_svg_circle_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgCircleElement *circle = LSM_SVG_CIRCLE_ELEMENT (self);
	LsmSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&circle->cx, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&circle->cy, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&circle->r, &length, parent_style,
						 LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_circle_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgCircleElement *circle = LSM_SVG_CIRCLE_ELEMENT (self);

	lsm_svg_view_show_circle (view,
				  circle->cx.length.base.value,
				  circle->cy.length.base.value,
				  circle->r.length.base.value);
}

/* LsmSvgCircleElement implementation */

LsmDomNode *
lsm_svg_circle_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_CIRCLE_ELEMENT, NULL);
}

static void
lsm_svg_circle_element_init (LsmSvgCircleElement *self)
{
}

static void
lsm_svg_circle_element_finalize (GObject *object)
{
}

/* LsmSvgCircleElement class */

static void
lsm_svg_circle_element_class_init (LsmSvgCircleElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_circle_element_finalize;

	d_node_class->get_node_name = lsm_svg_circle_element_get_node_name;

	s_element_class->update = lsm_svg_circle_element_update;

	s_graphic_class->graphic_render = lsm_svg_circle_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cx",
					  offsetof (LsmSvgCircleElement, cx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cy",
					  offsetof (LsmSvgCircleElement, cy));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "r",
					  offsetof (LsmSvgCircleElement, r));
}

G_DEFINE_TYPE (LsmSvgCircleElement, lsm_svg_circle_element, LSM_TYPE_SVG_GRAPHIC)
