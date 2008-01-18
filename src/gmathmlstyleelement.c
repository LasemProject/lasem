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

#include <gmathmlstyleelement.h>

/* GDomNode implementation */

static const char *
gmathml_style_element_get_node_name (GDomNode *node)
{
	return "mstyle";
}

/* GMathmlElement implementation */

static void
gmathml_style_element_update_attributes (GMathmlElement *self)
{
	GMathmlStyleElement *style = GMATHML_STYLE_ELEMENT (self);
	GDomNode *parent;

	for (parent = GDOM_NODE (self)->parent_node;
	     parent != NULL && !GMATHML_IS_STYLE_ELEMENT (parent);
	     parent = parent->parent_node);

	if (GMATHML_IS_STYLE_ELEMENT (parent)) {
		gmathml_attribute_script_level_parse (&style->style_attrs.script_level,
						      GMATHML_STYLE_ELEMENT (parent)->style_attrs.script_level.value);
		gmathml_attribute_boolean_parse (&style->style_attrs.display_style,
						 GMATHML_STYLE_ELEMENT (parent)->style_attrs.display_style.value);
	} else {
		gmathml_attribute_script_level_parse (&style->style_attrs.script_level, 0.0);
		gmathml_attribute_boolean_parse (&style->style_attrs.display_style, FALSE);
	}

	gmathml_attribute_double_parse (&style->style_attrs.script_size_multiplier, 0.71);
	gmathml_attribute_color_parse (&style->style_attrs.background, 0.0, 0.0, 0.0, 0.0);

/*        gmathml_boolean_attribute_set_default (&m_element->style_attrs.display_style, FALSE);*/
/*        gmathml_double_attribute_set_default (&m_element->style_attrs.script_size_multiplier, 0.71);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.script_min_size, 1.0, GMATHML_UNIT_PX);*/
	/* FIXME background */
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.very_very_thin_math_space,*/
/*                                              0.0555556, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.very_thin_math_space,*/
/*                                              0.111111, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.thin_math_space,*/
/*                                              0.166667, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.medium_math_space,*/
/*                                              0.222222, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.thick_math_space,*/
/*                                              0.277778, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.very_thick_math_space,*/
/*                                              0.333333, GMATHML_UNIT_EM);*/
/*        gmathml_length_attribute_set_default (&m_element->style_attrs.very_very_thick_math_space,*/
/*                                              0.388889, GMATHML_UNIT_EM);*/
}

/* GMathmlStyleElement implementation */

GDomNode *
gmathml_style_element_new (void)
{
	return g_object_new (GMATHML_TYPE_STYLE_ELEMENT, NULL);
}

static void
gmathml_style_element_init (GMathmlStyleElement *self)
{
}

/* GMathmlStyleElement class */

void
gmathml_element_class_add_style_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptlevel",
					     offsetof (GMathmlStyleElement, style_attrs.script_level));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "display",
					     offsetof (GMathmlStyleElement, style_attrs.display_style));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptsizemultiplier",
					     offsetof (GMathmlStyleElement, style_attrs.script_size_multiplier));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptminsize",
					     offsetof (GMathmlStyleElement, style_attrs.script_min_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "background",
					     offsetof (GMathmlStyleElement, style_attrs.background));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythinmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.very_very_thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "verythinmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.very_thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "thinmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mediummathspace",
					     offsetof (GMathmlStyleElement, style_attrs.medium_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "thickmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.thick_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "verythickmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.very_thick_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythickmathspace",
					     offsetof (GMathmlStyleElement, style_attrs.very_very_thick_math_space));
}

static void
gmathml_style_element_class_init (GMathmlStyleElementClass *operator_class)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (operator_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (operator_class);

	node_class->get_node_name = gmathml_style_element_get_node_name;

	m_element_class->update_attributes = gmathml_style_element_update_attributes;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_style_attributes (m_element_class);
}

G_DEFINE_TYPE (GMathmlStyleElement, gmathml_style_element, GMATHML_TYPE_PRESENTATION_CONTAINER)
