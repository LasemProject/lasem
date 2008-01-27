/* gmathmlradicalelement.c
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

#include <gmathmlradicalelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_radical_get_node_name (GDomNode *self)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT)
		return "msqrt";

	return "mroot";
}

static gboolean
gmathml_radical_element_can_append_child (GDomNode *self, GDomNode *child)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT)
		return GMATHML_IS_ELEMENT (child);

	return GMATHML_IS_ELEMENT (child) && (self->first_child == NULL ||
					      self->first_child->next_sibling == NULL);
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_radical_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT) {
		GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view);

		self->bbox.width += 10;

		return &self->bbox;
	}

	self->bbox.width = 0.0;
	self->bbox.height = 0.0;
	self->bbox.depth = 0.0;

	node = GDOM_NODE (self)->first_child;

	if (node == NULL)
		return &self->bbox;

	self->bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view);

	self->bbox.width += 10;

	return &self->bbox;
}

static void
gmathml_radical_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT) {
		GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x + 10.0, y, bbox);
		return;
	}

	node = GDOM_NODE (self)->first_child;

	if (node != NULL)
		gmathml_element_layout (GMATHML_ELEMENT (node), view, x + 10.0, y, bbox);
}

static void
gmathml_radical_element_render (GMathmlElement *self, GMathmlView *view)
{
	gmathml_view_draw_line (view,
				self->x, self->y - 5,
				self->x +2, self->y - 5, 1.0);
	gmathml_view_draw_line (view,
				self->x + 2, self->y - 5,
				self->x + 5, self->y + self->bbox.depth, 1.0);
	gmathml_view_draw_line (view,
				self->x + 5, self->y + self->bbox.depth,
				self->x + 10, self->y - self->bbox.height, 1.0);
	gmathml_view_draw_line (view,
				self->x + 10, self->y - self->bbox.height,
			        self->x + self->bbox.width, self->y - self->bbox.height, 1.0);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);
}

/* GMathmlRadicalElement implementation */

GDomNode *
gmathml_sqrt_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_RADICAL_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_RADICAL_ELEMENT (node)->type = GMATHML_RADICAL_ELEMENT_TYPE_SQRT;

	return node;
}

GDomNode *
gmathml_root_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_RADICAL_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_RADICAL_ELEMENT (node)->type = GMATHML_RADICAL_ELEMENT_TYPE_ROOT;

	return node;
}

static void
gmathml_radical_element_init (GMathmlRadicalElement *self)
{
}

/* GMathmlRadicalElement class */

static void
gmathml_radical_element_class_init (GMathmlRadicalElementClass *radical_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (radical_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (radical_class);

	parent_class = g_type_class_peek_parent (radical_class);

	d_node_class->get_node_name = gmathml_radical_get_node_name;
	d_node_class->can_append_child = gmathml_radical_element_can_append_child;

	m_element_class->measure = gmathml_radical_element_measure;
	m_element_class->layout = gmathml_radical_element_layout;
	m_element_class->render = gmathml_radical_element_render;
}

G_DEFINE_TYPE (GMathmlRadicalElement, gmathml_radical_element, GMATHML_TYPE_ELEMENT)
