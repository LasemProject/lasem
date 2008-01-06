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

/* GdomNode implementation */

static const char *
gmathml_math_element_get_node_name (GDomNode *node)
{
	return "math";
}

static gboolean
gmathml_math_element_can_append_child (GDomNode *self, GDomNode *child)
{
	/* Math element has only one element child */

	return (GMATHML_IS_ELEMENT (child) &&
		self->first_child == NULL);
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_math_element_measure (GMathmlElement *element, GMathmlView *view)
{
	return gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE(element)->first_child), view);
}

static void
gmathml_math_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
	gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view, x, y, bbox);
}

/* GMathmlMathElement implementation */

GDomNode *
gmathml_math_element_new (void)
{
	return g_object_new (GMATHML_TYPE_MATH_ELEMENT, NULL);
}

static void
gmathml_math_element_init (GMathmlMathElement *self)
{
	GMathmlElement *m_element = GMATHML_ELEMENT (self);

	g_message ("Init math element");

	m_element->style_attrs.script_level.value = 0;
	m_element->style_attrs.display_style.value = FALSE;
	m_element->style_attrs.script_size_multiplier.value = 0.71;
	m_element->style_attrs.script_min_size.value = 1;
	m_element->style_attrs.script_min_size.unit = GMATHML_UNIT_PX;
	/* FIXME background */
	m_element->style_attrs.very_very_thin_math_space.value = 0.05555556;
	m_element->style_attrs.very_very_thin_math_space.unit = GMATHML_UNIT_EM;
}

/* GMathmlMathElement class */

static void
gmathml_math_element_class_init (GMathmlMathElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (klass);

	d_node_class->get_node_name = gmathml_math_element_get_node_name;
	d_node_class->can_append_child = gmathml_math_element_can_append_child;

	m_element_class->measure = gmathml_math_element_measure;
	m_element_class->layout = gmathml_math_element_layout;
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
