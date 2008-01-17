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
#include <math.h>

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
gmathml_script_element_update_child_pointers (GMathmlScriptElement *self)
{
	GDomNode *node;

	node = GDOM_NODE (self)->first_child;
	if (node == NULL) {
		self->base = NULL;
		self->subscript = NULL;
		self->superscript = NULL;
		return;
	} else
		self->base = GMATHML_ELEMENT (node);

	node = node->next_sibling;
	if (node != NULL)
		switch (self->type) {
			case GMATHML_SCRIPT_ELEMENT_TYPE_SUP:
				self->subscript = NULL;
				self->superscript = GMATHML_ELEMENT (node);
				break;
			case GMATHML_SCRIPT_ELEMENT_TYPE_SUB:
				self->subscript = GMATHML_ELEMENT (node);
				self->superscript = NULL;
				break;
			default:
				self->subscript = GMATHML_ELEMENT (node);
				node = node->next_sibling;
				if (node != NULL)
					self->superscript = GMATHML_ELEMENT (node);
		}
	else {
		self->subscript = NULL;
		self->superscript = NULL;
	}
}

static void
gmathml_script_element_post_new_child (GDomNode *self, GDomNode *child)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);

	if (child != self->first_child) {
#warning TODO
/*                GMathmlElement *child_element = GMATHML_ELEMENT (child);*/

/*                gmathml_increment_attribute_set_default (&child_element->style_attrs.script_level,*/
/*                                                         1, GMATHML_LEVEL_TYPE_UP);*/
/*                gmathml_boolean_attribute_set_default (&child_element->style_attrs.display_style, FALSE);*/
	}

	gmathml_script_element_update_child_pointers (script);
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
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);
	const GMathmlBbox *base_bbox, *subscript_bbox, *superscript_bbox;
	double super_shift, sub_shift;

	if (script->base == NULL)
		return;

	base_bbox = script->base != NULL ?  gmathml_element_measure (script->base, view) : NULL;
	subscript_bbox = script->subscript != NULL ?  gmathml_element_measure (script->subscript, view) : NULL;
	superscript_bbox = script->superscript != NULL ?  gmathml_element_measure (script->superscript, view) : NULL;

	/* Find automatic position */

	super_shift = gmathml_view_get_length_ex (view, 1.0);
	sub_shift = gmathml_view_get_length_ex (view, 0.5);

	g_message ("super_shift = %g", super_shift);
	g_message ("sub_shift   = %g", sub_shift);

	if (superscript_bbox != NULL && subscript_bbox != NULL) {
		double delta = (super_shift + sub_shift) - (superscript_bbox->depth + subscript_bbox->height);
		if (delta < 0.0) {
			super_shift += fabs (delta) * 0.5;
			sub_shift += fabs (delta) * 0.5;
		}
	}

#warning TODO
/*        super_shift = gmathml_view_get_length (view, &script->superscript_shift, super_shift);*/
/*        sub_shift = gmathml_view_get_length (view, &script->subscript_shift, sub_shift);*/

	g_message ("super_shift = %g", super_shift);
	g_message ("sub_shift   = %g", sub_shift);

	gmathml_element_layout (script->base, view, x, y, base_bbox);
	if (script->subscript)
		gmathml_element_layout (script->subscript, view,
					x + base_bbox->width,
					y + sub_shift, subscript_bbox);
	if (script->superscript)
		gmathml_element_layout (script->superscript, view,
					x + base_bbox->width,
					y - super_shift, superscript_bbox);
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
