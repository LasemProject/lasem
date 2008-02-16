/* gmathmloperatorelement.c
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

#include <gmathmloperatorelement.h>
#include <gmathmloperatordictionary.h>
#include <gmathmlview.h>

static GObject *parent_class;

/* GDomNode implementation */

static const char *
gmathml_operator_element_get_node_name (GDomNode *node)
{
	return "mo";
}

static void
gmathml_operator_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlOperatorElement *operator_element = GMATHML_OPERATOR_ELEMENT (self);
	GDomNode *node = GDOM_NODE (self);
	const GMathmlOperator *operator;
	GMathmlSpace space;
	GMathmlForm form;
	char *text;
	gboolean flag;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	if ((node->previous_sibling != NULL && node->next_sibling != NULL) ||
	    (node->previous_sibling == NULL && node->next_sibling == NULL))
		form = GMATHML_FORM_INFIX;
	else if (node->previous_sibling == NULL)
		form = GMATHML_FORM_PREFIX;
	else
		form = GMATHML_FORM_POSTFIX;

	gmathml_attribute_form_parse (&operator_element->form, &form);

	operator = gmathml_operator_get_attributes (text, form);

	g_message ("Find operator: %s", operator->name);

	space = operator->left_space;
	gmathml_attribute_space_parse (&operator_element->left_space, &space, style);
	space = operator->right_space;
	gmathml_attribute_space_parse (&operator_element->right_space, &space, style);
	flag = operator->stretchy;
	gmathml_attribute_boolean_parse (&operator_element->stretchy, &flag);
	flag = operator->fence;
	gmathml_attribute_boolean_parse (&operator_element->fence, &flag);
	flag = operator->accent;
	gmathml_attribute_boolean_parse (&operator_element->accent, &flag);
	flag = operator->large_op;
	gmathml_attribute_boolean_parse (&operator_element->large_op, &flag);
	flag = operator->movable_limits;
	gmathml_attribute_boolean_parse (&operator_element->movable_limits, &flag);
	flag = operator->separator;
	gmathml_attribute_boolean_parse (&operator_element->separator, &flag);
	space = operator->min_size;
	gmathml_attribute_space_parse (&operator_element->min_size, &space, style);
	space = operator->max_size;
	gmathml_attribute_space_parse (&operator_element->max_size, &space, style);
	flag = operator->symmetric;
	gmathml_attribute_boolean_parse (&operator_element->symmetric, &flag);

	g_free (text);

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);
}

static const GMathmlBbox *
gmathml_operator_element_measure (GMathmlElement *self, GMathmlView *view)
{
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_measure_text (view, text, &self->bbox);

	g_free (text);

	return &self->bbox;
}

static void
gmathml_operator_element_render (GMathmlElement *self, GMathmlView *view)
{
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_show_bbox (view, self->x, self->y, &self->bbox);
	gmathml_view_show_text (view, self->x, self->y, text);

	g_free (text);
}

static const GMathmlOperatorElement *
gmathml_operator_element_get_embellished_core (const GMathmlElement *self)
{
	return GMATHML_OPERATOR_ELEMENT (self);
}

/* GMathmlOperatorElement implementation */

GDomNode *
gmathml_operator_element_new (void)
{
	return g_object_new (GMATHML_TYPE_OPERATOR_ELEMENT, NULL);
}

static void
gmathml_operator_element_init (GMathmlOperatorElement *self)
{
}

/* GMathmlOperatorElement class */

static void
gmathml_operator_element_class_init (GMathmlOperatorElementClass *operator_class)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (operator_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (operator_class);

	parent_class = g_type_class_peek_parent (operator_class);

	node_class->get_node_name = gmathml_operator_element_get_node_name;

	m_element_class->update = gmathml_operator_element_update;
	m_element_class->measure = gmathml_operator_element_measure;
	m_element_class->render = gmathml_operator_element_render;
	m_element_class->get_embellished_core = gmathml_operator_element_get_embellished_core;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_presentation_token_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "form",
					     offsetof (GMathmlOperatorElement, form));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "fence",
					     offsetof (GMathmlOperatorElement, fence));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "separator",
					     offsetof (GMathmlOperatorElement, separator));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "lspace",
					     offsetof (GMathmlOperatorElement, left_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "rspace",
					     offsetof (GMathmlOperatorElement, right_space));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "stretchy",
					     offsetof (GMathmlOperatorElement, stretchy));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "symmetric",
					     offsetof (GMathmlOperatorElement, symmetric));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "accent",
					     offsetof (GMathmlOperatorElement, accent));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "movablelimits",
					     offsetof (GMathmlOperatorElement, movable_limits));
}

G_DEFINE_TYPE (GMathmlOperatorElement, gmathml_operator_element, GMATHML_TYPE_PRESENTATION_TOKEN)
