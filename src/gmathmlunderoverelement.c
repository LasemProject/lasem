/* gmathmlunderoverelement.c
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

#include <gmathmlunderoverelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_under_over_get_node_name (GDomNode *node)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (node);

	switch (under_over->type) {
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
			return "munder";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
			return "mover";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER:
		default:
			return "munderover";
	}
}

static gboolean
gmathml_under_over_element_can_append_child (GDomNode *self, GDomNode *child)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	if (!GMATHML_ELEMENT (child))
		return FALSE;

	if (self->first_child == NULL ||
	    self->first_child->next_sibling == NULL)
		return TRUE;

	if (under_over->type != GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER)
		return FALSE;

	return (self->first_child->next_sibling->next_sibling == NULL);
}

static void
gmathml_under_over_element_update_child_pointers (GMathmlUnderOverElement *self)
{
	GDomNode *node;

	node = GDOM_NODE (self)->first_child;
	if (node == NULL) {
		self->base = NULL;
		self->underscript = NULL;
		self->overscript = NULL;
		return;
	} else
		self->base = GMATHML_ELEMENT (node);

	node = node->next_sibling;
	if (node != NULL)
		switch (self->type) {
			case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
				self->underscript = NULL;
				self->overscript = GMATHML_ELEMENT (node);
				break;
			case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
				self->underscript = GMATHML_ELEMENT (node);
				self->overscript = NULL;
				break;
			default:
				self->underscript = GMATHML_ELEMENT (node);
				node = node->next_sibling;
				if (node != NULL)
					self->overscript = GMATHML_ELEMENT (node);
		}
	else {
		self->underscript = NULL;
		self->overscript = NULL;
	}
}

static void
gmathml_under_over_element_post_new_child (GDomNode *self, GDomNode *child)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	gmathml_under_over_element_update_child_pointers (under_over);
}

/* GMathmlElement implementation */

static void
gmathml_under_over_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	gboolean accent;
	gboolean accent_under;

	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	accent = FALSE;
	gmathml_attribute_boolean_parse (&under_over->accent, &accent);
	accent_under = FALSE;
	gmathml_attribute_boolean_parse (&under_over->accent_under, &accent_under);

	under_over->under_space = accent_under ? style->very_thin_math_space_value : style->very_thick_math_space_value;
	under_over->over_space  = accent       ? style->very_thin_math_space_value : style->very_thick_math_space_value;

	g_message ("space under = %g, over = %g", under_over->under_space, under_over->over_space);

	if (under_over->base != NULL)
		gmathml_element_update (GMATHML_ELEMENT (under_over->base), style);

	style->display_style = FALSE;

	if (!accent_under)
		gmathml_style_change_script_level (style, +1);

	if (under_over->underscript != NULL)
		gmathml_element_update (GMATHML_ELEMENT (under_over->underscript), style);

	if (!accent_under)
		gmathml_style_change_script_level (style, -1);
	if (!accent)
		gmathml_style_change_script_level (style, +1);

	if (under_over->overscript != NULL)
		gmathml_element_update (GMATHML_ELEMENT (under_over->overscript), style);
}

static const GMathmlBbox *
gmathml_under_over_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);
	GDomNode *node;
	GMathmlBbox const *base_bbox = NULL;
	GMathmlBbox const *underscript_bbox = NULL;
	GMathmlBbox const *overscript_bbox = NULL;
	GMathmlBbox child_bbox;

	self->bbox.width = 0.0;
	self->bbox.height = 0.0;
	self->bbox.depth = 0.0;

	node = GDOM_NODE (self)->first_child;

	if (node != NULL) {
		base_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);
		gmathml_bbox_add_to_right (&self->bbox, base_bbox);

		node = node->next_sibling;

		if (node != NULL) {
			GMathmlBbox const *node_bbox;

			node_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);

			switch (under_over->type) {
				case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
					underscript_bbox = node_bbox;
					break;
				case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
					overscript_bbox = node_bbox;
					break;
				case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER:
					underscript_bbox = node_bbox;

					node = node->next_sibling;

					if (node != NULL)
						overscript_bbox = gmathml_element_measure (GMATHML_ELEMENT (node),
											   view, NULL);
			}
		}
	}

	if (overscript_bbox) {
		child_bbox = *overscript_bbox;
		child_bbox.depth += gmathml_view_measure_length (view, under_over->over_space);
		gmathml_bbox_add_over (&self->bbox, &child_bbox);
	}
	if (underscript_bbox) {
		child_bbox = *underscript_bbox;
		child_bbox.height += gmathml_view_measure_length (view, under_over->under_space);
		gmathml_bbox_add_under (&self->bbox, &child_bbox);
	}

	return &self->bbox;
}

static void
gmathml_under_over_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);
	const GMathmlBbox *child_bbox;

	if (under_over->base == NULL)
		return;

	child_bbox = gmathml_element_measure (under_over->base, view, NULL);
	gmathml_element_layout (under_over->base, view, x + (bbox->width - child_bbox->width) * 0.5, y,
				child_bbox);

	if (under_over->underscript) {
		child_bbox = gmathml_element_measure (under_over->underscript, view, NULL);
		gmathml_element_layout (under_over->underscript, view,
					x + (bbox->width - child_bbox->width) * 0.5,
					y + self->bbox.depth - child_bbox->depth,
					child_bbox);
	}
	if (under_over->overscript) {
		child_bbox = gmathml_element_measure (under_over->overscript, view, NULL);
		gmathml_element_layout (under_over->overscript, view,
					x + (bbox->width - child_bbox->width) * 0.5,
					y - self->bbox.height + child_bbox->height,
					child_bbox);
	}
}

static const GMathmlOperatorElement *
gmathml_under_over_element_get_embellished_core (const GMathmlElement *self)
{
	if (GDOM_NODE (self)->first_child != NULL)
		return gmathml_element_get_embellished_core (GMATHML_ELEMENT (GDOM_NODE (self)->first_child));

	return NULL;
}

/* GMathmlUnderOverElement implementation */

GDomNode *
gmathml_under_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER;

	return node;
}

GDomNode *
gmathml_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER;

	return node;
}

GDomNode *
gmathml_under_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER;

	return node;
}

static void
gmathml_under_over_element_init (GMathmlUnderOverElement *self)
{
}

/* GMathmlUnderOverElement class */

static void
gmathml_under_over_element_class_init (GMathmlUnderOverElementClass *under_over_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (under_over_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (under_over_class);

	parent_class = g_type_class_peek_parent (under_over_class);

	d_node_class->get_node_name = gmathml_under_over_get_node_name;
	d_node_class->can_append_child = gmathml_under_over_element_can_append_child;
	d_node_class->post_new_child = gmathml_under_over_element_post_new_child;

	m_element_class->update = gmathml_under_over_element_update;
	m_element_class->measure = gmathml_under_over_element_measure;
	m_element_class->layout = gmathml_under_over_element_layout;

	m_element_class->get_embellished_core = gmathml_under_over_element_get_embellished_core;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "accent",
					     offsetof (GMathmlUnderOverElement, accent));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "accentunder",
					     offsetof (GMathmlUnderOverElement, accent_under));
}

G_DEFINE_TYPE (GMathmlUnderOverElement, gmathml_under_over_element, GMATHML_TYPE_ELEMENT)
