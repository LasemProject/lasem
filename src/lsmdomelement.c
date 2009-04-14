/* lsmdomelement.c
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

#include <lsmdomelement.h>

/* LsmDomNode implementation */

static const char *
lsm_dom_element_get_node_value (LsmDomNode *node)
{
	return NULL;
}

static LsmDomNodeType
lsm_dom_element_get_node_type (LsmDomNode *node)
{
	return LSM_DOM_NODE_TYPE_ELEMENT_NODE;
}

/* LsmDomElement implementation */

const char*
lsm_dom_element_get_attribute (LsmDomElement* self, const char* name)
{
	g_return_val_if_fail (LSM_IS_DOM_ELEMENT (self), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	return LSM_DOM_ELEMENT_GET_CLASS (self)->get_attribute (self, name);
}

void
lsm_dom_element_set_attribute (LsmDomElement* self, const char* name, const char* attribute_value)
{
	g_return_if_fail (LSM_IS_DOM_ELEMENT (self));
	g_return_if_fail (name != NULL);

	LSM_DOM_ELEMENT_GET_CLASS (self)->set_attribute (self, name, attribute_value);

	lsm_dom_node_changed (LSM_DOM_NODE (self));
}

static void
lsm_dom_element_init (LsmDomElement *element)
{
}

/* LsmDomElement class */

static void
lsm_dom_element_class_init (LsmDomElementClass *klass)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_CLASS (klass);

	node_class->get_node_value = lsm_dom_element_get_node_value;
	node_class->get_node_type = lsm_dom_element_get_node_type;
}

G_DEFINE_ABSTRACT_TYPE (LsmDomElement, lsm_dom_element, LSM_TYPE_DOM_NODE)
