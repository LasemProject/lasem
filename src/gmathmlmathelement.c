/* gmathmlmathelement.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmlmathelement.h>

GDomNode *
gmathml_math_element_new (void)
{
	return g_object_new (GMATHML_TYPE_MATH_ELEMENT, NULL);
}

static char *
gmathml_math_element_get_node_name (GDomNode *node)
{
	return "math";
}

static gboolean
gmathml_math_element_can_add_node (GDomNode *self, GDomNode *child)
{
	return (GMATHML_IS_ELEMENT (child));
}

static void
gmathml_math_element_init (GMathmlMathElement *self)
{
}

static void
gmathml_math_element_class_init (GMathmlMathElementClass *klass)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (klass);

	node_class->get_node_name = gmathml_math_element_get_node_name;
	node_class->can_add_node = gmathml_math_element_can_add_node;
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
