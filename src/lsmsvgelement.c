/*
 * Copyright © 2009 Emmanuel Pacaud
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

#include <lsmdebug.h>
#include <lsmdomdocument.h>
#include <lsmsvgelement.h>
#include <lsmsvgview.h>
#include <string.h>

static GObjectClass *parent_class;

/* LsmDomNode implementation */

static gboolean
lsm_svg_element_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return (LSM_IS_SVG_ELEMENT (child));
}

static gboolean
lsm_svg_element_child_changed (LsmDomNode *parent, LsmDomNode *child)
{
	LsmSvgElement *element = LSM_SVG_ELEMENT (parent);

	if (element->need_children_update)
		return FALSE;

	element->need_children_update = TRUE;

	return TRUE;
}

/* LsmDomElement implementation */

static void
lsm_svg_element_set_attribute (LsmDomElement *self, const char* name, const char *value)
{
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_GET_CLASS(self);

	lsm_debug ("[LsmSvgElement::set_attribute] node = %s, name = %s, value = %s",
		    lsm_dom_node_get_node_name (LSM_DOM_NODE (self)), name, value);

	lsm_dom_attribute_map_set_attribute (s_element_class->attributes, self,
					     name, value);
}

const char *
lsm_svg_element_get_attribute (LsmDomElement *self, const char *name)
{
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_GET_CLASS(self);

	return lsm_dom_attribute_map_get_attribute (s_element_class->attributes, self, name);
}

/* LsmSvgElement implementation */

void
lsm_svg_element_update (LsmSvgElement *self, const LsmSvgStyle *parent_style)
{
	LsmSvgElementClass *element_class;
	LsmSvgStyle *style;
	LsmDomDocument *document;
	LsmDomNode *node;

	g_return_if_fail (LSM_IS_SVG_ELEMENT (self));
	g_return_if_fail (parent_style != NULL);

	if (!self->need_update && !self->need_children_update) {
		lsm_debug ("[Element::update] %s already up to date",
			    lsm_dom_node_get_node_name (LSM_DOM_NODE (self)));
		return;
	}

	document = lsm_dom_node_get_owner_document (LSM_DOM_NODE (self));
	if (document != NULL)
		lsm_dom_document_register_element (document, LSM_DOM_ELEMENT (self), self->id.value);

	element_class = LSM_SVG_ELEMENT_GET_CLASS (self);

	style = lsm_svg_style_duplicate (parent_style);
	g_return_if_fail (style != NULL);

	lsm_debug ("[Element::update] update %s",
		    lsm_dom_node_get_node_name (LSM_DOM_NODE (self)));

	if (element_class->update != NULL)
		element_class->update (self, style);

	for (node = LSM_DOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (LSM_IS_SVG_ELEMENT (node)) {
			if (self->need_update)
				LSM_SVG_ELEMENT (node)->need_update = TRUE;
			lsm_svg_element_update (LSM_SVG_ELEMENT (node), style);
		}

	lsm_svg_style_free (style);

	self->need_update = FALSE;
	self->need_children_update = FALSE;
}

/* LsmSvgElement implementation */

static void
_render (LsmSvgElement *element, LsmSvgView *view)
{
	LsmDomNode *node;

	lsm_debug ("[LsmSvgElement::_render");

	for (node = LSM_DOM_NODE (element)->first_child; node != NULL; node = node->next_sibling)
		if (LSM_IS_SVG_ELEMENT (node))
		    lsm_svg_element_render (LSM_SVG_ELEMENT (node), view);
}

void
lsm_svg_element_render (LsmSvgElement *element, LsmSvgView *view)
{
	LsmSvgElementClass *element_class;

	g_return_if_fail (LSM_IS_SVG_ELEMENT (element));

	element_class = LSM_SVG_ELEMENT_GET_CLASS (element);
	if (element_class->render != NULL) {
		lsm_debug ("[LsmSvgElement::render] Render %s (%s)",
			    lsm_dom_node_get_node_name (LSM_DOM_NODE (element)),
			    element->id.value != NULL ? element->id.value : "no id");

		element_class->render (element, view);
	}
}

void
lsm_svg_element_render_paint (LsmSvgElement *element, LsmSvgView *view)
{
	LsmSvgElementClass *element_class;

	g_return_if_fail (LSM_IS_SVG_ELEMENT (element));

	element_class = LSM_SVG_ELEMENT_GET_CLASS (element);
	if (element_class->render_paint != NULL) {
		lsm_debug ("[LsmSvgElement::render_paint] Render %s (%s)",
			    lsm_dom_node_get_node_name (LSM_DOM_NODE (element)),
			    element->id.value != NULL ? element->id.value : "no id");

		element_class->render_paint (element, view);
	}
}

static void
lsm_svg_element_init (LsmSvgElement *element)
{
	element->need_update = TRUE;
}

static void
lsm_svg_element_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgElement class */

static void
lsm_svg_element_class_init (LsmSvgElementClass *s_element_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_element_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_element_class);
	LsmDomElementClass *d_element_class = LSM_DOM_ELEMENT_CLASS (s_element_class);

	parent_class = g_type_class_peek_parent (s_element_class);

	object_class->finalize = lsm_svg_element_finalize;

	d_node_class->can_append_child = lsm_svg_element_can_append_child;
	d_node_class->child_changed = lsm_svg_element_child_changed;

	d_element_class->get_attribute = lsm_svg_element_get_attribute;
	d_element_class->set_attribute = lsm_svg_element_set_attribute;

	s_element_class->update = NULL;
	s_element_class->render = _render;
	s_element_class->render_paint = NULL;

	s_element_class->attributes = lsm_dom_attribute_map_new ();

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "id",
					  offsetof (LsmSvgElement, id));
}

G_DEFINE_ABSTRACT_TYPE (LsmSvgElement, lsm_svg_element, LSM_TYPE_DOM_ELEMENT)
