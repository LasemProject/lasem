/* gmathmlfractionelement.c
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
gmathml_fraction_element_update (GMathmlElement *self, GMathmlView *view, GMathmlStyle *style)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);

	gmathml_attribute_length_parse (&fraction->line_thickness, &style->line_thickness, style->font_size);
	gmathml_attribute_boolean_parse (&fraction->bevelled, &style->bevelled);

	fraction->font_size = style->font_size;
	fraction->space = style->medium_math_space_value;
	fraction->color = style->math_color;

	g_message ("space = %g", fraction->space);

	if (!style->display_style)
		gmathml_style_change_script_level (style, +1);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, view, style);
}

static const GMathmlBbox *
gmathml_fraction_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	self->bbox.depth = - gmathml_view_measure_fraction_offset (view, fraction->font_size) +
		gmathml_view_measure_space (view, fraction->space) +
		gmathml_view_measure_space (view, fraction->line_thickness.value / 2.0);

	self->bbox.width = 0.0;
	self->bbox.height = - self->bbox.depth + 2 * gmathml_view_measure_space (view, fraction->space) +
		gmathml_view_measure_space (view, fraction->line_thickness.value);

	node = GDOM_NODE (self)->first_child;

	if (node == NULL)
		return &self->bbox;

	child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);

	gmathml_bbox_add_over (&self->bbox, child_bbox);

	node = node->next_sibling;

	if (node == NULL)
		return &self->bbox;

	child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);

	gmathml_bbox_add_under (&self->bbox, child_bbox);

	return &self->bbox;
}

static void
gmathml_fraction_element_layout (GMathmlElement *self, GMathmlView *view,
				 double x, double y, const GMathmlBbox *bbox)
{
/*        GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);*/
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	node = GDOM_NODE (self)->first_child;

	if (node != NULL) {
		child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
		gmathml_element_layout (GMATHML_ELEMENT (node), view,
					x + (bbox->width - child_bbox->width) / 2.0,
					y - self->bbox.height + child_bbox->height,
					child_bbox);

		node = node->next_sibling;
		if (node != NULL) {
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);

			g_message ("[GMathmlFractionElement::layout] numerator height  %g",
				   child_bbox->height);

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

	gmathml_view_draw_fraction_line (view, self->x,
					 self->y - gmathml_view_measure_fraction_offset (view, fraction->font_size),
					 self->bbox.width,
					 fraction->line_thickness.value,
					 &fraction->color);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);
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
	self->font_size = 0.0;
	self->space = 0.0;
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

	element_class->measure = gmathml_fraction_element_measure;
	element_class->layout = gmathml_fraction_element_layout;
	element_class->render = gmathml_fraction_element_render;

	element_class->update = gmathml_fraction_element_update;

	element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (element_class);
	gmathml_element_class_add_fraction_attributes (element_class);
}

G_DEFINE_TYPE (GMathmlFractionElement, gmathml_fraction_element, GMATHML_TYPE_ELEMENT)
