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

#include <gdomdebug.h>
#include <gmathmlstringelement.h>
#include <gmathmlview.h>

static GObject *parent_class;

/* GDomNode implementation */

static const char *
gmathml_string_element_get_node_name (GDomNode *node)
{
	return "ms";
}

/* GMathmlElement implementation */

static void
gmathml_string_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlStringElement *string_element = GMATHML_STRING_ELEMENT (self);
	char *default_quote;

	GMATHML_ELEMENT_CLASS (parent_class)->update (self, style);

	default_quote = g_strdup ("\"");
	gdom_attribute_string_parse (&string_element->left_quote, &default_quote);
	g_free (default_quote);

	default_quote = g_strdup ("\"");
	gdom_attribute_string_parse (&string_element->right_quote, &default_quote);
	g_free (default_quote);
}

/* GMathmlPresentationToken implementation */

static char *
gmathml_string_element_get_text (GMathmlPresentationToken *token)
{
	GMathmlStringElement *string = GMATHML_STRING_ELEMENT (token);
	char *token_text;
	char *text;

	token_text = GMATHML_PRESENTATION_TOKEN_CLASS (parent_class)->get_text (token);

	text = g_strdup_printf ("%s%s%s",
				string->left_quote.value != NULL ? string->left_quote.value : "",
				token_text != NULL ? token_text : "",
				string->right_quote.value != NULL ? string->right_quote.value : "");

	g_free (token_text);

	return text;
}

/* GMathmlStringElement implementation */

GDomNode *
gmathml_string_element_new (void)
{
	return g_object_new (GMATHML_TYPE_STRING_ELEMENT, NULL);
}

static void
gmathml_string_element_init (GMathmlStringElement *self)
{
}

/* GMathmlStringElement class */

static void
gmathml_string_element_class_init (GMathmlStringElementClass *string_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (string_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (string_class);
	GMathmlPresentationTokenClass *m_token_class = GMATHML_PRESENTATION_TOKEN_CLASS (string_class);

	parent_class = g_type_class_peek_parent (string_class);

	d_node_class->get_node_name = gmathml_string_element_get_node_name;

	m_element_class->update = gmathml_string_element_update;

	m_token_class->get_text = gmathml_string_element_get_text;

	m_element_class->attributes = gdom_attribute_map_duplicate (m_element_class->attributes);

	gdom_attribute_map_add_string (m_element_class->attributes, "lquote",
				       offsetof (GMathmlStringElement, left_quote));
	gdom_attribute_map_add_string (m_element_class->attributes, "rquote",
				       offsetof (GMathmlStringElement, right_quote));
}

G_DEFINE_TYPE (GMathmlStringElement, gmathml_string_element, GMATHML_TYPE_PRESENTATION_TOKEN)

