/* lsmmathmloperatorelement.c
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

#include <lsmdomdebug.h>
#include <lsmmathmloperatorelement.h>
#include <lsmmathmloperatordictionary.h>
#include <lsmmathmlview.h>

static GObject *parent_class;

/* LsmDomNode implementation */

static const char *
lsm_mathml_operator_element_get_node_name (LsmDomNode *node)
{
	return "mo";
}

static const LsmMathmlOperatorDictionaryEntry *
lsm_mathml_operator_element_dictionary_lookup (LsmMathmlOperatorElement *operator)
{
	const LsmMathmlOperatorDictionaryEntry *entry;
	LsmDomNode *node = LSM_DOM_NODE (operator);
	LsmMathmlForm form;
	char *text;

	for (node = LSM_DOM_NODE (operator);
	     node->parent_node != NULL &&
	     LSM_MATHML_IS_ELEMENT (node->parent_node) &&
	     lsm_mathml_element_get_embellished_core (LSM_MATHML_ELEMENT (node->parent_node)) == operator;
	     node = node->parent_node);

	text = lsm_mathml_presentation_token_get_text (LSM_MATHML_PRESENTATION_TOKEN (operator));

	if (LSM_MATHML_IS_ELEMENT (node->parent_node) &&
	    lsm_mathml_element_is_inferred_row (LSM_MATHML_ELEMENT (node->parent_node))) {
		if ((node->previous_sibling != NULL && node->next_sibling != NULL) ||
		    (node->previous_sibling == NULL && node->next_sibling == NULL))
			form = LSM_MATHML_FORM_INFIX;
		else if (node->previous_sibling == NULL)
			form = LSM_MATHML_FORM_PREFIX;
		else
			form = LSM_MATHML_FORM_POSTFIX;
	} else
		form = LSM_MATHML_FORM_INFIX;

	lsm_mathml_form_attribute_parse (&operator->form, &form);

	entry = lsm_mathml_operator_dictionary_lookup (text, form);

	g_free (text);

	return entry;
}

static void
lsm_mathml_operator_element_post_new_child (LsmDomNode *self, LsmDomNode *child)
{
	LsmMathmlOperatorElement *operator_element = LSM_MATHML_OPERATOR_ELEMENT (self);
	const LsmMathmlOperatorDictionaryEntry *entry;
	gboolean flag;

	entry = lsm_mathml_operator_element_dictionary_lookup (operator_element);

	lsm_dom_debug ("[OperatorElement::post_new_child] found %s %s",
		    lsm_mathml_form_to_string (entry->form), entry->name);

	flag = entry->stretchy;
	lsm_dom_boolean_attribute_parse (&operator_element->stretchy, &flag);
	flag = entry->fence;
	lsm_dom_boolean_attribute_parse (&operator_element->fence, &flag);
	flag = entry->accent;
	lsm_dom_boolean_attribute_parse (&operator_element->accent, &flag);
}

/* LsmMathmlElement implementation */

static void
lsm_mathml_operator_element_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlOperatorElement *operator_element = LSM_MATHML_OPERATOR_ELEMENT (self);
	const LsmMathmlOperatorDictionaryEntry *entry;
	LsmMathmlSpace space;
	gboolean flag;

	LSM_MATHML_ELEMENT_CLASS (parent_class)->update (self, style);

	entry = lsm_mathml_operator_element_dictionary_lookup (operator_element);

	lsm_dom_debug ("[OperatorElement::update] found %s %s",
		    lsm_mathml_form_to_string (entry->form), entry->name);

	space = entry->left_space;
	lsm_mathml_space_attribute_parse (&operator_element->left_space, &space, style);
	space = entry->right_space;
	lsm_mathml_space_attribute_parse (&operator_element->right_space, &space, style);
	flag = entry->stretchy;
	lsm_dom_boolean_attribute_parse (&operator_element->stretchy, &flag);
	flag = entry->fence;
	lsm_dom_boolean_attribute_parse (&operator_element->fence, &flag);
	flag = entry->accent;
	lsm_dom_boolean_attribute_parse (&operator_element->accent, &flag);

	if (operator_element->accent.value)
		lsm_dom_debug ("[OperatorElement::update] Is accent");

	flag = entry->large_op;
	lsm_dom_boolean_attribute_parse (&operator_element->large_op, &flag);
	flag = entry->movable_limits;
	lsm_dom_boolean_attribute_parse (&operator_element->movable_limits, &flag);
	flag = entry->separator;
	lsm_dom_boolean_attribute_parse (&operator_element->separator, &flag);
	space = entry->min_size;
	lsm_mathml_space_attribute_parse (&operator_element->min_size, &space, style);
	space = entry->max_size;
	lsm_mathml_space_attribute_parse (&operator_element->max_size, &space, style);
	flag = entry->symmetric;
	lsm_dom_boolean_attribute_parse (&operator_element->symmetric, &flag);

	operator_element->is_large_op = operator_element->large_op.value &&
		(style->display == LSM_MATHML_DISPLAY_BLOCK);
	if (operator_element->is_large_op)
		lsm_dom_debug ("[OperatorElement::update] Large op");
}

