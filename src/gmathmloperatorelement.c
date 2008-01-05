/* gmathmloperatorelement.c
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

#include <gmathmloperatorelement.h>

GDomNode *
gmathml_operator_element_new (void)
{
	return g_object_new (GMATHML_TYPE_OPERATOR_ELEMENT, NULL);
}

static char *
gmathml_operator_element_get_node_name (GDomNode *node)
{
	return "mo";
}

static void
gmathml_operator_element_init (GMathmlOperatorElement *self)
{
}

void
gmathml_element_class_add_operator_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attributes_add_attribute (m_element_class->attributes, "form", GMATHML_ATTRIBUTE_STRING,
					  offsetof (GMathmlOperatorElement, form));
	gmathml_attributes_add_attribute (m_element_class->attributes, "fence", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, fence));
	gmathml_attributes_add_attribute (m_element_class->attributes, "separator", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, separator));
	gmathml_attributes_add_attribute (m_element_class->attributes, "stretchy", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, stretchy));
	gmathml_attributes_add_attribute (m_element_class->attributes, "symmetric", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, symmetric));
	gmathml_attributes_add_attribute (m_element_class->attributes, "accent", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, accent));
	gmathml_attributes_add_attribute (m_element_class->attributes, "movablelimits", GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlOperatorElement, movable_limits));
}

static void
gmathml_operator_element_class_init (GMathmlOperatorElementClass *operator_class)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (operator_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (operator_class);

	node_class->get_node_name = gmathml_operator_element_get_node_name;

	m_element_class->attributes = gmathml_attributes_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_style_attributes (m_element_class);
}

G_DEFINE_TYPE (GMathmlOperatorElement, gmathml_operator_element, GMATHML_TYPE_PRESENTATION_TOKEN)

