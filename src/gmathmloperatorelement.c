/* gmathmloperatorelement.c
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

static const GMathmlOperatorDictionaryEntry *
gmathml_operator_element_dictionary_lookup (GMathmlOperatorElement *operator)
{
	const GMathmlOperatorDictionaryEntry *entry;
	GDomNode *node = GDOM_NODE (operator);
	GMathmlForm form;
	char *text;

	for (node = GDOM_NODE (operator);
	     node->parent_node != NULL &&
	     gmathml_element_get_embellished_core (GMATHML_ELEMENT (node->parent_node)) == operator;
	     node = node->parent_node);

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (operator));

	if (GMATHML_IS_ELEMENT (node->parent_node) &&
	    gmathml_element_is_inferred_row (GMATHML_ELEMENT (node->parent_node))) {
		if ((node->previous_sibling != NULL && node->next_sibling != NULL) ||
		    (node->previous_sibling == NULL && node->next_sibling == NULL))
			form = GMATHML_FORM_INFIX;
		else if (node->previous_sibling == NULL)
			form = GMATHML_FORM_PREFIX;
		else
			form = GMATHML_FORM_POSTFIX;
	} else
		form = GMATHML_FORM_INFIX;

	gmathml_attribute_form_parse (&operator->form, &form);

	entry = gmathml_operator_dictionary_lookup (text, form);

	g_free (text);

	return entry;
}

static void
gmathml_operator_element_post_new_child (GDomNode *self, GDomNode *child)
{
	GMathmlOperatorElement *operator_element = GMATHML_OPERATOR_ELEMENT (self);
	const GMathmlOperatorDictionaryEntry *entry;
	gboolean flag;

	entry = gmathml_operator_element_dictionary_lookup (operator_element);

	gdom_debug ("[OperatorElement::post_new_child] found %s %s",
		    gmathml_form_to_string (entry->form), entry->name);

	flag = entry->stretchy;
	gmathml_attribute_boolean_parse (&operator_element->stretchy, &flag);
	flag = entry->fence;
	gmathml_attribute_boolean_parse (&operator_element->fence, &flag);
	flag = entry->accent;
	gmathml_attribute_boolean_parse (&operator_element->accent, &flag);
}

/* GMathmlElement implementation */

static void
gmathml_operator_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlOperatorElement *operator_element = GMATHML_OPERATOR_ELEMENT (self);
	const GMathmlOperatorDictionaryEntry *entry;
	GMathmlSpace space;
	gboolean flag;

	entry = gmathml_operator_element_dictionary_lookup (operator_element);

	gdom_debug ("[OperatorElement::update] found %s %s",
		    gmathml_form_to_string (entry->form), entry->name);

	space = entry->left_space;
	gmathml_attribute_space_parse (&operator_element->left_space, &space, style);
	space = entry->right_space;
	gmathml_attribute_space_parse (&operator_element->right_space, &space, style);
	flag = entry->stretchy;
	gmathml_attribute_boolean_parse (&operator_element->stretchy, &flag);
	flag = entry->fence;
	gmathml_attribute_boolean_parse (&operator_element->fence, &flag);
	flag = entry->accent;
	gmathml_attribute_boolean_parse (&operator_element->accent, &flag);

	if (operator_element->accent.value)
		gdom_debug ("[OperatorElement::update] Is accent");

	flag = entry->large_op;
	gmathml_attribute_boolean_parse (&operator_element->large_op, &flag);
	flag = entry->movable_limits;
	gmathml_attribute_boolean_parse (&operator_element->movable_limits, &flag);
	flag = entry->separator;
	gmathml_attribute_boolean_parse (&operator_element->separator, &flag);
	space = entry->min_size;
	gmathml_attribute_space_parse (&operator_element->min_size, &space, style);
	space = entry->max_size;
	gmathml_attribute_space_parse (&operator_element->max_size, &space, style);
	flag = entry->symmetric;
	gmathml_attribute_boolean_parse (&operator_element->symmetric, &flag);

	operator_element->is_large_op = operator_element->large_op.value && style->display_style;
	if (operator_element->is_large_op)
		gdom_debug ("[OperatorElement::update] Large op");

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);
}

static const GMathmlBbox *
gmathml_operator_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *stretch_bbox)
{
	GMathmlOperatorElement *operator_element = GMATHML_OPERATOR_ELEMENT (self);
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	self->bbox = gmathml_bbox_null;

	gmathml_view_measure_operator (view, text,
				       self->style.math_size,
				       self->style.math_variant,
				       operator_element->is_large_op,
				       operator_element->symmetric.value,
				       gmathml_view_measure_axis_offset (view, self->style.math_size),
				       operator_element->stretchy.value ? stretch_bbox : &gmathml_bbox_null,
				       &self->bbox);

	g_free (text);

	return &self->bbox;
}

static void
gmathml_operator_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlOperatorElement *operator_element = GMATHML_OPERATOR_ELEMENT (self);
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_show_operator (view, self->x, self->y, text,
				    self->style.math_size,
				    self->style.math_variant,
				    &self->style.math_color,
				    operator_element->is_large_op, &self->bbox);

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
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (operator_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (operator_class);

	parent_class = g_type_class_peek_parent (operator_class);

	d_node_class->get_node_name = gmathml_operator_element_get_node_name;
	d_node_class->post_new_child = gmathml_operator_element_post_new_child;

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
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "largeop",
					     offsetof (GMathmlOperatorElement, large_op));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "movablelimits",
					     offsetof (GMathmlOperatorElement, movable_limits));
}

G_DEFINE_TYPE (GMathmlOperatorElement, gmathml_operator_element, GMATHML_TYPE_PRESENTATION_TOKEN)
