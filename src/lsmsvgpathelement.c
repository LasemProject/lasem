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

#include <lsmsvgpathelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_path_element_get_node_name (LsmDomNode *node)
{
	return "path";
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_path_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgPathElement *path;

	path = LSM_SVG_PATH_ELEMENT (self);

	lsm_debug ("[LsmSvgPathElement::graphic_render]");

	lsm_svg_view_show_path (view, lsm_dom_attribute_get_value (&path->d));
}

/* LsmSvgPathElement implementation */

LsmDomNode *
lsm_svg_path_element_new (void)
{
	return g_object_new (LSM_SVG_TYPE_PATH_ELEMENT, NULL);
}

static void
lsm_svg_path_element_init (LsmSvgPathElement *self)
{
}

static void
lsm_svg_path_element_finalize (GObject *object)
{
}

/* LsmSvgPathElement class */

static void
lsm_svg_path_element_class_init (LsmSvgPathElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_path_element_finalize;

	d_node_class->get_node_name = lsm_svg_path_element_get_node_name;

	s_graphic_class->graphic_render = lsm_svg_path_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "d",
					  offsetof (LsmSvgPathElement, d));
}

G_DEFINE_TYPE (LsmSvgPathElement, lsm_svg_path_element, LSM_SVG_TYPE_GRAPHIC)
