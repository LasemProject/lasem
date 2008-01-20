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

static gboolean
gmathml_math_element_can_append_child (GDomNode *self, GDomNode *child)
{
	/* Math element has only one element child */

	return (GMATHML_IS_ELEMENT (child) &&
		self->first_child == NULL);
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_math_element_measure (GMathmlElement *self, GMathmlView *view)
{
	const GMathmlBbox *bbox;

	bbox = gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view);

	return bbox;
}

static void
gmathml_math_element_layout (GMathmlElement *self, GMathmlView *view, double x, double y, const GMathmlBbox *bbox)
{
	gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view, x, y, bbox);
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

	style->script_size_multiplier = 0.71;
	style->script_min_size = 8.0;

	style->math_size = 12.0;
	style->math_color.red = 0;
	style->math_color.green = 0;
	style->math_color.blue = 0;
	style->math_color.alpha = 1;
	style->math_background.red = 0;
	style->math_background.green = 0;
	style->math_background.blue = 0;
	style->math_background.alpha = 1;
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

	parent_class = g_type_class_peek_parent (object_class);

	object_class->finalize = gmathml_math_element_finalize;

	d_node_class->get_node_name = gmathml_math_element_get_node_name;
	d_node_class->can_append_child = gmathml_math_element_can_append_child;

	m_element_class->measure = gmathml_math_element_measure;
	m_element_class->layout = gmathml_math_element_layout;
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
