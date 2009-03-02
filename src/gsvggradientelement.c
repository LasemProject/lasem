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

/* GGradientElement implementation */

/* GSvgGradientElement implementation */

static void
gsvg_gradient_element_init (GSvgGradientElement *self)
{
}

/* GSvgGradientElement class */

static void
gsvg_gradient_element_class_init (GSvgGradientElementClass *s_svg_class)
{
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "gradientUnits",
					  offsetof (GSvgGradientElement, gradient_units));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "gradientTransform",
					  offsetof (GSvgGradientElement, gradient_transform));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "spreadMethod",
					  offsetof (GSvgGradientElement, spread_method));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (GSvgGradientElement, href));
}

G_DEFINE_ABSTRACT_TYPE (GSvgGradientElement, gsvg_gradient_element, GSVG_TYPE_ELEMENT)
