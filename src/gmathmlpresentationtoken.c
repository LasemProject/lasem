/* gmathmlpresentationtoken.c
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

#include <gmathmlpresentationtoken.h>
#include <gmathmlview.h>
#include <gdomtext.h>

#include <math.h>
#include <string.h>

static GObjectClass *parent_class;

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

static char *
_get_text (GMathmlPresentationToken *self)
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

char *
gmathml_presentation_token_get_text (GMathmlPresentationToken *self)
{
	GMathmlPresentationTokenClass *token_class;

	g_return_val_if_fail (GMATHML_IS_PRESENTATION_TOKEN (self), NULL);

	token_class = GMATHML_PRESENTATION_TOKEN_GET_CLASS (self);

	return token_class->get_text (self);
}

static void
gmathml_presentation_token_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlFontStyle font_style;
	GMathmlFontWeight font_weight;
	GMathmlPresentationToken *token = GMATHML_PRESENTATION_TOKEN (self);

	if (token->type == GMATHML_PRESENTATION_TOKEN_TYPE_IDENTIFIER) {
		char *text;
		text = gmathml_presentation_token_get_text (token);
		style->math_variant = g_utf8_strlen (text, -1) > 1 ? GMATHML_VARIANT_NORMAL : GMATHML_VARIANT_ITALIC;
		g_free (text);
	}

	font_style = GMATHML_FONT_STYLE_ERROR;
	gmathml_attribute_font_style_parse (&token->font_style, &font_style);

	font_weight = GMATHML_FONT_WEIGHT_ERROR;
	gmathml_attribute_font_weight_parse (&token->font_weight, &font_weight);

	gmathml_variant_set_font_style (&style->math_variant, font_style);
	gmathml_variant_set_font_weight (&style->math_variant, font_weight);

	gmathml_attribute_string_parse (&token->math_family, &style->math_family);
	gmathml_attribute_variant_parse (&token->math_variant, &style->math_variant);
	gmathml_attribute_length_parse (&token->math_size, &style->math_size, style->math_size_value);
	gmathml_attribute_color_parse (&token->math_color, &style->math_color);
	gmathml_attribute_color_parse (&token->math_background, &style->math_background);

	style->math_size_value = token->math_size.value;
}

static const GMathmlBbox *
gmathml_presentation_token_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	char *text;

	text = gmathml_presentation_token_get_text (GMATHML_PRESENTATION_TOKEN (self));

	gmathml_view_measure_text (view, &self->style, text, &self->bbox);

	g_free (text);

	self->bbox.width += self->style.math_size * GMATHML_SPACE_EM_VERY_THIN;

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

	gmathml_view_show_text (view, &self->style,
				self->x + 0.5 * self->style.math_size * GMATHML_SPACE_EM_VERY_THIN,
				self->y, text);

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
gmathml_element_class_add_presentation_token_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "mathfamily",
						  offsetof (GMathmlPresentationToken, math_family),
						  gmathml_attribute_string_finalize);
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathvariant",
					     offsetof (GMathmlPresentationToken, math_variant));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathsize",
					     offsetof (GMathmlPresentationToken, math_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathcolor",
					     offsetof (GMathmlPresentationToken, math_color));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "mathbackground",
					     offsetof (GMathmlPresentationToken, math_background));

	/* Deprecated attributes */

	gmathml_attribute_map_add_attribute_full (m_element_class->attributes, "fontfamily",
						  offsetof (GMathmlPresentationToken, math_family),
						  gmathml_attribute_string_finalize);
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "fontsize",
					     offsetof (GMathmlPresentationToken, math_size));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "color",
					     offsetof (GMathmlPresentationToken, math_color));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "fontweight",
					     offsetof (GMathmlPresentationToken, font_weight));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "fontstyle",
					     offsetof (GMathmlPresentationToken, font_style));
}

static void
gmathml_presentation_token_class_init (GMathmlPresentationTokenClass *m_token_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_token_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (m_token_class);

	parent_class = g_type_class_peek_parent (m_token_class);

	d_node_class->get_node_name = gmathml_presentation_token_get_node_name;
	d_node_class->can_append_child = gmathml_presentation_token_can_append_child;

	m_element_class->layout = gmathml_presentation_token_layout;
	m_element_class->measure = gmathml_presentation_token_measure;
	m_element_class->render = gmathml_presentation_token_render;
	m_element_class->is_inferred_row = NULL;

	m_element_class->update = gmathml_presentation_token_update;

	m_token_class->get_text = _get_text;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_presentation_token_attributes (m_element_class);
}

G_DEFINE_TYPE (GMathmlPresentationToken, gmathml_presentation_token, GMATHML_TYPE_ELEMENT)
