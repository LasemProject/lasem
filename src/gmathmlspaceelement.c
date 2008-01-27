/* gmathmlspaceelement.c
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

#include <gmathmlspaceelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_space_get_node_name (GDomNode *node)
{
	return "mspace";
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_space_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view);

	return &self->bbox;
}

static void
gmathml_space_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x, y, bbox);
}

/* GMathmlSpaceElement implementation */

GDomNode *
gmathml_space_element_new (void)
{
	return g_object_new (GMATHML_TYPE_SPACE_ELEMENT, NULL);
}

static void
gmathml_space_element_init (GMathmlSpaceElement *self)
{
}

/* GMathmlSpaceElement class */

static void
gmathml_space_element_class_init (GMathmlSpaceElementClass *space_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (space_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (space_class);

	parent_class = g_type_class_peek_parent (space_class);

	d_node_class->get_node_name = gmathml_space_get_node_name;

	m_element_class->measure = gmathml_space_element_measure;
	m_element_class->layout = gmathml_space_element_layout;
}

G_DEFINE_TYPE (GMathmlSpaceElement, gmathml_space_element, GMATHML_TYPE_ELEMENT)