static const LsmMathmlBbox *
lsm_mathml_operator_element_measure (LsmMathmlElement *self, LsmMathmlView *view, const LsmMathmlBbox *stretch_bbox)
{
	LsmMathmlOperatorElement *operator_element = LSM_MATHML_OPERATOR_ELEMENT (self);
	char *text;

	text = lsm_mathml_presentation_token_get_text (LSM_MATHML_PRESENTATION_TOKEN (self));

	self->bbox = lsm_mathml_bbox_null;

	lsm_mathml_view_measure_operator (view, &self->style,
				       text,
				       operator_element->is_large_op,
				       operator_element->symmetric.value,
				       lsm_mathml_view_measure_axis_offset (view, self->style.math_size),
				       operator_element->stretchy.value ? stretch_bbox : &lsm_mathml_bbox_null,
				       &self->bbox);

	g_free (text);

	self->bbox.width += self->style.math_size * LSM_MATHML_SPACE_EM_VERY_THIN;

	return &self->bbox;
}

static void
lsm_mathml_operator_element_render (LsmMathmlElement *self, LsmMathmlView *view)
{
	LsmMathmlOperatorElement *operator_element = LSM_MATHML_OPERATOR_ELEMENT (self);
	LsmMathmlBbox stretch_bbox;
	char *text;

	text = lsm_mathml_presentation_token_get_text (LSM_MATHML_PRESENTATION_TOKEN (self));

	stretch_bbox = self->bbox;
	stretch_bbox.width -= self->style.math_size * LSM_MATHML_SPACE_EM_VERY_THIN;

	lsm_mathml_view_show_operator (view, &self->style,
				    self->x + 0.5 * self->style.math_size * LSM_MATHML_SPACE_EM_VERY_THIN,
				    self->y, text,
				    operator_element->is_large_op, &stretch_bbox);

	g_free (text);
}

static LsmMathmlOperatorElement *
lsm_mathml_operator_element_get_embellished_core (const LsmMathmlElement *self)
{
	return LSM_MATHML_OPERATOR_ELEMENT (self);
}

/* LsmMathmlOperatorElement implementation */

double
lsm_mathml_operator_element_get_slant (const LsmMathmlOperatorElement *operator_element, LsmMathmlView *view)
{
	char *text;

	g_return_val_if_fail (LSM_MATHML_IS_OPERATOR_ELEMENT (operator_element), 0.0);

	text = lsm_mathml_presentation_token_get_text (LSM_MATHML_PRESENTATION_TOKEN (operator_element));

	return lsm_mathml_view_get_operator_slant (view, &LSM_MATHML_ELEMENT (operator_element)->style, text);
}

LsmDomNode *
lsm_mathml_operator_element_new (void)
{
	return g_object_new (LSM_MATHML_TYPE_OPERATOR_ELEMENT, NULL);
}

static void
lsm_mathml_operator_element_init (LsmMathmlOperatorElement *self)
{
}

/* LsmMathmlOperatorElement class */

static void
lsm_mathml_operator_element_class_init (LsmMathmlOperatorElementClass *operator_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (operator_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (operator_class);

	parent_class = g_type_class_peek_parent (operator_class);

	d_node_class->get_node_name = lsm_mathml_operator_element_get_node_name;
	d_node_class->post_new_child = lsm_mathml_operator_element_post_new_child;

	m_element_class->update = lsm_mathml_operator_element_update;
	m_element_class->measure = lsm_mathml_operator_element_measure;
	m_element_class->render = lsm_mathml_operator_element_render;
	m_element_class->get_embellished_core = lsm_mathml_operator_element_get_embellished_core;

	m_element_class->attributes = lsm_dom_attribute_map_duplicate (m_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "form",
					  offsetof (LsmMathmlOperatorElement, form));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "fence",
					  offsetof (LsmMathmlOperatorElement, fence));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "separator",
					  offsetof (LsmMathmlOperatorElement, separator));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "lspace",
					  offsetof (LsmMathmlOperatorElement, left_space));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "rspace",
					  offsetof (LsmMathmlOperatorElement, right_space));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "stretchy",
					  offsetof (LsmMathmlOperatorElement, stretchy));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "symmetric",
					  offsetof (LsmMathmlOperatorElement, symmetric));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "accent",
					  offsetof (LsmMathmlOperatorElement, accent));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "minsize",
					  offsetof (LsmMathmlOperatorElement, min_size));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "maxsize",
					  offsetof (LsmMathmlOperatorElement, max_size));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "largeop",
					  offsetof (LsmMathmlOperatorElement, large_op));
	lsm_dom_attribute_map_add_attribute (m_element_class->attributes, "movablelimits",
					  offsetof (LsmMathmlOperatorElement, movable_limits));
}

G_DEFINE_TYPE (LsmMathmlOperatorElement, lsm_mathml_operator_element, LSM_MATHML_TYPE_PRESENTATION_TOKEN)
