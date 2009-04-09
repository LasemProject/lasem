/* lsmmathmltablerowelement.c
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

#include <lsmmathmltablerowelement.h>
#include <lsmmathmltablecellelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_mathml_table_row_get_node_name (LsmDomNode *node)
{
	LsmMathmlTableRowElement *row = LSM_MATHML_TABLE_ROW_ELEMENT (node);

	switch (row->type) {
		case LSM_MATHML_TABLE_ROW_ELEMENT_TYPE_LABELED_ROW:
			return "mtr";
			break;
		default:
			return "mlabeledtr";
			break;
	}
}

static gboolean
lsm_mathml_table_row_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return LSM_MATHML_IS_ELEMENT (child);
}

/* LsmMathmlElement implementation */

static const LsmMathmlBbox *
lsm_mathml_table_row_element_measure (LsmMathmlElement *self, LsmMathmlView *view, const LsmMathmlBbox *bbox)
{
	LSM_MATHML_ELEMENT_CLASS (parent_class)->measure (self, view, bbox);

	return &self->bbox;
}

static void
lsm_mathml_table_row_element_layout (LsmMathmlElement *self, LsmMathmlView *view,
				double x, double y, const LsmMathmlBbox *bbox)
{
	LSM_MATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x, y, bbox);
}

/* LsmMathmlTableRowElement implementation */

LsmDomNode *
lsm_mathml_table_row_element_new (void)
{
	LsmDomNode *node;

	node = g_object_new (LSM_MATHML_TYPE_TABLE_ROW_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	LSM_MATHML_TABLE_ROW_ELEMENT (node)->type = LSM_MATHML_TABLE_ROW_ELEMENT_TYPE_ROW;

	return node;
}

LsmDomNode *
lsm_mathml_labeled_table_row_element_new (void)
{
	LsmDomNode *node;

	node = g_object_new (LSM_MATHML_TYPE_TABLE_ROW_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	LSM_MATHML_TABLE_ROW_ELEMENT (node)->type = LSM_MATHML_TABLE_ROW_ELEMENT_TYPE_LABELED_ROW;

	return node;
}

static void
lsm_mathml_table_row_element_init (LsmMathmlTableRowElement *self)
{
}

/* LsmMathmlTableRowElement class */

static void
lsm_mathml_table_row_element_class_init (LsmMathmlTableRowElementClass *table_row_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (table_row_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (table_row_class);

	parent_class = g_type_class_peek_parent (table_row_class);

	d_node_class->get_node_name = lsm_mathml_table_row_get_node_name;
	d_node_class->can_append_child = lsm_mathml_table_row_can_append_child;

	m_element_class->measure = lsm_mathml_table_row_element_measure;
	m_element_class->layout = lsm_mathml_table_row_element_layout;
	m_element_class->is_inferred_row = NULL;
}

G_DEFINE_TYPE (LsmMathmlTableRowElement, lsm_mathml_table_row_element, LSM_MATHML_TYPE_ELEMENT)
