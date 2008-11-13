/* gmathmlfractionelement.c
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

#include <gmathmlfractionelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GDomNode implementation */

static const char *
gmathml_fraction_element_get_node_name (GDomNode *node)
{
	return "mfrac";
}

static gboolean
gmathml_fraction_element_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GMATHML_IS_ELEMENT (child) &&
		(self->first_child == NULL ||
		 self->first_child->next_sibling == NULL));
}

/* GMathmlElement implementation */

static void
gmathml_fraction_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);

	gmathml_attribute_length_parse (&fraction->line_thickness, &style->line_thickness, style->math_size_value);
	gmathml_attribute_boolean_parse (&fraction->bevelled, &style->bevelled);

	fraction->display = style->display;
}

static gboolean
gmathml_fraction_element_update_children (GMathmlElement *self, GMathmlStyle *style)
{
	if (style->display == GMATHML_DISPLAY_INLINE)
		gmathml_style_change_script_level (style, +1);
	else
		style->display = GMATHML_DISPLAY_INLINE;

	return GMATHML_ELEMENT_CLASS (parent_class)->update_children (self, style);
}

static const GMathmlBbox *
gmathml_fraction_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	GDomNode *node;
	const GMathmlBbox *child_bbox;
	double h_space;
	double v_space;
	double length, dividend_offset, divisor_offset;

	fraction->axis_offset = gmathml_view_measure_axis_offset (view, self->style.math_size);

	if (fraction->display == GMATHML_DISPLAY_INLINE)
		v_space = self->style.math_size * GMATHML_SPACE_EM_VERY_THIN;
	else
		v_space = self->style.math_size * GMATHML_SPACE_EM_MEDIUM;
	h_space = self->style.math_size * GMATHML_SPACE_EM_VERY_THIN;

	self->bbox.is_defined = TRUE;
	self->bbox.depth = - gmathml_view_measure_length (view,
							  fraction->axis_offset -
							  v_space - 0.5 * fraction->line_thickness.value);

	self->bbox.width = 0.0;
	self->bbox.height = gmathml_view_measure_length (view, fraction->axis_offset +
							 v_space + 0.5 * fraction->line_thickness.value);

	node = GDOM_NODE (self)->first_child;

	if (node == NULL)
		return &self->bbox;

	child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);
	gmathml_view_get_font_metrics (view, &GMATHML_ELEMENT (node)->style, NULL, &length);
	if (fraction->display == GMATHML_DISPLAY_INLINE)
		length /= 2.0;
	if (child_bbox->depth < length)
		dividend_offset = length - child_bbox->depth;
	else
		dividend_offset = 0.0;

	gmathml_bbox_add_over (&self->bbox, child_bbox);

	node = node->next_sibling;

	if (node == NULL)
		return &self->bbox;

	child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);
	gmathml_view_get_font_metrics (view, &GMATHML_ELEMENT (node)->style, &length, NULL);
	if (child_bbox->height < length)
		divisor_offset = length - child_bbox->height;
	else
		divisor_offset = 0.0;

	gmathml_bbox_add_under (&self->bbox, child_bbox);

	self->bbox.height += dividend_offset;
	self->bbox.depth += divisor_offset;
	self->bbox.width += 2.0 * h_space;

	return &self->bbox;
}

static void
gmathml_fraction_element_layout (GMathmlElement *self, GMathmlView *view,
				 double x, double y, const GMathmlBbox *bbox)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	node = GDOM_NODE (self)->first_child;

	if (node != NULL) {
		child_bbox = gmathml_element_get_bbox (GMATHML_ELEMENT (node));
		gmathml_element_layout (GMATHML_ELEMENT (node), view,
					x + (bbox->width - child_bbox->width) / 2.0,
					y - self->bbox.height + child_bbox->height,
					child_bbox);

		node = node->next_sibling;
		if (node != NULL) {
			child_bbox = gmathml_element_get_bbox (GMATHML_ELEMENT (node));
			gmathml_element_layout (GMATHML_ELEMENT (node), view,
						x + (bbox->width - child_bbox->width) / 2.0,
						y + self->bbox.depth - child_bbox->depth,
						child_bbox);
		}
	}
}

static void
gmathml_fraction_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	double h_space;

	h_space = self->style.math_size * GMATHML_SPACE_EM_VERY_THIN;

	gmathml_view_show_fraction_line (view, &self->style,
					 self->x + h_space,
					 self->y - gmathml_view_measure_length (view, fraction->axis_offset),
					 self->bbox.width - 2.0 * h_space,
					 fraction->line_thickness.value);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);
}

static const GMathmlOperatorElement *
gmathml_fraction_element_get_embellished_core (const GMathmlElement *self)
{
	if (GDOM_NODE (self)->first_child != NULL)
		return gmathml_element_get_embellished_core (GMATHML_ELEMENT (GDOM_NODE (self)->first_child));

	return NULL;
}

/* GMathmlFraction implementation */

GDomNode *
gmathml_fraction_element_new (void)
{
	return g_object_new (GMATHML_TYPE_FRACTION_ELEMENT, NULL);
}

static void
gmathml_fraction_element_init (GMathmlFractionElement *self)
{
	self->axis_offset = 0.0;
}

/* GMathmlFractionElement class */

void
gmathml_element_class_add_fraction_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "linethickness",
					     offsetof (GMathmlFractionElement, line_thickness));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "bevelled",
					     offsetof (GMathmlFractionElement, bevelled));
}

static void
gmathml_fraction_element_class_init (GMathmlFractionElementClass *fraction_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (fraction_class);
	GMathmlElementClass *element_class = GMATHML_ELEMENT_CLASS (fraction_class);

	parent_class = g_type_class_peek_parent (fraction_class);

	d_node_class->get_node_name = gmathml_fraction_element_get_node_name;
	d_node_class->can_append_child = gmathml_fraction_element_can_append_child;

	element_class->update = gmathml_fraction_element_update;
	element_class->update_children = gmathml_fraction_element_update_children;
	element_class->measure = gmathml_fraction_element_measure;
	element_class->layout = gmathml_fraction_element_layout;
	element_class->render = gmathml_fraction_element_render;
	element_class->get_embellished_core = gmathml_fraction_element_get_embellished_core;
	element_class->is_inferred_row = NULL;

	element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (element_class);
	gmathml_element_class_add_fraction_attributes (element_class);
}

G_DEFINE_TYPE (GMathmlFractionElement, gmathml_fraction_element, GMATHML_TYPE_ELEMENT)
