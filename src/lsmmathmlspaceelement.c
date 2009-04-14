/* lsmmathmlspaceelement.c
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

#include <lsmmathmlspaceelement.h>
#include <lsmmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_mathml_space_get_node_name (LsmDomNode *node)
{
	return "mspace";
}

static gboolean
lsm_mathml_space_element_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return FALSE;
}

/* LsmMathmlElement implementation */

static void
lsm_mathml_space_element_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlSpaceElement *space_element = LSM_MATHML_SPACE_ELEMENT (self);
	LsmMathmlSpace space;
	LsmMathmlLength length;


	space.length.unit = LSM_MATHML_UNIT_EM;
	space.length.value = 0;
	space.name = LSM_MATHML_SPACE_NAME_ERROR;

	lsm_mathml_space_attribute_parse (&space_element->width, &space, style);

	length.unit = LSM_MATHML_UNIT_EM;
	length.value = 0.0;

	lsm_mathml_length_attribute_parse (&space_element->height, &length, style->math_size_value);

	length.unit = LSM_MATHML_UNIT_EM;
	length.value = 0.0;

	lsm_mathml_length_attribute_parse (&space_element->depth, &length, style->math_size_value);
}

static const LsmMathmlBbox *
lsm_mathml_space_element_measure (LsmMathmlElement *self, LsmMathmlView *view, const LsmMathmlBbox *bbox)
{
	LsmMathmlSpaceElement *space_element = LSM_MATHML_SPACE_ELEMENT (self);

	self->bbox.width = space_element->width.value;
	self->bbox.height = space_element->height.value;
	self->bbox.depth = space_element->depth.value;
	self->bbox.is_defined = TRUE;

	return &self->bbox;
}

static void
lsm_mathml_space_element_layout (LsmMathmlElement *self, LsmMathmlView *view,
				double x, double y, const LsmMathmlBbox *bbox)
{
	LSM_MATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x, y, bbox);
}

/* LsmMathmlSpaceElement implementation */

LsmDomNode *
lsm_mathml_space_element_new (void)
{
	return g_object_new (LSM_TYPE_MATHML_SPACE_ELEMENT, NULL);
}

static void
lsm_mathml_space_element_init (LsmMathmlSpaceElement *self)
{
}

/* LsmMathmlSpaceElement class */

static void
lsm_mathml_space_element_class_init (LsmMathmlSpaceElementClass *space_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (space_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (space_class);

	parent_class = g_type_class_peek_parent (space_class);

	d_node_class->get_node_name = lsm_mathml_space_get_node_name;
	d_node_class->can_append_child = lsm_mathml_space_element_can_append_child;

	m_element_class->update = lsm_mathml_space_element_update;
	m_element_class->measure = lsm_mathml_space_element_measure;
	m_element_class->layout = lsm_mathml_space_element_layout;
	m_element_class->is_inferred_row = NULL;

	m_element_class->attributes = lsm_dom_attribute_map_duplicate (m_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "width",
					  offsetof (LsmMathmlSpaceElement, width));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "height",
					  offsetof (LsmMathmlSpaceElement, height));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "depth",
					  offsetof (LsmMathmlSpaceElement, depth));
}

G_DEFINE_TYPE (LsmMathmlSpaceElement, lsm_mathml_space_element, LSM_TYPE_MATHML_ELEMENT)
