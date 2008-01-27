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
	GDomNode *node;

	gmathml_attribute_length_parse (&fraction->line_thickness, &style->line_thickness, style->font_size);
	gmathml_attribute_boolean_parse (&fraction->bevelled, &style->bevelled);

	fraction->offset = style->font_size * 0.45;
	fraction->padding = style->font_size * 0.2 + style->line_thickness.value * 0.5;

	if (!style->display_style)
		gmathml_style_change_script_level (style, +1);

	node = GDOM_NODE (self)->first_child;
	if (node == NULL)
		return;

	gmathml_element_update (GMATHML_ELEMENT (node), view, style);

	node = node->next_sibling;
	if (node == NULL)
		return;

	gmathml_element_update (GMATHML_ELEMENT (node), view, style);
}

static const GMathmlBbox *
gmathml_fraction_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	self->bbox.width = 0.0;
	/* FIXME attribute setting */
	self->bbox.height = 1.0;
	self->bbox.depth = 1.0;

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

	self->bbox.height += fraction->padding + fraction->offset;
	self->bbox.depth += fraction->padding - fraction->offset;

	return &self->bbox;
}

static void
gmathml_fraction_element_layout (GMathmlElement *self, GMathmlView *view,
				 double x, double y, const GMathmlBbox *bbox)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	node = GDOM_NODE (self)->first_child;

	if (node != NULL) {
		child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
		gmathml_element_layout (GMATHML_ELEMENT (node), view,
					x + (bbox->width - child_bbox->width) / 2.0,
					y - child_bbox->depth -
					fraction->padding -
					fraction->offset, child_bbox);

		node = node->next_sibling;
		if (node != NULL) {
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);

			g_message ("[GMathmlFractionElement::layout] numerator height  %g",
				   child_bbox->height);

			gmathml_element_layout (GMATHML_ELEMENT (node), view,
						x + (bbox->width - child_bbox->width) / 2.0,
						y + child_bbox->height +
						fraction->padding -
						fraction->offset, child_bbox);
		}
	}
}

static void
gmathml_fraction_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlFractionElement *fraction = GMATHML_FRACTION_ELEMENT (self);
	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);

	gmathml_view_draw_line (view, self->x, self->y - fraction->offset,
				self->x + self->bbox.width, self->y - fraction->offset,
				fraction->line_thickness.value);
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
	self->offset = 0.0;
	self->padding = 0.0;
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
