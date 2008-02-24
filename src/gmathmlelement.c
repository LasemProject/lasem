/* gmathmlelement.c
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

#include <gmathmlelement.h>
#include <gmathmlspaceelement.h>
#include <gmathmloperatorelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

static const GMathmlBbox null_bbox = {0.0,0.0,0.0};

/* GDomNode implementation */

static gboolean
gmathml_element_can_append_child (GDomNode *self, GDomNode *child)
{
	/* Math element has only one element child */

	return (GMATHML_IS_ELEMENT (child));
}

static void
gmathml_element_post_new_child (GDomNode *parent, GDomNode *child)
{
}

static void
gmathml_element_pre_remove_child (GDomNode *parent, GDomNode *child)
{
}

/* GDomElement implementation */

static void
gmathml_element_set_attribute (GDomElement *self, const char* name, const char *value)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS(self);

	gmathml_attribute_map_set_attribute (m_element_class->attributes, self,
					     name, value);
}

const char *
gmathml_element_get_attribute (GDomElement *self, const char *name)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS(self);

	return gmathml_attribute_map_get_attribute (m_element_class->attributes, self, name);
}

/* GMathmlElement implementation */

static void
_update (GMathmlElement *self, GMathmlStyle *style)
{
	GDomNode *node;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node))
			gmathml_element_update (GMATHML_ELEMENT (node), style);

}

void
gmathml_element_update (GMathmlElement *self, const GMathmlStyle *parent_style)
{
	GMathmlElementClass *element_class;
	GMathmlStyle *style;

	g_return_if_fail (GMATHML_IS_ELEMENT (self));
	g_return_if_fail (parent_style != NULL);

	g_message ("[Element::update] %s", gdom_node_get_node_name (GDOM_NODE (self)));

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	style = gmathml_style_duplicate (parent_style);

#if 0
	gmathml_style_dump (style);
#endif

	if (element_class->update)
		element_class->update (self, style);

	self->math_color = style->math_color;
	self->math_background = style->math_background;
	self->math_size = style->math_size_value;
	self->math_variant = style->math_variant;

	g_message ("[Element::update] Math size = %g", self->math_size);

	gmathml_style_free (style);
}

/* Inferred mrow implementation */

static const GMathmlBbox *
_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	const GMathmlOperatorElement *operator;
	GDomNode *node;
	GMathmlBbox child_bbox;
	GMathmlBbox stretch_bbox;
	gboolean stretchy_found = FALSE;
	gboolean all_stretchy = TRUE;

	self->bbox = gmathml_bbox_null;

	if (bbox != NULL)
		stretch_bbox = *bbox;
	else
		stretch_bbox = gmathml_bbox_null;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node)) {
			operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
			if (operator != NULL && operator->stretchy.value) {
				stretchy_found = TRUE;
				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (operator), view, NULL);
				gmathml_bbox_stretch_vertically (&stretch_bbox, &child_bbox);
			} else {
				all_stretchy = FALSE;
				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view, NULL);
				if (operator != NULL) {
					child_bbox.width +=
						gmathml_view_measure_length (view, operator->left_space.value) +
						gmathml_view_measure_length (view, operator->right_space.value);
				}
				gmathml_bbox_add_horizontally (&self->bbox, &child_bbox);
			}
		}
	}

	if (stretchy_found) {
		if (!all_stretchy) {
			stretch_bbox.height = self->bbox.height;
			stretch_bbox.depth = self->bbox.depth;
			stretch_bbox.width = 0.0;
			gmathml_bbox_stretch_vertically (&stretch_bbox, bbox);
		}

		g_message ("[Element::_measure] Stretchy found (h = %g, d = %g)",
			   stretch_bbox.height, stretch_bbox.depth);

		for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
			if (GMATHML_IS_ELEMENT (node)) {
				operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
				if (operator != NULL && operator->stretchy.value) {
					child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view,
									       &stretch_bbox);
					child_bbox.width +=
						gmathml_view_measure_length (view, operator->left_space.value) +
						gmathml_view_measure_length (view, operator->right_space.value);
					gmathml_bbox_add_horizontally (&self->bbox, &child_bbox);
				}
			}
		}
	}

	return &self->bbox;
}

const GMathmlBbox *
gmathml_element_measure (GMathmlElement *element, GMathmlView *view, const GMathmlBbox *stretch_bbox)
{
	GMathmlElementClass *element_class;

	g_return_val_if_fail (GMATHML_IS_ELEMENT (element), &null_bbox);

	element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_val_if_fail (element_class != NULL, &null_bbox);

	if (!element->measure_done || stretch_bbox != NULL) {
		if (element_class->measure) {
			gmathml_view_push_element (view, element);
			element->bbox = *(element_class->measure (element, view, stretch_bbox));
			gmathml_view_pop_element (view);

			g_message ("[Element::measure] Bbox (%s) %g, %g, %g",
				   gdom_node_get_node_name (GDOM_NODE (element)),
				   element->bbox.width, element->bbox.height, element->bbox.depth);
		} else {

			g_message ("[Element::measure] method not defined");
			element->bbox.width = 0.0;
			element->bbox.height = 0.0;
			element->bbox.depth = 0.0;
		}

		element->measure_done = TRUE;
	}

	return &element->bbox;
}

