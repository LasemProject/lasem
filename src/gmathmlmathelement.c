/* gmathmlmathelement.c
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
_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlMathElement *math_element = GMATHML_MATH_ELEMENT (self);
	GMathmlMode default_mode = (style->display == GMATHML_DISPLAY_INLINE) ?
		GMATHML_MODE_INLINE : GMATHML_MODE_DISPLAY;

	gmathml_attribute_mode_parse (&math_element->mode, &default_mode);

	style->display = (default_mode == GMATHML_MODE_INLINE) ?
		GMATHML_DISPLAY_INLINE : GMATHML_DISPLAY_BLOCK;

	gmathml_attribute_display_parse (&math_element->display, &style->display);
}

/* GMathmlMathElement implementation */

GMathmlStyle *
gmathml_math_element_get_default_style (GMathmlMathElement *math_element)
{
	g_return_val_if_fail (GMATHML_IS_MATH_ELEMENT (math_element), NULL);

	return math_element->default_style;
}

void
gmathml_math_element_set_default_style (GMathmlMathElement *math_element, GMathmlStyle *style)
{
	g_return_if_fail (GMATHML_IS_MATH_ELEMENT (math_element));
	g_return_if_fail (style != NULL);

	if (style == math_element->default_style) {
		gdom_node_changed (GDOM_NODE (math_element));
		return;
	}

	gmathml_style_free (math_element->default_style);
	math_element->default_style = style;

	gdom_node_changed (GDOM_NODE (math_element));
}

void
gmathml_math_element_update (GMathmlMathElement *math_element)
{
	gmathml_element_update (GMATHML_ELEMENT (math_element),
				gmathml_math_element_get_default_style (math_element));
}

const GMathmlBbox *
gmathml_math_element_measure (GMathmlMathElement *math_element,
			      GMathmlView *view)
{
	return gmathml_element_measure (GMATHML_ELEMENT (math_element), view, NULL);
}

void
gmathml_math_element_layout (GMathmlMathElement *math_element,
			     GMathmlView *view,
			     const GMathmlBbox *bbox)
{
	gmathml_element_layout (GMATHML_ELEMENT (math_element), view, 0, 0, bbox);
}

void
gmathml_math_element_render (GMathmlMathElement *math_element, GMathmlView *view)
{
	gmathml_element_render (GMATHML_ELEMENT (math_element), view);
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

	style->math_size_value = 12.0;

	style->display = GMATHML_DISPLAY_INLINE;

	style->script_level = 0;
	style->script_size_multiplier = 0.71;
	style->script_min_size.value = 8.0;
	style->script_min_size.unit = GMATHML_UNIT_PT;

	style->very_very_thin_math_space.value = 	GMATHML_SPACE_EM_VERY_VERY_THIN;
	style->very_very_thin_math_space.unit = 	GMATHML_UNIT_EM;
	style->very_thin_math_space.value = 		GMATHML_SPACE_EM_VERY_THIN;
	style->very_thin_math_space.unit = 		GMATHML_UNIT_EM;
	style->thin_math_space.value = 			GMATHML_SPACE_EM_THIN;
	style->thin_math_space.unit = 			GMATHML_UNIT_EM;
	style->medium_math_space.value = 		GMATHML_SPACE_EM_MEDIUM;
	style->medium_math_space.unit = 		GMATHML_UNIT_EM;
	style->thick_math_space.value = 		GMATHML_SPACE_EM_THICK;
	style->thick_math_space.unit = 			GMATHML_UNIT_EM;
	style->very_thick_math_space.value = 		GMATHML_SPACE_EM_VERY_THICK;
	style->very_thick_math_space.unit = 		GMATHML_UNIT_EM;
	style->very_very_thick_math_space.value = 	GMATHML_SPACE_EM_VERY_VERY_THICK;
	style->very_very_thick_math_space.unit = 	GMATHML_UNIT_EM;

	style->very_very_thin_math_space_value = 	GMATHML_SPACE_EM_VERY_VERY_THIN * style->math_size_value;
	style->very_thin_math_space_value = 		GMATHML_SPACE_EM_VERY_THIN * style->math_size_value;
	style->thin_math_space_value = 			GMATHML_SPACE_EM_THIN * style->math_size_value;
	style->medium_math_space_value = 		GMATHML_SPACE_EM_MEDIUM * style->math_size_value;
	style->thick_math_space_value = 		GMATHML_SPACE_EM_THICK * style->math_size_value;
	style->very_thick_math_space_value = 		GMATHML_SPACE_EM_VERY_THICK * style->math_size_value;
	style->very_very_thick_math_space_value = 	GMATHML_SPACE_EM_VERY_VERY_THICK * style->math_size_value;

	style->math_family = g_strdup ("Serif");
	style->math_variant = GMATHML_VARIANT_NORMAL;
	style->math_size.value = 12.0;
	style->math_size.unit = GMATHML_UNIT_PT;
	style->math_color.red = 0;
	style->math_color.green = 0;
	style->math_color.blue = 0;
	style->math_color.alpha = 1;
	style->math_background.red = 0;
	style->math_background.green = 0;
	style->math_background.blue = 0;
	style->math_background.alpha = 0;

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

	m_element_class->update = _update;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mode",
					     offsetof (GMathmlMathElement, mode));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "display",
					     offsetof (GMathmlMathElement, display));
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
