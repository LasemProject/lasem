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

#include <gdomdebug.h>
#include <gmathmlelement.h>
#include <gmathmlspaceelement.h>
#include <gmathmloperatorelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

static const GMathmlBbox null_bbox = {0.0,0.0,0.0, FALSE};

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
	GDomNode *iter;

	if (GMATHML_IS_ELEMENT (child)) {
		GMATHML_ELEMENT (child)->need_update = TRUE;
		GMATHML_ELEMENT (child)->need_measure = TRUE;
	}
	for (iter = parent; iter != NULL && GMATHML_IS_ELEMENT (iter); iter = iter->parent_node) {
		if (GMATHML_ELEMENT (iter)->need_children_update)
			break;
		GMATHML_ELEMENT (iter)->need_children_update = TRUE;
		GMATHML_ELEMENT (iter)->need_measure = TRUE;
	}
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

static gboolean
_update_children (GMathmlElement *self, GMathmlStyle *style)
{
	GDomNode *node;
	gboolean need_measure = FALSE;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node))
			if (gmathml_element_update (GMATHML_ELEMENT (node), style))
				need_measure = TRUE;

	return need_measure;
}

gboolean
gmathml_element_update (GMathmlElement *self, const GMathmlStyle *parent_style)
{
	GMathmlElementClass *element_class;
	GMathmlStyle *style;
	gboolean need_measure;

	g_return_val_if_fail (GMATHML_IS_ELEMENT (self), FALSE);
	g_return_val_if_fail (parent_style != NULL, FALSE);

	if (!self->need_update && !self->need_children_update) {
		gdom_debug ("[Element::update] %s already up to date",
			    gdom_node_get_node_name (GDOM_NODE (self)));
		return FALSE;
	}

	gdom_debug ("[Element::update] update %s", gdom_node_get_node_name (GDOM_NODE (self)));

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	style = gmathml_style_duplicate (parent_style);

	if (element_class->update != NULL)
		element_class->update (self, style);

	g_free (self->style.math_family);
	self->style.math_family = g_strdup (style->math_family);
	self->style.math_variant = style->math_variant;
	self->style.math_size = style->math_size_value;
	self->style.math_color = style->math_color;
	self->style.math_background = style->math_background;

	if (self->need_update) {
		GDomNode *node;

		for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
			if (GMATHML_IS_ELEMENT (node))
				GMATHML_ELEMENT (node)->need_update = TRUE;
	}

	if (element_class->update_children != NULL)
		need_measure = element_class->update_children (self, style);
	else
		need_measure = FALSE;

	gmathml_style_free (style);

	self->need_measure = need_measure || self->need_update;

	self->need_update = FALSE;
	self->need_children_update = FALSE;

	return self->need_measure;
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
	unsigned int n_elements = 0;

	self->bbox = gmathml_bbox_null;

	stretch_bbox = *bbox;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node)) {
			operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
			if (operator != NULL && operator->stretchy.value) {
				stretchy_found = TRUE;
				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (operator), view,
								       &gmathml_bbox_null);
				gmathml_bbox_stretch_vertically (&stretch_bbox, &child_bbox);
			} else {
				all_stretchy = FALSE;
				child_bbox = *gmathml_element_measure (GMATHML_ELEMENT (node), view,
								       &gmathml_bbox_null);
				if (operator != NULL) {
					child_bbox.width +=
						gmathml_view_measure_length (view, operator->left_space.value) +
						gmathml_view_measure_length (view, operator->right_space.value);
				}
				gmathml_bbox_add_horizontally (&self->bbox, &child_bbox);
				gmathml_bbox_stretch_vertically (&stretch_bbox, &child_bbox);
			}
			n_elements++;
		}
	}

	if (n_elements > 0)
		self->bbox.width += (n_elements - 1) * self->style.math_size * GMATHML_SPACE_EM_THIN;

	if (stretchy_found) {
		gdom_debug ("[Element::_measure] Stretchy found (width = %g, height = %g, depth = %g)",
			    stretch_bbox.width, stretch_bbox.height, stretch_bbox.depth);

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

	if (stretch_bbox == NULL)
		stretch_bbox = &gmathml_bbox_null;

	if (element->need_measure || stretch_bbox->is_defined) {
		if (element_class->measure) {
			element->bbox = *(element_class->measure (element, view, stretch_bbox));

			gdom_debug ("[Element::measure] Bbox (%s) %g, %g, %g",
				    gdom_node_get_node_name (GDOM_NODE (element)),
				    element->bbox.width, element->bbox.height, element->bbox.depth);
		} else {

			gdom_debug ("[Element::measure] method not defined");
			element->bbox.width = 0.0;
			element->bbox.height = 0.0;
			element->bbox.depth = 0.0;
		}

		element->need_measure = FALSE;
		element->need_layout = TRUE;
	} else
		gdom_debug ("[Element::measure] %s already up to date",
			    gdom_node_get_node_name (GDOM_NODE (element)));

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
			x += child_bbox.width + self->style.math_size * GMATHML_SPACE_EM_THIN;
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

	gdom_debug ("[Element::layout] assigned bbox for %s = %g, %g, %g at %g, %g",
		    gdom_node_get_node_name (GDOM_NODE (self)), bbox->width, bbox->height, bbox->depth, x , y);

	self->x = x;
	self->y = y;

	if (element_class->layout)
		element_class->layout (self, view, x, y, bbox);

	self->need_layout = FALSE;
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
	GMathmlElementClass *element_class;

	g_return_if_fail (GMATHML_IS_ELEMENT (element));
	element_class = GMATHML_ELEMENT_GET_CLASS (element);
	g_return_if_fail (element_class != NULL);

	if (element_class->render) {
		gmathml_view_show_bbox (view, element->x, element->y, &element->bbox);
		gmathml_view_show_background (view, &element->style,
					      element->x, element->y,
					      &element->bbox);
		element_class->render (element, view);
	}
}

/* Inferred mrow implementation */

static GMathmlOperatorElement *
_get_embellished_core (const GMathmlElement *self)
{
	GDomNode *node;
	GMathmlOperatorElement *core = NULL;

	g_assert (GMATHML_IS_ELEMENT (self));

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node)) {
			if (!GMATHML_IS_SPACE_ELEMENT (node)) {
				if (core != NULL)
					return NULL;
				core = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
			}
		} else
			if (core != NULL)
				return NULL;
	}

	return core;
}

GMathmlOperatorElement *
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
	GMathmlElement *m_element = GMATHML_ELEMENT (object);

	gmathml_attribute_map_free_attributes (m_element_class->attributes, object);

	g_free (m_element->style.math_family);

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

	m_element_class->update = NULL;
	m_element_class->update_children = _update_children;
	m_element_class->measure = _measure;
	m_element_class->layout = _layout;
	m_element_class->render = _render;
	m_element_class->get_embellished_core = _get_embellished_core;
	m_element_class->is_inferred_row = _is_inferred_row;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
}

G_DEFINE_ABSTRACT_TYPE (GMathmlElement, gmathml_element, GDOM_TYPE_ELEMENT)
