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
#include <gmathmlstyleelement.h>

static GObjectClass *parent_class;

struct _GMathmlViewPrivate {

	GMathmlDocument *document;

	PangoLayout *pango_layout;
	PangoContext *pango_context;
	PangoFontDescription *font_description;

	/* rendering context */
	cairo_t *cairo;
	const GMathmlStyleElement *current_style;
	const GMathmlElement *current_element;
	GSList *styles;
	GSList *elements;
};

void
gmathml_view_measure_text (GMathmlView *view, char const *text, GMathmlBbox *bbox)
{
	GMathmlPresentationToken *token;
	PangoRectangle rect;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_PRESENTATION_TOKEN (view->priv->current_element));

	token = GMATHML_PRESENTATION_TOKEN (view->priv->current_element);

	if (text == NULL)
		return;

	g_return_if_fail (bbox != NULL);

	pango_font_description_set_size (view->priv->font_description,
					 token->math_size.value);
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_get_extents (view->priv->pango_layout, NULL, &rect);

	bbox->width = pango_units_to_double (rect.width);
	bbox->height = pango_units_to_double (PANGO_ASCENT (rect));
	bbox->depth = pango_units_to_double (PANGO_DESCENT (rect));
}

void
gmathml_view_show_text (GMathmlView *view, double x, double y, char const *text)
{
	GMathmlPresentationToken *token;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_PRESENTATION_TOKEN (view->priv->current_element));

	token = GMATHML_PRESENTATION_TOKEN (view->priv->current_element);

	if (text == NULL)
		return;

	g_message ("View: show_text %s at %g, %g", text, x, y);

	cairo_set_source_rgba (view->priv->cairo,
			       token->math_color.red,
			       token->math_color.green,
			       token->math_color.blue,
			       token->math_color.alpha);
	cairo_move_to (view->priv->cairo, x, y);

	pango_font_description_set_size (view->priv->font_description,
					 token->math_size.value);
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_cairo_show_layout (view->priv->cairo, view->priv->pango_layout);
}

void
gmathml_view_push_style (GMathmlView *view, const GMathmlStyleElement *style)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_STYLE_ELEMENT (style));

	view->priv->styles = g_slist_prepend (view->priv->styles, (void *) style);
	view->priv->current_style = style;
}

void
gmathml_view_pop_style (GMathmlView *view)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (view->priv->styles != NULL);

	view->priv->styles = g_slist_remove (view->priv->styles, view->priv->styles->data);

	if (view->priv->styles != NULL)
		view->priv->current_style = view->priv->styles->data;
	else
		view->priv->current_style = NULL;
}

void
gmathml_view_push_element (GMathmlView *view, const GMathmlElement *element)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (element));

	view->priv->elements = g_slist_prepend (view->priv->elements, (void *) element);
	view->priv->current_element = element;
}

void
gmathml_view_pop_element (GMathmlView *view)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (view->priv->elements != NULL);

	view->priv->elements = g_slist_remove (view->priv->elements, view->priv->elements->data);

	if (view->priv->elements != NULL)
		view->priv->current_element = view->priv->elements->data;
	else
		view->priv->current_element = NULL;
}

double
gmathml_view_get_font_size (GMathmlView *view)
{
	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	if (GMATHML_IS_PRESENTATION_TOKEN (view->priv->current_element))
		return 0.5 * GMATHML_PRESENTATION_TOKEN (view->priv->current_element)->math_size.value;
	else if (view->priv->current_style != NULL)
		return 0.5 * view->priv->current_style->math_size.value;

	g_warning ("[GMathmlView::get_font_size] can't get font size");
	return 0.0;
}

double
gmathml_view_get_ex_length (GMathmlView *view)
{
	return gmathml_view_get_font_size (view);
}

double
gmathml_view_get_em_length (GMathmlView *view)
{
	return 0.5 * gmathml_view_get_font_size (view);
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
	view->priv->current_style = NULL;
	view->priv->current_element = NULL;
	view->priv->styles = NULL;
	view->priv->elements = NULL;

	gmathml_element_update (GMATHML_ELEMENT (root), view);

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view);

	g_message ("bbox = %g, %g, %g", bbox->width, bbox->height, bbox->depth);

	gmathml_element_layout (GMATHML_ELEMENT (root), view, 0, 0, bbox);

	gmathml_element_render (GMATHML_ELEMENT (root), view);

	view->priv->cairo = NULL;

	if (view->priv->styles != NULL) {
		g_warning ("[GMathmlView::render] dangling styles (check push/pop style calls)");
		g_slist_free (view->priv->styles);
		view->priv->styles = NULL;
	}

	if (view->priv->elements != NULL) {
		g_warning ("[GMathmlView::render] dangling elements (check push/pop element calls)");
		g_slist_free (view->priv->elements);
		view->priv->elements = NULL;
	}
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
