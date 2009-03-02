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

#include <gsvglineargradientelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_linear_gradient_element_get_node_name (GDomNode *node)
{
	return "linearGradient";
}

/* GLinearGradientElement implementation */

static void
_linear_gradient_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
/*        GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);*/
}

/* GSvgLinearGradientElement implementation */

GDomNode *
gsvg_linear_gradient_element_new (void)
{
	return g_object_new (GSVG_TYPE_LINEAR_GRADIENT_ELEMENT, NULL);
}

static void
gsvg_linear_gradient_element_init (GSvgLinearGradientElement *self)
{

}

static void
gsvg_linear_gradient_element_finalize (GObject *object)
{
}

/* GSvgLinearGradientElement class */

static void
gsvg_linear_gradient_element_class_init (GSvgLinearGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (s_svg_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = gsvg_linear_gradient_element_finalize;

	d_node_class->get_node_name = gsvg_linear_gradient_element_get_node_name;

	s_element_class->update = _linear_gradient_element_update;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "x1",
					  offsetof (GSvgLinearGradientElement, x1));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y1",
					  offsetof (GSvgLinearGradientElement, y1));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "x2",
					  offsetof (GSvgLinearGradientElement, x2));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "y2",
					  offsetof (GSvgLinearGradientElement, y2));
}

G_DEFINE_TYPE (GSvgLinearGradientElement, gsvg_linear_gradient_element, GSVG_TYPE_GRADIENT_ELEMENT)
