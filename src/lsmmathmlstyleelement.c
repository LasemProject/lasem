/* lsmmathmloperatorelement.c
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

#include <lsmmathmlstyleelement.h>

#include <math.h>

GObject *parent_class;

/* LsmDomNode implementation */

static const char *
lsm_mathml_style_element_get_node_name (LsmDomNode *node)
{
	return "mstyle";
}

/* LsmMathmlElement implementation */

static void
lsm_mathml_style_element_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlFontStyle font_style;
	LsmMathmlFontWeight font_weight;
	gboolean display_style;

	LsmMathmlStyleElement *style_element = LSM_MATHML_STYLE_ELEMENT (self);

	display_style = style->display == LSM_MATHML_DISPLAY_BLOCK;
	lsm_mathml_boolean_attribute_parse (&style_element->display_style, &display_style);
	style->display = display_style ? LSM_MATHML_DISPLAY_BLOCK : LSM_MATHML_DISPLAY_INLINE;

	lsm_mathml_double_attribute_parse (&style_element->script_size_multiplier, &style->script_size_multiplier);
	lsm_mathml_color_attribute_parse (&style_element->math_background, &style->math_background);
	lsm_mathml_length_attribute_parse (&style_element->script_min_size,
					&style->script_min_size,
					style->math_size_value);

	lsm_mathml_script_level_attribute_parse (&style_element->script_level, &style->script_level);

	/* token */

	font_style = LSM_MATHML_FONT_STYLE_ERROR;
	lsm_mathml_font_style_attribute_parse (&style_element->font_style, &font_style);

	font_weight = LSM_MATHML_FONT_WEIGHT_ERROR;
	lsm_mathml_font_weight_attribute_parse (&style_element->font_weight, &font_weight);

	lsm_mathml_variant_set_font_style (&style->math_variant, font_style);
	lsm_mathml_variant_set_font_weight (&style->math_variant, font_weight);

	lsm_mathml_string_attribute_parse (&style_element->math_family, &style->math_family);
	lsm_mathml_length_attribute_parse (&style_element->math_size, &style->math_size, style->math_size_value);
	lsm_mathml_color_attribute_parse (&style_element->math_color, &style->math_color);
	lsm_mathml_color_attribute_parse (&style_element->math_background, &style->math_background);
	lsm_mathml_variant_attribute_parse (&style_element->math_variant, &style->math_variant);

	style->math_size_value = style_element->math_size.value;

	/* mstyle */

	lsm_mathml_length_attribute_parse (&style_element->very_very_thin_math_space,
					&style->very_very_thin_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->very_thin_math_space,
					&style->very_thin_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->thin_math_space,
					&style->thin_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->medium_math_space,
					&style->medium_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->thick_math_space,
					&style->thick_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->very_thick_math_space,
					&style->very_thick_math_space, style->math_size_value);
	lsm_mathml_length_attribute_parse (&style_element->very_very_thick_math_space,
					&style->very_very_thick_math_space, style->math_size_value);

	style->very_very_thin_math_space_value = style_element->very_very_thin_math_space.value;
	style->very_thin_math_space_value = style_element->very_thin_math_space.value;
	style->thin_math_space_value = style_element->thin_math_space.value;
	style->medium_math_space_value = style_element->medium_math_space.value;
	style->thick_math_space_value = style_element->thick_math_space.value;
	style->very_thick_math_space_value = style_element->very_thick_math_space.value;
	style->very_very_thick_math_space_value = style_element->very_very_thick_math_space.value;

	/* mfrac */

	lsm_mathml_length_attribute_parse (&style_element->line_thickness,
					&style->line_thickness,
					style->math_size_value);
}

/* LsmMathmlStyleElement implementation */

LsmDomNode *
lsm_mathml_style_element_new (void)
{
	return g_object_new (LSM_TYPE_MATHML_STYLE_ELEMENT, NULL);
}

static void
lsm_mathml_style_element_init (LsmMathmlStyleElement *self)
{
}

/* LsmMathmlStyleElement class */

static void
lsm_mathml_style_element_class_init (LsmMathmlStyleElementClass *style_class)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_CLASS (style_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (style_class);

	parent_class = g_type_class_peek_parent (style_class);

	node_class->get_node_name = lsm_mathml_style_element_get_node_name;

	m_element_class->update = lsm_mathml_style_element_update;

	m_element_class->attributes = lsm_mathml_attribute_map_duplicate (m_element_class->attributes);

	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "scriptlevel",
					  offsetof (LsmMathmlStyleElement, script_level));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "displaystyle",
					  offsetof (LsmMathmlStyleElement, display_style));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "scriptsizemultiplier",
					  offsetof (LsmMathmlStyleElement, script_size_multiplier));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "scriptminsize",
					  offsetof (LsmMathmlStyleElement, script_min_size));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "background",
					  offsetof (LsmMathmlStyleElement, math_background));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythinmathspace",
					  offsetof (LsmMathmlStyleElement, very_very_thin_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "verythinmathspace",
					  offsetof (LsmMathmlStyleElement, very_thin_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "thinmathspace",
					  offsetof (LsmMathmlStyleElement, thin_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "mediummathspace",
					  offsetof (LsmMathmlStyleElement, medium_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "thickmathspace",
					  offsetof (LsmMathmlStyleElement, thick_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "verythickmathspace",
					  offsetof (LsmMathmlStyleElement, very_thick_math_space));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "veryverythickmathspace",
					  offsetof (LsmMathmlStyleElement, very_very_thick_math_space));

	lsm_mathml_attribute_map_add_string (m_element_class->attributes, "mathfamily",
				       offsetof (LsmMathmlStyleElement, math_family));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "mathvariant",
					  offsetof (LsmMathmlStyleElement, math_variant));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "mathsize",
					  offsetof (LsmMathmlStyleElement, math_size));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "mathcolor",
					  offsetof (LsmMathmlStyleElement, math_color));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "mathbackground",
					  offsetof (LsmMathmlStyleElement, math_background));

	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "linethickness",
					  offsetof (LsmMathmlStyleElement, line_thickness));

	/* Deprecated attributes */

	lsm_mathml_attribute_map_add_string (m_element_class->attributes, "fontfamily",
				       offsetof (LsmMathmlStyleElement, math_family));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "fontsize",
					  offsetof (LsmMathmlStyleElement, math_size));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "color",
					  offsetof (LsmMathmlStyleElement, math_color));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "fontweight",
					  offsetof (LsmMathmlStyleElement, font_weight));
	lsm_mathml_attribute_map_add_attribute (m_element_class->attributes, "fontstyle",
					  offsetof (LsmMathmlStyleElement, font_style));
}

G_DEFINE_TYPE (LsmMathmlStyleElement, lsm_mathml_style_element, LSM_TYPE_MATHML_PRESENTATION_CONTAINER)
