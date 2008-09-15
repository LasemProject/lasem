/* gmathmlradicalelement.c
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

#include <gdomdebug.h>
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

static void
gmathml_radical_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT) {
		GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);
		return;
	}

	node = GDOM_NODE (self)->first_child;
	if (node != NULL) {
		gmathml_element_update (GMATHML_ELEMENT (node), style);

		node = node->next_sibling;
		if (node != NULL) {
			gmathml_style_change_script_level (style, +1);

			gmathml_element_update (GMATHML_ELEMENT (node), style);
		}
	}
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_radical_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GMathmlBbox stretch_bbox;
	GDomNode *node;
	double x_offset = 0.0;
	double y_offset = 0.0;

	node = GDOM_NODE (self)->first_child;
	if (node == NULL) {
		stretch_bbox.width = 0;
		stretch_bbox.height = self->math_size;
		stretch_bbox.depth = 0;
		stretch_bbox.is_defined = TRUE;

		gmathml_view_measure_radical (view, &stretch_bbox, &radical->bbox, NULL, NULL);
		self->bbox = radical->bbox;
		return &self->bbox;
	}

	node = GDOM_NODE (self)->first_child;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT)
		GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view, &gmathml_bbox_null);
	else
		self->bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);

	stretch_bbox = self->bbox;

	radical->width = self->bbox.width;
	radical->radical_x_offset = 0.0;
	radical->order_y_offset = 0.0;

	gmathml_view_measure_radical (view, &stretch_bbox, &radical->bbox,
				      &x_offset, &y_offset);

	gmathml_bbox_add_horizontally (&self->bbox, &radical->bbox);

	gdom_debug ("[GMathmlRadicalElement::measure] Radical bbox w = %g, h = %g, d = %g",
		    radical->bbox.width, radical->bbox.height, radical->bbox.depth);

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_ROOT) {

		node = node->next_sibling;

		if (node != NULL) {
			GMathmlBbox child_bbox;
			double height;

			child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);

			radical->radical_x_offset = child_bbox.width - x_offset;
			self->bbox.width += radical->radical_x_offset;

			gdom_debug ("[GMathmlRadicalElement::measure] y_offset = %g", y_offset);

			height = self->bbox.height - y_offset + child_bbox.height + child_bbox.depth;
			if (height > self->bbox.height)
				self->bbox.height = height;

			radical->order_y_offset = - self->bbox.height + child_bbox.height;

			gdom_debug ("[GMathmlRadicalElement::measure] order_y_offset = %g",
				    radical->order_y_offset);
		}
	}

	return &self->bbox;
}

static void
gmathml_radical_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);
	GDomNode *node;

	if (radical->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT)
		GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x + radical->bbox.width, y, bbox);
	else {
		node = GDOM_NODE (self)->first_child;

		if (node != NULL) {
			GMathmlBbox child_bbox;

			child_bbox = *gmathml_element_get_bbox (GMATHML_ELEMENT (node));

			gmathml_element_layout (GMATHML_ELEMENT (node), view,
						x + radical->bbox.width + radical->radical_x_offset,
						y, &child_bbox);

			node = node->next_sibling;

			if (node != NULL) {
				child_bbox = *gmathml_element_get_bbox (GMATHML_ELEMENT (node));

				gmathml_element_layout (GMATHML_ELEMENT (node), view, x,
							y + radical->order_y_offset,
							&child_bbox);
			}
		}
	}
}

static void
gmathml_radical_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (self);

	gmathml_view_show_radical (view, self->x + radical->radical_x_offset, self->y,
				   radical->width, &radical->bbox);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);
}

static gboolean
gmathml_radical_element_is_inferred_row (const GMathmlElement *self)
{
	return GMATHML_RADICAL_ELEMENT (self)->type == GMATHML_RADICAL_ELEMENT_TYPE_SQRT;
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

	m_element_class->is_inferred_row = gmathml_radical_element_is_inferred_row;
}

G_DEFINE_TYPE (GMathmlRadicalElement, gmathml_radical_element, GMATHML_TYPE_ELEMENT)
