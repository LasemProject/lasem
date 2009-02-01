/* gmathmltablecellelement.c
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

#include <gmathmltablecellelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_table_cell_get_node_name (GDomNode *node)
{
	return "mtd";
}

/* GMathmlElement implementation */

static void
gmathml_table_cell_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlTableCellElement *cell = GMATHML_TABLE_CELL_ELEMENT (self);
	unsigned int span;

	span = 1;
	gdom_attribute_unsigned_parse (&cell->row_span, &span);

	span = 1;
	gdom_attribute_unsigned_parse (&cell->column_span, &span);
}

static const GMathmlBbox *
gmathml_table_cell_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view, bbox);

	return &self->bbox;
}

static void
gmathml_table_cell_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x, y, bbox);
}

/* GMathmlTableCellElement implementation */

void
gmathml_table_cell_element_get_spans (const GMathmlTableCellElement *self,
				      unsigned int *row_span,
				      unsigned int *column_span)
{
	if (row_span != NULL)
		*row_span = 0;
	if (column_span != NULL)
		*column_span = 0;

	g_return_if_fail (GMATHML_IS_TABLE_CELL_ELEMENT (self));

	if (row_span != NULL)
		*row_span = self->row_span.value;
	if (column_span != NULL)
		*column_span = self->column_span.value;
}

GDomNode *
gmathml_table_cell_element_new (void)
{
	return g_object_new (GMATHML_TYPE_TABLE_CELL_ELEMENT, NULL);
}

static void
gmathml_table_cell_element_init (GMathmlTableCellElement *self)
{
}

/* GMathmlTableCellElement class */

static void
gmathml_table_cell_element_class_init (GMathmlTableCellElementClass *table_cell_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (table_cell_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (table_cell_class);

	parent_class = g_type_class_peek_parent (table_cell_class);

	d_node_class->get_node_name = gmathml_table_cell_get_node_name;

	m_element_class->update = gmathml_table_cell_element_update;
	m_element_class->measure = gmathml_table_cell_element_measure;
	m_element_class->layout = gmathml_table_cell_element_layout;

	m_element_class->attributes = gdom_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gdom_attribute_map_add_attribute (m_element_class->attributes, "rowspan",
					  offsetof (GMathmlTableCellElement, row_span));
	gdom_attribute_map_add_attribute (m_element_class->attributes, "columnspan",
					  offsetof (GMathmlTableCellElement, column_span));
}

G_DEFINE_TYPE (GMathmlTableCellElement, gmathml_table_cell_element, GMATHML_TYPE_ELEMENT)
