/* gdomelement.c
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#include <gdomelement.h>

/* GDomNode implementation */

static const char *
gdom_element_get_node_value (GDomNode *node)
{
	return NULL;
}

static GDomNodeType
gdom_element_get_node_type (GDomNode *node)
{
	return GDOM_NODE_TYPE_ELEMENT_NODE;
}

/* GDomElement implementation */

const char*
gdom_element_get_attribute (GDomElement* self, const char* name)
{
	g_return_val_if_fail (GDOM_IS_ELEMENT (self), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	return GDOM_ELEMENT_GET_CLASS (self)->get_attribute (self, name);
}

void
gdom_element_set_attribute (GDomElement* self, const char* name, const char* attribute_value)
{
	g_return_if_fail (GDOM_IS_ELEMENT (self));
	g_return_if_fail (name != NULL);
	g_return_if_fail (attribute_value != NULL);

	GDOM_ELEMENT_GET_CLASS (self)->set_attribute (self, name, attribute_value);
}

static void
gdom_element_init (GDomElement *element)
{
}

/* GDomElement class */

static void
gdom_element_class_init (GDomElementClass *klass)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (klass);

	node_class->get_node_value = gdom_element_get_node_value;
	node_class->get_node_type = gdom_element_get_node_type;
}

G_DEFINE_ABSTRACT_TYPE (GDomElement, gdom_element, GDOM_TYPE_NODE)
