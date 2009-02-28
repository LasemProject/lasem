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

#include <gsvgsymbolelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <gdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_symbol_element_get_node_name (GDomNode *node)
{
	return "symbol";
}

/* GSvgElement implementation */

static void
gsvg_symbol_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* GSvgGraphic implementation */

/* GSvgSymbolElement implementation */

GDomNode *
gsvg_symbol_element_new (void)
{
	return g_object_new (GSVG_TYPE_SYMBOL_ELEMENT, NULL);
}

static void
gsvg_symbol_element_init (GSvgSymbolElement *self)
{
}

/* GSvgSymbolElement class */

static void
gsvg_symbol_element_class_init (GSvgSymbolElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = gsvg_symbol_element_get_node_name;

	s_element_class->update = gsvg_symbol_element_update;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);
}

G_DEFINE_TYPE (GSvgSymbolElement, gsvg_symbol_element, GSVG_TYPE_GRAPHIC)