/* Inferred mrow implementation */

static void
_layout (GMathmlElement *self, GMathmlView *view,
	 double x, double y, const GMathmlBbox *bbox)
{
	const GMathmlOperatorElement *operator;
	GDomNode *node;
	GMathmlBbox child_bbox;
	double offset;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node)) {
			child_bbox = *gmathml_element_get_bbox (GMATHML_ELEMENT (node));
			operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
			if (operator != NULL)
				offset = gmathml_view_measure_length (view, operator->left_space.value);
			else
				offset = 0.0;
			gmathml_element_layout (GMATHML_ELEMENT (node), view, x + offset, y, &child_bbox);
			if (operator != NULL)
				child_bbox.width += offset +
					gmathml_view_measure_length (view, operator->right_space.value);
			x += child_bbox.width;
		}
}

void
gmathml_element_layout (GMathmlElement *self, GMathmlView *view,
			double x, double y, const GMathmlBbox *bbox)
{
	GMathmlElementClass *element_class;

	g_return_if_fail (GMATHML_IS_ELEMENT (self));

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	g_return_if_fail (element_class != NULL);

	g_message ("[Element::layout] assigned bbox for %s = %g, %g, %g",
		   gdom_node_get_node_name (GDOM_NODE (self)), bbox->width, bbox->height, bbox->depth);

	self->x = x;
	self->y = y;

	if (element_class->layout) {
		gmathml_view_push_element (view, self);
		element_class->layout (self, view, x, y, bbox);
		gmathml_view_pop_element (view);
	}
}

/* Inferred mrow implementation */

static void
_render (GMathmlElement *element, GMathmlView *view)
{
	GDomNode *node;

	for (node = GDOM_NODE (element)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node))
		    gmathml_element_render (GMATHML_ELEMENT (node), view);
}

void
gmathml_element_render (GMathmlElement *element, GMathmlView *view)
{
	GMathmlElementClass *element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_if_fail (element_class != NULL);

	if (element_class->render) {
		gmathml_view_push_element (view, element);
		gmathml_view_show_background (view, element->x, element->y, &element->bbox);
		element_class->render (element, view);
		gmathml_view_pop_element (view);
	}
}

/* Inferred mrow implementation */

static const GMathmlOperatorElement *
_get_embellished_core (const GMathmlElement *self)
{
	GDomNode *node;
	const GMathmlOperatorElement *core = NULL;
	const GMathmlOperatorElement *operator;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node))
			operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
		else
			operator = NULL;

		if (!GMATHML_IS_SPACE_ELEMENT (node)) {
			if (operator == NULL || operator != core)
				return NULL;
			core = operator;
		}
	}

	return core;
}

const GMathmlOperatorElement *
gmathml_element_get_embellished_core (const GMathmlElement *self)
{
	GMathmlElementClass *element_class;

	g_return_val_if_fail (GMATHML_IS_ELEMENT (self), NULL);

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	if (element_class->get_embellished_core != NULL)
		return element_class->get_embellished_core (self);

	return NULL;
}

const GMathmlBbox *
gmathml_element_get_bbox (const GMathmlElement *self)
{
	g_return_val_if_fail (GMATHML_IS_ELEMENT (self), &gmathml_bbox_null);

	return &self->bbox;
}

static gboolean
_is_inferred_row (const GMathmlElement *self)
{
	return TRUE;
}

gboolean
gmathml_element_is_inferred_row (const GMathmlElement *self)
{
	GMathmlElementClass *element_class;

	g_return_val_if_fail (GMATHML_IS_ELEMENT (self), FALSE);

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	if (element_class->is_inferred_row != NULL)
		return element_class->is_inferred_row (self);

	return FALSE;
}

static void
gmathml_element_init (GMathmlElement *element)
{
}

static void
gmathml_element_finalize (GObject *object)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS (object);

	gmathml_attribute_map_free_attributes (m_element_class->attributes, object);

	parent_class->finalize (object);
}

/* GMathmlElement class */

void
gmathml_element_class_add_element_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "class",
					     offsetof (GMathmlElement, class_name));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "id",
					     offsetof (GMathmlElement, id));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "href",
					     offsetof (GMathmlElement, href));
}

static void
gmathml_element_class_init (GMathmlElementClass *m_element_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (m_element_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_element_class);
	GDomElementClass *d_element_class = GDOM_ELEMENT_CLASS (m_element_class);

	parent_class = g_type_class_peek_parent (m_element_class);

	object_class->finalize = gmathml_element_finalize;

	d_node_class->can_append_child = gmathml_element_can_append_child;
	d_node_class->post_new_child = gmathml_element_post_new_child;
	d_node_class->pre_remove_child = gmathml_element_pre_remove_child;

	d_element_class->get_attribute = gmathml_element_get_attribute;
	d_element_class->set_attribute = gmathml_element_set_attribute;

	m_element_class->update = _update;
	m_element_class->measure = _measure;
	m_element_class->layout = _layout;
	m_element_class->render = _render;
	m_element_class->get_embellished_core = _get_embellished_core;
	m_element_class->is_inferred_row = _is_inferred_row;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
}

G_DEFINE_ABSTRACT_TYPE (GMathmlElement, gmathml_element, GDOM_TYPE_ELEMENT)
