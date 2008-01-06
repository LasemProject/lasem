/* gmathmlview.c
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

#include <gmathmlview.h>
#include <gmathmldocument.h>
#include <gmathmlelement.h>

static GObjectClass *parent_class;

struct _GMathmlViewPrivate {

	GMathmlDocument *document;

	PangoLayout *pango_layout;
	PangoContext *pango_context;
	PangoFontDescription *font_description;

	/* cairo != NULL only during rendering */
	cairo_t *cairo;
	const GMathmlStyleAttributes *style_attrs;
	const GMathmlTokenAttributes *token_attrs;

	double font_size;
};

void
gmathml_view_measure_text (GMathmlView *view, char const *text, GMathmlBbox *bbox)
{
	PangoRectangle rect;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (text == NULL)
		return;

	g_return_if_fail (bbox != NULL);
	g_return_if_fail (view->priv->style_attrs != NULL);
	g_return_if_fail (view->priv->token_attrs != NULL);

	pango_font_description_set_size (view->priv->font_description, 12);
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_get_extents (view->priv->pango_layout, NULL, &rect);

	bbox->width = pango_units_to_double (rect.width);
	bbox->height = pango_units_to_double (PANGO_ASCENT (rect));
	bbox->depth = pango_units_to_double (PANGO_DESCENT (rect));
}

void
gmathml_view_show_text (GMathmlView *view, double x, double y, char const *text)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (view->priv->style_attrs != NULL);
	g_return_if_fail (view->priv->token_attrs != NULL);

	g_message ("View: show_text %s at %g, %g", text, x, y);

	cairo_set_source_rgb (view->priv->cairo,
			      view->priv->token_attrs->math_color.color.red,
			      view->priv->token_attrs->math_color.color.green,
			      view->priv->token_attrs->math_color.color.blue);
	cairo_move_to (view->priv->cairo, x, y);
	pango_font_description_set_size (view->priv->font_description, 12);
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_cairo_show_layout (view->priv->cairo, view->priv->pango_layout);
}

void
gmathml_view_set_style (GMathmlView *view,
			const GMathmlStyleAttributes *style_attrs,
			const GMathmlTokenAttributes *token_attrs)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	view->priv->style_attrs = style_attrs;
	view->priv->token_attrs = token_attrs;
}

double
gmathml_view_get_length (GMathmlView *view, const GMathmlUnitAttribute *attr,
			 double default_value)
{
	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);
	g_return_val_if_fail (attr != NULL, 0.0);

	switch (attr->unit) {
		case GMATHML_UNIT_EX:
			return gmathml_view_get_length_ex (view, attr->value);
		case GMATHML_UNIT_EM:
			return gmathml_view_get_length_em (view, attr->value);
		case GMATHML_UNIT_PERCENT:
			return attr->value * default_value / 100.0;
		default:
			break;
	}

	return 0.0;
}

double
gmathml_view_get_length_ex (GMathmlView *view, double value)
{
	PangoRectangle rect;

	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	pango_layout_set_text (view->priv->pango_layout, "x", -1);
	pango_layout_get_extents (view->priv->pango_layout, NULL, &rect);

	return (pango_units_to_double (rect.height - rect.y));
}

double
gmathml_view_get_length_em (GMathmlView *view, double value)
{
	PangoRectangle rect;

	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	pango_layout_set_text (view->priv->pango_layout, "M", -1);
	pango_layout_get_extents (view->priv->pango_layout, NULL, &rect);

	return (pango_units_to_double (rect.width - rect.x));
}

void
gmathml_view_draw_line (GMathmlView *view, double x0, double y0, double x1, double y1)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	cairo_move_to (view->priv->cairo, x0, y0);
	cairo_line_to (view->priv->cairo, x1, y1);
	cairo_stroke (view->priv->cairo);
}

void
gmathml_view_render (GMathmlView *view, cairo_t *cr)
{
	GDomElement *root;
	const GMathmlBbox *bbox;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_ELEMENT (root));

	view->priv->cairo = cr;
	view->priv->style_attrs = NULL;
	view->priv->token_attrs = NULL;
	view->priv->font_size = 12; /* 12 pts */

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view);

	g_message ("bbox = %g, %g, %g", bbox->width, bbox->height, bbox->depth);

	gmathml_element_layout (GMATHML_ELEMENT (root), view, 0, 0, bbox);

	gmathml_element_render (GMATHML_ELEMENT (root), view);

	view->priv->cairo = NULL;
	view->priv->style_attrs = NULL;
	view->priv->token_attrs = NULL;
}

GMathmlView *
gmathml_view_new (GMathmlDocument *document)
{
	GMathmlView *view;

	g_return_val_if_fail (GMATHML_IS_DOCUMENT (document), NULL);

	view = g_object_new (GMATHML_TYPE_VIEW, NULL);

	g_object_ref (document);
	view->priv->document = document;

	return view;
}

static void
gmathml_view_init (GMathmlView *view)
{
	PangoCairoFontMap *fontmap;

	view->priv = G_TYPE_INSTANCE_GET_PRIVATE (view, GMATHML_TYPE_VIEW, GMathmlViewPrivate);

	fontmap = PANGO_CAIRO_FONT_MAP (pango_cairo_font_map_get_default ());
	view->priv->pango_context = pango_cairo_font_map_create_context (fontmap);
	view->priv->pango_layout = pango_layout_new (view->priv->pango_context);
	view->priv->font_description = pango_font_description_new ();
	pango_font_description_set_family (view->priv->font_description, "Times new roman");
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
}

static void
gmathml_view_finalize (GObject *object)
{
	GMathmlView *view = GMATHML_VIEW (object);

	g_object_unref (view->priv->document);
	g_object_unref (view->priv->pango_layout);
	g_object_unref (view->priv->pango_context);

	parent_class->finalize (object);
}

static void
gmathml_view_class_init (GMathmlViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = gmathml_view_finalize;

	g_type_class_add_private (object_class, sizeof (GMathmlViewPrivate));
}

G_DEFINE_TYPE (GMathmlView, gmathml_view, G_TYPE_OBJECT)
