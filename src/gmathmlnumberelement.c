/* gmathmlnumberelement.c
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

#include <gmathmlnumberelement.h>

GDomNode *
gmathml_number_element_new (void)
{
	return g_object_new (GMATHML_TYPE_NUMBER_ELEMENT, NULL);
}

static char *
gmathml_number_element_get_node_name (GDomNode *node)
{
	return "mn";
}

static void
gmathml_number_element_init (GMathmlNumberElement *element)
{
}

static void
gmathml_number_element_class_init (GMathmlNumberElementClass *klass)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (klass);

	node_class->get_node_name = gmathml_number_element_get_node_name;
}

G_DEFINE_TYPE (GMathmlNumberElement, gmathml_number_element, GMATHML_TYPE_PRESENTATION_TOKEN)

