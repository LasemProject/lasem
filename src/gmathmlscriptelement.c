/* gmathmlscriptelement.c
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

#include <gmathmlscriptelement.h>
#include <gmathmllayoututils.h>
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

	gmathml_script_element_update_child_pointers (script);
}

/* GMathmlElement implementation */

static void
gmathml_script_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);

	gmathml_length_attribute_parse (&script->superscript_shift, &style->superscript_shift, style->math_size_value);
	gmathml_length_attribute_parse (&script->subscript_shift, &style->subscript_shift, style->math_size_value);

	script->display = style->display;
}

static gboolean
gmathml_script_element_update_children (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);
	gboolean need_measure = FALSE;

	if (script->base != NULL)
		if (gmathml_element_update (GMATHML_ELEMENT (script->base), style))
			need_measure = TRUE;

	gmathml_style_change_script_level (style, +1);
	style->display = GMATHML_DISPLAY_INLINE;

	if (script->subscript != NULL)
		if (gmathml_element_update (GMATHML_ELEMENT (script->subscript), style))
			need_measure = TRUE;
	if (script->superscript != NULL)
		if  (gmathml_element_update (GMATHML_ELEMENT (script->superscript), style))
			need_measure = TRUE;

	return need_measure;
}

static const GMathmlBbox *
gmathml_script_element_measure (GMathmlElement *element, GMathmlView *view,
				const GMathmlBbox *stretch_bbox)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (element);

	gmathml_measure_sub_sup (element,view,
				 script->base,
				 script->subscript,
				 script->superscript,
				 script->subscript_shift.value,
				 script->superscript_shift.value,
				 script->display, stretch_bbox, &element->bbox,
				 &script->subscript_offset, &script->superscript_offset);

	return &element->bbox;
}

static void
gmathml_script_element_layout (GMathmlElement *self, GMathmlView *view,
			       double x, double y, const GMathmlBbox *bbox)
{
	GMathmlScriptElement *script = GMATHML_SCRIPT_ELEMENT (self);

	gmathml_layout_sub_sup (self, view, x, y, script->base, script->subscript, script->superscript,
				script->subscript_offset, script->superscript_offset);
}

static GMathmlOperatorElement *
gmathml_script_element_get_embellished_core (const GMathmlElement *self)
{
	if (GDOM_NODE (self)->first_child != NULL)
		return gmathml_element_get_embellished_core (GMATHML_ELEMENT (GDOM_NODE (self)->first_child));

	return NULL;
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

	m_element_class->update = gmathml_script_element_update;
	m_element_class->update_children = gmathml_script_element_update_children;
	m_element_class->measure = gmathml_script_element_measure;
	m_element_class->layout = gmathml_script_element_layout;
	m_element_class->get_embellished_core = gmathml_script_element_get_embellished_core;
	m_element_class->is_inferred_row = NULL;
}

G_DEFINE_TYPE (GMathmlScriptElement, gmathml_script_element, GMATHML_TYPE_ELEMENT)
