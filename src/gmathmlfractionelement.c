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

static const GMathmlBbox *
gmathml_fraction_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	self->bbox.width = 0.0;
	/* FIXME attribute setting */
	self->bbox.ascent = 1.0;
	self->bbox.descent = 1.0;

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
gmathml_fraction_element_render (GMathmlElement *self, GMathmlView *view)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node)) {
			gmathml_element_render (GMATHML_ELEMENT (node), view);
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
			gmathml_view_rel_move_to (view, child_bbox->width, 0);
		}
}

GDomNode *
gmathml_fraction_element_new (void)
{
	return g_object_new (GMATHML_TYPE_FRACTION_ELEMENT, NULL);
}

static char *
gmathml_fraction_element_get_node_name (GDomNode *node)
{
	return "mfrac";
}

static void
gmathml_fraction_element_init (GMathmlFractionElement *element)
{
}

static gboolean
gmathml_fraction_element_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GMATHML_IS_ELEMENT (child) &&
		(self->first_child == NULL ||
		 self->first_child->next_sibling == NULL));
}

static void
gmathml_fraction_element_class_init (GMathmlFractionElementClass *fraction_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (fraction_class);
	GMathmlElementClass *element_class = GMATHML_ELEMENT_CLASS (fraction_class);

	d_node_class->get_node_name = gmathml_fraction_element_get_node_name;
	d_node_class->can_append_child = gmathml_fraction_element_can_append_child;

	element_class->measure = gmathml_fraction_element_measure;
	element_class->render = gmathml_fraction_element_render;
}

G_DEFINE_TYPE (GMathmlFractionElement, gmathml_fraction_element, GMATHML_TYPE_ELEMENT)
