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

static void
gmathml_table_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlTableElement *table = GMATHML_TABLE_ELEMENT (self);
	GMathmlSpaceList *space_list;
	GMathmlNamedList named_list;
	unsigned int named;

	named_list.n_values = 1;
	named_list.values = &named;

	named = GMATHML_ROW_ALIGN_BASELINE;
	gmathml_attribute_row_align_parse (&table->row_align, &named_list);

	named = GMATHML_COLUMN_ALIGN_CENTER;
	gmathml_attribute_column_align_parse (&table->column_align, &named_list);

	space_list = gmathml_space_list_new (1);

	space_list->spaces[0].length.value = 1.0;
	space_list->spaces[0].length.unit = GMATHML_UNIT_EX;
	space_list->spaces[0].name = GMATHML_SPACE_NAME_ERROR;

	gmathml_attribute_space_list_parse (&table->row_spacing, space_list, style);

	space_list->spaces[0].length.value = 0.8;
	space_list->spaces[0].length.unit = GMATHML_UNIT_EM;
	space_list->spaces[0].name = GMATHML_SPACE_NAME_ERROR;

	gmathml_attribute_space_list_parse (&table->column_spacing, space_list, style);

	gmathml_space_list_free (space_list);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);
}

static const GMathmlBbox *
gmathml_table_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMathmlTableElement *table = GMATHML_TABLE_ELEMENT (self);
	GDomNode *row_node;
	GDomNode *cell_node;
	const GMathmlBbox *bbox;
	unsigned int row, column;
	unsigned int max_index;
	double max_height = 0.0;
	double max_width = 0.0;
	double max_depth = 0.0;

	g_free (table->widths);
	g_free (table->heights);
	g_free (table->depths);

	table->n_columns = 0;
	table->n_rows = 0;

	self->bbox.width = 0;
	self->bbox.height = 0;
	self->bbox.depth = 0;

	for (row_node = GDOM_NODE (self)->first_child;
	     row_node != NULL;
	     row_node = row_node->next_sibling) {
		unsigned int counter = 0;

		for (cell_node = row_node->first_child;
		     cell_node != NULL;
		     cell_node = cell_node->next_sibling)
			counter++;
		table->n_rows++;
		if (table->n_columns < counter)
			table->n_columns = counter;
	}

	if (table->n_rows < 1 || table->n_columns < 1) {
		table->n_columns = 0;
		table->n_rows = 0;
		return &self->bbox;
	}

	table->widths = g_new (double, table->n_columns);
	table->heights = g_new (double, table->n_rows);
	table->depths = g_new (double, table->n_rows);

	row = 0;
	for (row_node = GDOM_NODE (self)->first_child;
	     row_node != NULL;
	     row_node = row_node->next_sibling) {
		column = 0;
		table->heights[row] = 0.0;
		table->depths[row] = 0.0;
		for (cell_node = row_node->first_child;
		     cell_node != NULL;
		     cell_node = cell_node->next_sibling) {
			bbox = gmathml_element_measure (GMATHML_ELEMENT (cell_node), view);
			max_width = MAX (max_width, bbox->width);
			max_height = MAX (max_height, bbox->height);
			max_depth = MAX (max_depth, bbox->depth);
			if (row == 0)
				table->widths[column] = bbox->width;
			else
				table->widths[column] = MAX (table->widths[column], bbox->width);
			if (column == 0) {
				table->heights[row] = bbox->height;
				table->depths[row] = bbox->depth;
			} else {
				table->heights[row] = MAX (table->heights[row], bbox->height);
				table->depths[row]  = MAX (table->depths[row],  bbox->depth);
			}
			column++;
		}
		row++;
	}

	max_index = table->column_spacing.space_list->n_spaces -  1;
	for (column = 0; column < table->n_columns; column++) {
		self->bbox.width += table->widths[column];
		if (column < table->n_columns - 1)
			self->bbox.width += table->column_spacing.values[MIN (column, max_index)];
	}

	max_index = table->row_spacing.space_list->n_spaces -  1;
	for (row = 0; row < table->n_rows; row++) {
		self->bbox.height += table->heights[row] + table->depths[row];
		if (row < table->n_rows - 1)
			self->bbox.height += table->row_spacing.values[MIN (row, max_index)];
	}

	self->bbox.height *= 0.5;
	self->bbox.depth = self->bbox.height;

	return &self->bbox;
}

