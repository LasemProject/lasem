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

#include <math.h>

GObject *parent_class;

/* GDomNode implementation */

static const char *
gmathml_style_element_get_node_name (GDomNode *node)
{
	return "mstyle";
}

/* GMathmlElement implementation */

static void
gmathml_style_element_update (GMathmlElement *self, GMathmlView *view, GMathmlStyle *style)
{
	GMathmlStyleElement *style_element = GMATHML_STYLE_ELEMENT (self);

	gmathml_attribute_boolean_parse (&style_element->display_style, &style->display_style);

	gmathml_attribute_double_parse (&style_element->script_size_multiplier, &style->script_size_multiplier);
	gmathml_attribute_color_parse (&style_element->background, &style->background);
	gmathml_attribute_length_parse (&style_element->script_min_size, &style->script_min_size, style->font_size);

	gmathml_attribute_script_level_parse (&style_element->script_level, &style->script_level);

	/* token */

	gmathml_attribute_length_parse (&style_element->math_size, &style->math_size, style->font_size);

	style->font_size = style_element->math_size.value;

	gmathml_attribute_color_parse (&style_element->math_color, &style->math_color);
	gmathml_attribute_color_parse (&style_element->math_background, &style->math_background);

	/* mstyle */

	gmathml_attribute_length_parse (&style_element->very_very_thin_math_space,
					&style->very_very_thin_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->very_thin_math_space,
					&style->very_thin_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->thin_math_space,
					&style->thin_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->medium_math_space,
					&style->medium_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->thick_math_space,
					&style->thick_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->very_thick_math_space,
					&style->very_thick_math_space, style->font_size);
	gmathml_attribute_length_parse (&style_element->very_very_thick_math_space,
					&style->very_very_thick_math_space, style->font_size);

	style->very_very_thin_math_space_value = style_element->very_very_thin_math_space.value;
	style->very_thin_math_space_value = style_element->very_thin_math_space.value;
	style->thin_math_space_value = style_element->thin_math_space.value;
	style->medium_math_space_value = style_element->medium_math_space.value;
	style->thick_math_space_value = style_element->thick_math_space.value;
	style->very_thick_math_space_value = style_element->very_thick_math_space.value;
	style->very_very_thick_math_space_value = style_element->very_very_thick_math_space.value;

	/* mfrac */

	gmathml_attribute_length_parse (&style_element->line_thickness, &style->line_thickness, style->font_size);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, view, style);
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

static void
gmathml_style_element_class_init (GMathmlStyleElementClass *style_class)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (style_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (style_class);

	parent_class = g_type_class_peek_parent (style_class);

	node_class->get_node_name = gmathml_style_element_get_node_name;

	m_element_class->update = gmathml_style_element_update;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptlevel",
					     offsetof (GMathmlStyleElement, script_level));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "displaystyle",
					     offsetof (GMathmlStyleElement, display_style));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptsizemultiplier",
					     offsetof (GMathmlStyleElement, script_size_multiplier));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "scriptminsize",
					     offsetof (GMathmlStyleElement, script_min_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "background",
					     offsetof (GMathmlStyleElement, background));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythinmathspace",
					     offsetof (GMathmlStyleElement, very_very_thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "verythinmathspace",
					     offsetof (GMathmlStyleElement, very_thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "thinmathspace",
					     offsetof (GMathmlStyleElement, thin_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mediummathspace",
					     offsetof (GMathmlStyleElement, medium_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "thickmathspace",
					     offsetof (GMathmlStyleElement, thick_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "verythickmathspace",
					     offsetof (GMathmlStyleElement, very_thick_math_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythickmathspace",
					     offsetof (GMathmlStyleElement, very_very_thick_math_space));

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathvariant",
					     offsetof (GMathmlStyleElement, math_variant));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathsize",
					     offsetof (GMathmlStyleElement, math_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathcolor",
					     offsetof (GMathmlStyleElement, math_color));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathbackground",
					     offsetof (GMathmlStyleElement, math_background));

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "linethickness",
					     offsetof (GMathmlStyleElement, line_thickness));
}

G_DEFINE_TYPE (GMathmlStyleElement, gmathml_style_element, GMATHML_TYPE_PRESENTATION_CONTAINER)
