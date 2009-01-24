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

#include <gsvgsvgelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_svg_element_get_node_name (GDomNode *node)
{
	return "svg";
}

/* GSvgElement implementation */

/* GSvgSvgElement implementation */

GDomNode *
gsvg_svg_element_new (void)
{
	return g_object_new (GSVG_TYPE_SVG_ELEMENT, NULL);
}

static void
gsvg_svg_element_init (GSvgSvgElement *self)
{
}

static void
gsvg_svg_element_finalize (GObject *object)
{
}

/* GSvgSvgElement class */

static void
gsvg_svg_element_class_init (GSvgSvgElementClass *svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (svg_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (svg_class);

	parent_class = g_type_class_peek_parent (svg_class);

	object_class->finalize = gsvg_svg_element_finalize;

	d_node_class->get_node_name = gsvg_svg_element_get_node_name;

}

G_DEFINE_TYPE (GSvgSvgElement, gsvg_svg_element, GSVG_TYPE_ELEMENT)