static void
gmathml_table_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlTableElement *table = GMATHML_TABLE_ELEMENT (self);
	GDomNode *row_node, *cell_node;
	double y_offset, x_offset;
	unsigned int max_column;
	unsigned int max_row;
	unsigned int row, column;
	double x_cell, y_cell;

	if (table->n_rows < 1 || table->n_columns < 1)
		return;

	max_column = table->column_spacing.space_list->n_spaces -  1;
	max_row = table->row_spacing.space_list->n_spaces -  1;
	y_offset = -self->bbox.height;

	row = 0;
	for (row_node = GDOM_NODE (self)->first_child;
	     row_node != NULL;
	     row_node = row_node->next_sibling) {
		column = 0;
		x_offset = 0.0;
		for (cell_node = row_node->first_child;
		     cell_node != NULL;
		     cell_node = cell_node->next_sibling) {
			bbox = gmathml_element_measure (GMATHML_ELEMENT (cell_node), view);

			switch (table->row_align.values[MIN (row, table->row_align.n_values - 1)]) {
				case GMATHML_ROW_ALIGN_TOP:
					y_cell = y + y_offset + bbox->height;
					break;
				case GMATHML_ROW_ALIGN_BOTTOM:
					y_cell = y + y_offset + table->heights[row] + table->depths[row] -
						bbox->depth;
					break;
				case GMATHML_ROW_ALIGN_CENTER:
					y_cell = y + y_offset + (table->heights[row] + table->depths[row] +
								 bbox->height + bbox->depth) * 0.5 +
						bbox->height;
				default:
					y_cell = y + y_offset + table->heights[row];
			}

			switch (table->column_align.values[MIN (column, table->column_align.n_values - 1)]) {
				case GMATHML_COLUMN_ALIGN_LEFT:
					x_cell = x + x_offset;
					break;
				case GMATHML_COLUMN_ALIGN_RIGHT:
					x_cell = x + x_offset + table->widths[column] - bbox->width;
					break;
				default:
					x_cell = x + x_offset + (table->widths[column] - bbox->width) * 0.5;
			}

			gmathml_element_layout (GMATHML_ELEMENT (cell_node), view,
						x_cell, y_cell, bbox);

			if (column < table->n_columns - 1) {
				x_offset += table->widths[column];
				x_offset += table->column_spacing.values[MIN (column, max_column)];
				column++;
			}
		}

		if (row < table->n_rows - 1) {
			y_offset += table->heights[row] + table->depths[row];
			y_offset += table->row_spacing.values[MIN (row, max_row)];
			row++;
		}
	}
}

/* GMathmlTableElement implementation */

GDomNode *
gmathml_table_element_new (void)
{
	return g_object_new (GMATHML_TYPE_TABLE_ELEMENT, NULL);
}

static void
gmathml_table_element_init (GMathmlTableElement *table)
{
	table->widths = NULL;
	table->heights = NULL;
	table->depths = NULL;
	table->n_columns = 0;
	table->n_rows = 0;
}

static void
gmathml_table_element_finalize (GObject *object)
{
	GMathmlTableElement *table = GMATHML_TABLE_ELEMENT (object);

	g_free (table->widths);
	g_free (table->heights);
	g_free (table->depths);
	table->widths = NULL;
	table->heights = NULL;
	table->depths = NULL;
}

/* GMathmlTableElement class */

static void
gmathml_table_element_class_init (GMathmlTableElementClass *table_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (table_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (table_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (table_class);

	parent_class = g_type_class_peek_parent (table_class);

	object_class->finalize = gmathml_table_element_finalize;

	d_node_class->get_node_name = gmathml_table_get_node_name;
	d_node_class->can_append_child = gmathml_table_element_can_append_child;

	m_element_class->update = gmathml_table_element_update;
	m_element_class->measure = gmathml_table_element_measure;
	m_element_class->layout = gmathml_table_element_layout;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "rowalign",
						  offsetof (GMathmlTableElement, row_align),
						  gmathml_attribute_named_list_finalize);
	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "columnalign",
						  offsetof (GMathmlTableElement, column_align),
						  gmathml_attribute_named_list_finalize);
	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "rowspacing",
						  offsetof (GMathmlTableElement, row_spacing),
						  gmathml_attribute_space_list_finalize);
	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "columnspacing",
						  offsetof (GMathmlTableElement, column_spacing),
						  gmathml_attribute_space_list_finalize);
}

G_DEFINE_TYPE (GMathmlTableElement, gmathml_table_element, GMATHML_TYPE_ELEMENT)
