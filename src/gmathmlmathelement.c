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
#include <gmathmlstyleelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_math_element_get_node_name (GDomNode *node)
{
	return "math";
}

/* GMathmlElement implementation */

static void
gmathml_math_element_update (GMathmlElement *self, GMathmlView *view, GMathmlStyle *style)
{
	GMathmlMathElement *math_element = GMATHML_MATH_ELEMENT (self);
	GMathmlMode default_mode;

	gmathml_attribute_mode_parse (&math_element->mode, &default_mode);

	style->display_style = (default_mode == GMATHML_MODE_DISPLAY);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, view, style);
}
/* GMathmlMathElement implementation */

const GMathmlStyle *
gmathml_math_element_get_default_style (GMathmlMathElement *math_element)
{
	g_return_val_if_fail (GMATHML_IS_MATH_ELEMENT (math_element), NULL);

	return math_element->default_style;
}

GDomNode *
gmathml_math_element_new (void)
{
	return g_object_new (GMATHML_TYPE_MATH_ELEMENT, NULL);
}

static void
gmathml_math_element_init (GMathmlMathElement *self)
{
	GMathmlStyle *style;

	style = gmathml_style_new ();
	self->default_style = style;
	g_return_if_fail (style != NULL);

	style->font_size = 12.0;

	style->script_level = 0;
	style->script_size_multiplier = 0.71;
	style->script_min_size.value = 8.0;
	style->script_min_size.unit = GMATHML_UNIT_PT;

	style->very_very_thin_math_space.value = 0.0555556;
	style->very_very_thin_math_space.unit = GMATHML_UNIT_EM;
	style->very_thin_math_space.value = 0.111111;
	style->very_thin_math_space.unit = GMATHML_UNIT_EM;
	style->thin_math_space.value = 0.166667;
	style->thin_math_space.unit = GMATHML_UNIT_EM;
	style->medium_math_space.value = 0.222222;
	style->medium_math_space.unit = GMATHML_UNIT_EM;
	style->thick_math_space.value = 0.277778;
	style->thick_math_space.unit = GMATHML_UNIT_EM;
	style->very_thick_math_space.value = 0.333333;
	style->very_thick_math_space.unit = GMATHML_UNIT_EM;
	style->very_very_thick_math_space.value = 0.388889;
	style->very_very_thick_math_space.unit = GMATHML_UNIT_EM;

	style->very_very_thin_math_space_value = 0.0555556 * style->font_size;
	style->very_thin_math_space_value = 0.111111 * style->font_size;
	style->thin_math_space_value = 0.166667 * style->font_size;
	style->medium_math_space_value = 0.222222 * style->font_size;
	style->thick_math_space_value = 0.277778 * style->font_size;
	style->very_thick_math_space_value = 0.333333 * style->font_size;
	style->very_very_thick_math_space_value = 0.388889 * style->font_size;

	style->math_size.value = 12.0;
	style->math_size.unit = GMATHML_UNIT_PT;
	style->math_color.red = 0;
	style->math_color.green = 0;
	style->math_color.blue = 0;
	style->math_color.alpha = 1;
	style->math_background.red = 0;
	style->math_background.green = 0;
	style->math_background.blue = 0;
	style->math_background.alpha = 1;
	style->math_variant = GMATHML_VARIANT_NORMAL;

	style->subscript_shift.value = 0;
	style->subscript_shift.unit = GMATHML_UNIT_PT;
	style->superscript_shift.value = 0;
	style->superscript_shift.unit = GMATHML_UNIT_PT;

	style->line_thickness.value = 0.1;
	style->line_thickness.unit = GMATHML_UNIT_EM;
}

static void
gmathml_math_element_finalize (GObject *object)
{
	GMathmlMathElement *math_element = GMATHML_MATH_ELEMENT (object);

	gmathml_style_free (math_element->default_style);
}

/* GMathmlMathElement class */

static void
gmathml_math_element_class_init (GMathmlMathElementClass *math_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (math_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (math_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (math_class);

	parent_class = g_type_class_peek_parent (math_class);

	object_class->finalize = gmathml_math_element_finalize;

	d_node_class->get_node_name = gmathml_math_element_get_node_name;

	m_element_class->update = gmathml_math_element_update;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mode",
					     offsetof (GMathmlMathElement, mode));
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
