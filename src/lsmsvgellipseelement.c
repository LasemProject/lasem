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

#include <lsmsvgellipseelement.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_ellipse_element_get_node_name (LsmDomNode *node)
{
	return "ellipse";
}

/* LsmSvgElement implementation */

static void
lsm_svg_ellipse_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgEllipseElement *ellipse = LSM_SVG_ELLIPSE_ELEMENT (self);
	LsmSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&ellipse->cx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&ellipse->cy, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&ellipse->rx, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&ellipse->ry, &length, 0.0);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_ellipse_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgEllipseElement *ellipse = LSM_SVG_ELLIPSE_ELEMENT (self);

	lsm_svg_view_show_ellipse (view,
				ellipse->cx.length.base.value,
				ellipse->cy.length.base.value,
				ellipse->rx.length.base.value,
				ellipse->ry.length.base.value);
}

/* LsmSvgEllipseElement implementation */

LsmDomNode *
lsm_svg_ellipse_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_ELLIPSE_ELEMENT, NULL);
}

static void
lsm_svg_ellipse_element_init (LsmSvgEllipseElement *self)
{
}

static void
lsm_svg_ellipse_element_finalize (GObject *object)
{
}

/* LsmSvgEllipseElement class */

static void
lsm_svg_ellipse_element_class_init (LsmSvgEllipseElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_ellipse_element_finalize;

	d_node_class->get_node_name = lsm_svg_ellipse_element_get_node_name;

	s_element_class->update = lsm_svg_ellipse_element_update;

	s_graphic_class->graphic_render = lsm_svg_ellipse_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cx",
					  offsetof (LsmSvgEllipseElement, cx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "cy",
					  offsetof (LsmSvgEllipseElement, cy));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "rx",
					  offsetof (LsmSvgEllipseElement, rx));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "ry",
					  offsetof (LsmSvgEllipseElement, ry));
}

G_DEFINE_TYPE (LsmSvgEllipseElement, lsm_svg_ellipse_element, LSM_TYPE_SVG_GRAPHIC)
