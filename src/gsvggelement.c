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

#include <gsvggelement.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_g_element_get_node_name (GDomNode *node)
{
	return "g";
}

/* GGElement implementation */

/* GSvgGraphic implementation */

/* GSvgGElement implementation */

GDomNode *
gsvg_g_element_new (void)
{
	return g_object_new (GSVG_TYPE_G_ELEMENT, NULL);
}

static void
gsvg_g_element_init (GSvgGElement *self)
{
}

/* GSvgGElement class */

static void
gsvg_g_element_class_init (GSvgGElementClass *s_g_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_g_class);

	parent_class = g_type_class_peek_parent (s_g_class);

	d_node_class->get_node_name = gsvg_g_element_get_node_name;

}

G_DEFINE_TYPE (GSvgGElement, gsvg_g_element, GSVG_TYPE_GRAPHIC)
