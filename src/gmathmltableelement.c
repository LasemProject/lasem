/* gmathmltableelement.c
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

#include <gmathmltableelement.h>
#include <gmathmltablerowelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_table_get_node_name (GDomNode *node)
{
	return "mtable";
}

static gboolean
gmathml_table_element_can_append_child (GDomNode *self, GDomNode *child)
{
	return GMATHML_IS_TABLE_ROW_ELEMENT (child);
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_table_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;
	double height = 0.0;
	double width = 0.0;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
		height += child_bbox->height + child_bbox->depth;
		width = MAX (width, child_bbox->width);
	}

	self->bbox.width = width;
	self->bbox.height = height / 2.0;
	self->bbox.depth = height / 2.0;

	return &self->bbox;
}

static void
gmathml_table_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;
	double height = 0.0;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);

		gmathml_element_layout (GMATHML_ELEMENT (node), view, x,
					y - self->bbox.height + height + child_bbox->height,
					child_bbox);
		height += child_bbox->height + child_bbox->depth;
	}
}

/* GMathmlTableElement implementation */

GDomNode *
gmathml_table_element_new (void)
{
	return g_object_new (GMATHML_TYPE_TABLE_ELEMENT, NULL);
}

static void
gmathml_table_element_init (GMathmlTableElement *self)
{
}

/* GMathmlTableElement class */

static void
gmathml_table_element_class_init (GMathmlTableElementClass *table_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (table_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (table_class);

	parent_class = g_type_class_peek_parent (table_class);

	d_node_class->get_node_name = gmathml_table_get_node_name;
	d_node_class->can_append_child = gmathml_table_element_can_append_child;

	m_element_class->measure = gmathml_table_element_measure;
	m_element_class->layout = gmathml_table_element_layout;
}

G_DEFINE_TYPE (GMathmlTableElement, gmathml_table_element, GMATHML_TYPE_ELEMENT)
