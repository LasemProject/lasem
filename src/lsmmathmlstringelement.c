/*
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

#include <lsmdebug.h>
#include <lsmmathmlstringelement.h>
#include <lsmmathmlview.h>

static GObject *parent_class;

/* LsmDomNode implementation */

static const char *
lsm_mathml_string_element_get_node_name (LsmDomNode *node)
{
	return "ms";
}

/* LsmMathmlElement implementation */

static void
lsm_mathml_string_element_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlStringElement *string_element = LSM_MATHML_STRING_ELEMENT (self);
	char *default_quote;

	LSM_MATHML_ELEMENT_CLASS (parent_class)->update (self, style);

	default_quote = g_strdup ("\"");
	lsm_dom_string_attribute_parse (&string_element->left_quote, &default_quote);
	g_free (default_quote);

	default_quote = g_strdup ("\"");
	lsm_dom_string_attribute_parse (&string_element->right_quote, &default_quote);
	g_free (default_quote);
}

/* LsmMathmlPresentationToken implementation */

static char *
lsm_mathml_string_element_get_text (LsmMathmlPresentationToken *token)
{
	LsmMathmlStringElement *string = LSM_MATHML_STRING_ELEMENT (token);
	char *token_text;
	char *text;

	token_text = LSM_MATHML_PRESENTATION_TOKEN_CLASS (parent_class)->get_text (token);

	text = g_strdup_printf ("%s%s%s",
				string->left_quote.value != NULL ? string->left_quote.value : "",
				token_text != NULL ? token_text : "",
				string->right_quote.value != NULL ? string->right_quote.value : "");

	g_free (token_text);

	return text;
}

/* LsmMathmlStringElement implementation */

LsmDomNode *
lsm_mathml_string_element_new (void)
{
	return g_object_new (LSM_TYPE_MATHML_STRING_ELEMENT, NULL);
}

static void
lsm_mathml_string_element_init (LsmMathmlStringElement *self)
{
}

/* LsmMathmlStringElement class */

static void
lsm_mathml_string_element_class_init (LsmMathmlStringElementClass *string_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (string_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (string_class);
	LsmMathmlPresentationTokenClass *m_token_class = LSM_MATHML_PRESENTATION_TOKEN_CLASS (string_class);

	parent_class = g_type_class_peek_parent (string_class);

	d_node_class->get_node_name = lsm_mathml_string_element_get_node_name;

	m_element_class->update = lsm_mathml_string_element_update;

	m_token_class->get_text = lsm_mathml_string_element_get_text;

	m_element_class->attributes = lsm_mathml_attribute_map_duplicate (m_element_class->attributes);

	lsm_mathml_attribute_map_add_string (m_element_class->attributes, "lquote",
				       offsetof (LsmMathmlStringElement, left_quote));
	lsm_mathml_attribute_map_add_string (m_element_class->attributes, "rquote",
				       offsetof (LsmMathmlStringElement, right_quote));
}

G_DEFINE_TYPE (LsmMathmlStringElement, lsm_mathml_string_element, LSM_TYPE_MATHML_PRESENTATION_TOKEN)

