/* gmathmlscriptelement.c
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

#include <gmathmlscriptelement.h>
#include <gmathmlview.h>

/* GdomNode implementation */

static const char *
gmathml_script_element_get_node_name (GDomNode *node)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (node);

	switch (script->type) {
		case GMATHML_SCRIPT_ELEMENT_TYPE_SUB:
			return "msub";
		case GMATHML_SCRIPT_ELEMENT_TYPE_SUP:
			return "msup";
		case GMATHML_SCRIPT_ELEMENT_TYPE_SUB_SUP:
		default:
			return "msubsup";
	}
}

static gboolean
gmathml_script_element_can_append_child (GDomNode *self, GDomNode *child)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);

	if (!GMATHML_ELEMENT (child))
		return FALSE;

	if (self->first_child == NULL ||
	    self->first_child->next_sibling == NULL)
		return TRUE;

	if (script->type != GMATHML_SCRIPT_ELEMENT_TYPE_SUB_SUP)
		return FALSE;

	return (self->first_child->next_sibling->next_sibling == NULL);
}

static void
gmathml_script_element_post_new_child (GDomNode *parent, GDomNode *child)
{
	if (child != parent->first_child) {
		GMathmlElement *child_element = GMATHML_ELEMENT (child);

		if (!child_element->style_attrs.script_level.is_defined)
			child_element->style_attrs.script_level.value =
				GMATHML_ELEMENT (parent)->style_attrs.script_level.value;
		if (!child_element->style_attrs.display_style.is_defined)
			child_element->style_attrs.display_style.value = FALSE;
	}
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_script_element_measure (GMathmlElement *element, GMathmlView *view)
{
	GDomNode *node = GDOM_NODE (element);
	GMathmlBbox const *base_bbox;
	GMathmlBbox const *child_bbox;
	GMathmlBbox children_bbox = {0.0, 0.0, 0.0};
	GMathmlScriptElementType type = GMATHML_SCRIPT_ELEMENT (element)->type;

	element->bbox.width = 0.0;
	element->bbox.height = 0.0;
	element->bbox.depth = 0.0;

	node = GDOM_NODE (element)->first_child;

	if (node != NULL) {
		base_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
		gmathml_bbox_add_to_right (&element->bbox, base_bbox);

		node = node->next_sibling;

		if (node != NULL) {
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node),
							      view);
			gmathml_bbox_add_to_right (&children_bbox, child_bbox);

			node = node->next_sibling;

			if (node != NULL && type == GMATHML_SCRIPT_ELEMENT_TYPE_SUB_SUP) {
				child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node),
								      view);
				gmathml_bbox_add_under (&children_bbox, child_bbox);
			}
			gmathml_bbox_add_to_right (&element->bbox, &children_bbox);
		}
	}

	return &element->bbox;
}

static void
gmathml_script_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
	double default_length = gmathml_view_get_length_ex (view, 1.0);

	g_message ("default_length = %g", default_length);
}

/* GMathmlScriptElement implementation */

GDomNode *
gmathml_sub_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_SCRIPT_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_SCRIPT_ELEMENT (node)->type = GMATHML_SCRIPT_ELEMENT_TYPE_SUB;

	return node;
}

GDomNode *
gmathml_sup_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_SCRIPT_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_SCRIPT_ELEMENT (node)->type = GMATHML_SCRIPT_ELEMENT_TYPE_SUP;

	return node;
}

GDomNode *
gmathml_sub_sup_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_SCRIPT_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_SCRIPT_ELEMENT (node)->type = GMATHML_SCRIPT_ELEMENT_TYPE_SUB_SUP;

	return node;
}

static void
gmathml_script_element_init (GMathmlScriptElement *self)
{
}

/* GMathmlScriptElement class */

static void
gmathml_script_element_class_init (GMathmlScriptElementClass *script_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (script_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (script_class);

	d_node_class->get_node_name = gmathml_script_element_get_node_name;
	d_node_class->can_append_child = gmathml_script_element_can_append_child;
	d_node_class->post_new_child = gmathml_script_element_post_new_child;

	m_element_class->measure = gmathml_script_element_measure;
	m_element_class->layout = gmathml_script_element_layout;
}

G_DEFINE_TYPE (GMathmlScriptElement, gmathml_script_element, GMATHML_TYPE_ELEMENT)
