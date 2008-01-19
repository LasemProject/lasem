/* gmathmlpresentationtoken.c
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

#include <gmathmlpresentationtoken.h>
#include <gmathmlview.h>
#include <gdomtext.h>

/* GDomNode implementation */

static const char *
gmathml_presentation_token_get_node_name (GDomNode *node)
{
	GMathmlPresentationToken *token = GMATHML_PRESENTATION_TOKEN (node);

	switch (token->type) {
		case GMATHML_PRESENTATION_TOKEN_TYPE_NUMBER:
			return "mn";
		case GMATHML_PRESENTATION_TOKEN_TYPE_IDENTIFIER:
			return "mi";
		case GMATHML_PRESENTATION_TOKEN_TYPE_TEXT:
		default:
			return "mtext";
	}
}

static gboolean
gmathml_presentation_token_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GDOM_IS_TEXT (child) /*||
		GMATHML_IS_GLYPH_ELEMENT (child) ||
		GMATHML_IS_ALIGN_MARK_ELEMENT (child)*/);
}

/* GMathmlElement implementation */

static void
gmathml_presentation_token_update (GMathmlElement *self, GMathmlView *view)
{
	GMathmlPresentationToken *token = GMATHML_PRESENTATION_TOKEN (self);
	GDomNode *parent;

	for (parent = GDOM_NODE (self)->parent_node;
	     parent != NULL && !GMATHML_IS_PRESENTATION_TOKEN (parent);
	     parent = parent->parent_node);

	if (GMATHML_IS_PRESENTATION_TOKEN (parent)) {
		GMathmlPresentationToken *parent_token;

		parent_token = GMATHML_PRESENTATION_TOKEN (parent);

		gmathml_attribute_length_parse (&token->math_size,
						parent_token->math_size.value,
						gmathml_view_get_em_length (view));
		gmathml_attribute_color_parse (&token->math_color,
					       parent_token->math_color.red,
					       parent_token->math_color.green,
					       parent_token->math_color.blue,
					       parent_token->math_color.alpha);
		gmathml_attribute_color_parse (&token->math_background,
					       parent_token->math_background.red,
					       parent_token->math_background.green,
					       parent_token->math_background.blue,
					       parent_token->math_background.alpha);
	} else {
		gmathml_attribute_length_parse (&token->math_size, 12.0, gmathml_view_get_em_length (view));
		gmathml_attribute_color_parse (&token->math_color, 0.0, 0.0, 0.0, 1.0);
		gmathml_attribute_color_parse (&token->math_background, 0.0, 0.0, 0.0, 1.0);
	}
}

static char *
gmathml_presentation_token_get_text (GMathmlPresentationToken *self)
{
	GDomNode *node;
	GString *string = g_string_new ("");
	char *text;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GDOM_IS_TEXT (node)) {
			g_string_append (string, gdom_node_get_node_value (node));
		}
	}

	text = g_strdup (/* FIXME is it safe ? */g_strstrip (string->str));

	g_string_free (string, TRUE);

	return text;
}

static const GMathmlBbox *
gmathml_presentation_token_measure (GMathmlElement *self, GMathmlView *view)
{
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_measure_text (view, text, &self->bbox);

	g_free (text);

	return &self->bbox;
}

static void
gmathml_presentation_token_layout (GMathmlElement *self, GMathmlView *view,
				   double x, double y, const GMathmlBbox *bbox)
{
}

static void
gmathml_presentation_token_render (GMathmlElement *self, GMathmlView *view)
{
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_show_text (view, self->x, self->y, text);

	g_free (text);
}

GDomNode *
gmathml_number_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_PRESENTATION_TOKEN, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_PRESENTATION_TOKEN (node)->type = GMATHML_PRESENTATION_TOKEN_TYPE_NUMBER;

	return node;
}

GDomNode *
gmathml_identifier_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_PRESENTATION_TOKEN, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_PRESENTATION_TOKEN (node)->type = GMATHML_PRESENTATION_TOKEN_TYPE_IDENTIFIER;

	return node;
}

GDomNode *
gmathml_text_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_PRESENTATION_TOKEN, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_PRESENTATION_TOKEN (node)->type = GMATHML_PRESENTATION_TOKEN_TYPE_TEXT;

	return node;
}

static void
gmathml_presentation_token_init (GMathmlPresentationToken *token)
{
}

/* GMathmlPresentationToken class */

void
gmathml_element_class_add_token_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathvariant",
					     offsetof (GMathmlPresentationToken, math_variant));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathsize",
					     offsetof (GMathmlPresentationToken, math_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathcolor",
					     offsetof (GMathmlPresentationToken, math_color));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathbackground",
					     offsetof (GMathmlPresentationToken, math_background));
}

static void
gmathml_presentation_token_class_init (GMathmlPresentationTokenClass *token_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (token_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (token_class);

	d_node_class->get_node_name = gmathml_presentation_token_get_node_name;
	d_node_class->can_append_child = gmathml_presentation_token_can_append_child;

	m_element_class->layout = gmathml_presentation_token_layout;
	m_element_class->measure = gmathml_presentation_token_measure;
	m_element_class->render = gmathml_presentation_token_render;

	m_element_class->update = gmathml_presentation_token_update;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_token_attributes (m_element_class);
}

G_DEFINE_TYPE (GMathmlPresentationToken, gmathml_presentation_token, GMATHML_TYPE_ELEMENT)
