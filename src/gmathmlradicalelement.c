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

static void
gmathml_radical_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);

	radical->top_padding = style->medium_math_space_value;

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);
}

static const GMathmlBbox *
gmathml_radical_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT) {
		GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view);

		self->bbox.width += gmathml_view_measure_length (view, self->math_size);
		self->bbox.height += gmathml_view_measure_length (view, radical->top_padding);

		radical->height = self->bbox.height + self->bbox.depth;
		radical->offset = 0.0;
	} else {
		GMathmlBbox child_bbox;

		node = GDOM_NODE (self)->first_child;

		self->bbox.width = 0.0;
		self->bbox.height = 0.0;
		self->bbox.depth = 0.0;
		radical->offset = 0.0;
		radical->height = 0.0;

		if (node != NULL) {
			child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view);

			self->bbox.width = child_bbox.width + gmathml_view_measure_length (view,
											  self->math_size);
			self->bbox.height = child_bbox.height + gmathml_view_measure_length (view, radical->top_padding);
			self->bbox.depth = child_bbox.depth;

			radical->height = self->bbox.height + self->bbox.depth;

			node = node->next_sibling;

			if (node != NULL) {
				double height;

				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view);

				radical->offset = child_bbox.width -
					gmathml_view_measure_length (view, self->math_size * 0.1);

				self->bbox.width += radical->offset;

				height = radical->height * 0.6 -
					self->bbox.depth + child_bbox.height + child_bbox.depth;
				if (height > self->bbox.height)
					self->bbox.height = height;
			}
		}
	}

	g_message ("top_padding = %g", radical->top_padding);

	return &self->bbox;
}

static void
gmathml_radical_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;
	double offset;

	offset = gmathml_view_measure_length (view, self->math_size);

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT)
		GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x + offset, y, bbox);
	else {
		node = GDOM_NODE (self)->first_child;

		if (node != NULL) {
			GMathmlBbox child_bbox;

			child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view);

			gmathml_element_layout (GMATHML_ELEMENT (node), view,
						x + offset + radical->offset, y, &child_bbox);

			node = node->next_sibling;

			if (node != NULL) {
				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view);

				gmathml_element_layout (GMATHML_ELEMENT (node), view, x,
							y + self->bbox.depth - radical->height * 0.6 -
							child_bbox.depth,
							&child_bbox);
			}
		}
	}
}

static void
gmathml_radical_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);

	gmathml_view_draw_root (view,
				self->x + radical->offset,
				self->y + self->bbox.depth,
				gmathml_view_measure_length (view, self->math_size),
				radical->height,
				self->bbox.width - gmathml_view_measure_length (view, self->math_size) -
				radical->offset,
				self->math_size / 20.0,
				&self->math_color);

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

	m_element_class->update = gmathml_radical_element_update;
	m_element_class->measure = gmathml_radical_element_measure;
	m_element_class->layout = gmathml_radical_element_layout;
	m_element_class->render = gmathml_radical_element_render;
}

G_DEFINE_TYPE (GMathmlRadicalElement, gmathml_radical_element, GMATHML_TYPE_ELEMENT)
