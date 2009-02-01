/*
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

#include <gmathmlaligngroupelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_align_group_get_node_name (GDomNode *node)
{
	return "maligngroup";
}

static gboolean
gmathml_align_group_element_can_append_child (GDomNode *self, GDomNode *child)
{
	return FALSE;
}

/* GMathmlElement implementation */

static void
gmathml_align_group_element_update (GMathmlElement *self, GMathmlStyle *style)
{
}

static const GMathmlBbox *
gmathml_align_group_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	self->bbox = gmathml_bbox_null;

	return &self->bbox;
}

static void
gmathml_align_group_element_layout (GMathmlElement *self, GMathmlView *view,
				   double x, double y, const GMathmlBbox *bbox)
{
}

static void
gmathml_align_group_element_render (GMathmlElement *element, GMathmlView *view)
{
}

/* GMathmlAlignGroupElement implementation */

GDomNode *
gmathml_align_group_element_new (void)
{
	return g_object_new (GMATHML_TYPE_ALIGN_GROUP_ELEMENT, NULL);
}

static void
gmathml_align_group_element_init (GMathmlAlignGroupElement *self)
{
}

/* GMathmlAlignGroupElement class */

static void
gmathml_align_group_element_class_init (GMathmlAlignGroupElementClass *align_group_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (align_group_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (align_group_class);

	parent_class = g_type_class_peek_parent (align_group_class);

	d_node_class->get_node_name = gmathml_align_group_get_node_name;
	d_node_class->can_append_child = gmathml_align_group_element_can_append_child;

	m_element_class->update = gmathml_align_group_element_update;
	m_element_class->measure = gmathml_align_group_element_measure;
	m_element_class->layout = gmathml_align_group_element_layout;
	m_element_class->render = gmathml_align_group_element_render;
	m_element_class->is_inferred_row = NULL;

	m_element_class->attributes = gdom_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
}

G_DEFINE_TYPE (GMathmlAlignGroupElement, gmathml_align_group_element, GMATHML_TYPE_ELEMENT)
