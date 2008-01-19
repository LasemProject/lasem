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

static void
gmathml_math_element_update (GMathmlElement *self, GMathmlView *view)
{
	gmathml_view_push_style (view, GMATHML_MATH_ELEMENT (self)->default_style);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, view);

	gmathml_view_pop_style (view);
}

static const GMathmlBbox *
gmathml_math_element_measure (GMathmlElement *self, GMathmlView *view)
{
	const GMathmlBbox *bbox;

	gmathml_view_push_style (view, GMATHML_MATH_ELEMENT (self)->default_style);

	bbox = gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view);

	gmathml_view_pop_style (view);

	return bbox;
}

static void
gmathml_math_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
	gmathml_view_push_style (view, GMATHML_MATH_ELEMENT (self)->default_style);

	gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view, x, y, bbox);

	gmathml_view_pop_style (view);
}

static void
gmathml_math_element_render (GMathmlElement *self, GMathmlView *view)
{
	gmathml_view_push_style (view, GMATHML_MATH_ELEMENT (self)->default_style);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);

	gmathml_view_pop_style (view);
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
	self->default_style = GMATHML_STYLE_ELEMENT (gmathml_style_element_new ());
}

/* GMathmlMathElement class */

static void
gmathml_math_element_class_init (GMathmlMathElementClass *math_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (math_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (math_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (math_class);

	parent_class = g_type_class_peek_parent (object_class);

	d_node_class->get_node_name = gmathml_math_element_get_node_name;
	d_node_class->can_append_child = gmathml_math_element_can_append_child;

	m_element_class->update = gmathml_math_element_update;
	m_element_class->measure = gmathml_math_element_measure;
	m_element_class->layout = gmathml_math_element_layout;
	m_element_class->render = gmathml_math_element_render;
}

G_DEFINE_TYPE (GMathmlMathElement, gmathml_math_element, GMATHML_TYPE_ELEMENT)
