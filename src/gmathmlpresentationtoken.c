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

static gboolean
gmathml_presentation_token_can_append_child (GDomNode *self, GDomNode *child)
{
	return (GDOM_IS_TEXT (child) /*||
		GMATHML_IS_GLYPH_ELEMENT (child) ||
		GMATHML_IS_ALIGN_MARK_ELEMENT (child)*/);
}

/* GMathmlElement implementation */

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

	text = g_strdup (string->str);

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
	GMathmlPresentationToken *token = GMATHML_PRESENTATION_TOKEN (self);
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_set_color (view, &token->math_color.color);
	gmathml_view_show_text (view, self->x, self->y, text);

	g_free (text);
}

static void
gmathml_presentation_token_init (GMathmlPresentationToken *token)
{
}

/* GMathmlPresentationToken class */

void
gmathml_element_class_add_token_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attributes_add_attribute (m_element_class->attributes, "mathcolor", GMATHML_ATTRIBUTE_COLOR,
					  offsetof (GMathmlPresentationToken, math_color));
	gmathml_attributes_add_attribute (m_element_class->attributes, "mathbackground", GMATHML_ATTRIBUTE_COLOR,
					  offsetof (GMathmlPresentationToken, math_background));
}

static void
gmathml_presentation_token_class_init (GMathmlPresentationTokenClass *token_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (token_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (token_class);

	d_node_class->can_append_child = gmathml_presentation_token_can_append_child;

	m_element_class->layout = gmathml_presentation_token_layout;
	m_element_class->measure = gmathml_presentation_token_measure;
	m_element_class->render = gmathml_presentation_token_render;

	m_element_class->attributes = gmathml_attributes_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_style_attributes (m_element_class);
	gmathml_element_class_add_token_attributes (m_element_class);
}

G_DEFINE_ABSTRACT_TYPE (GMathmlPresentationToken, gmathml_presentation_token, GMATHML_TYPE_ELEMENT)
