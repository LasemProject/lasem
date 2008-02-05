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
#include <gmathmlmathelement.h>
#include <gmathmlstyleelement.h>

#include <math.h>

static GObjectClass *parent_class;

struct _GMathmlViewPrivate {

	gboolean debug;

	GMathmlDocument *document;

	PangoLayout *pango_layout;
	PangoFontDescription *font_description;

	gboolean is_vector;

	/* rendering context */
	cairo_t *cairo;

	const GMathmlElement *current_element;
	GSList *elements;
};

double
gmathml_view_measure_space (GMathmlView *view, double space)
{
	g_return_val_if_fail (GMATHML_IS_VIEW (view), 1.0);

	if (view->priv->is_vector)
		return space;

	space = floor (space + 0.5);

	if (space > 1.0)
		return space;

	return 1.0;
}

double
gmathml_view_measure_fraction_offset (GMathmlView *view, double font_size)
{
	PangoRectangle rect, ink_rect;
	PangoLayoutIter *iter;
	int baseline;

	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	pango_font_description_set_size (view->priv->font_description, font_size * PANGO_SCALE);
	pango_layout_set_text (view->priv->pango_layout, "+", -1);
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
	pango_layout_get_extents (view->priv->pango_layout, &ink_rect, &rect);

	iter = pango_layout_get_iter (view->priv->pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	return gmathml_view_measure_space (view, pango_units_to_double (baseline -
									ink_rect.y -
									0.5 * ink_rect.height));
}

void
gmathml_view_measure_text (GMathmlView *view, char const *text, GMathmlBbox *bbox)
{
	GMathmlPresentationToken *token;
	PangoRectangle rect, ink_rect;
	PangoLayoutIter *iter;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_PRESENTATION_TOKEN (view->priv->current_element));

	token = GMATHML_PRESENTATION_TOKEN (view->priv->current_element);

	if (text == NULL)
		return;

	g_return_if_fail (bbox != NULL);

	pango_font_description_set_size (view->priv->font_description,
					 token->math_size.value * PANGO_SCALE);
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
	pango_layout_get_extents (view->priv->pango_layout, &ink_rect, &rect);

	iter = pango_layout_get_iter (view->priv->pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	bbox->width = pango_units_to_double (ink_rect.width);
	bbox->height = pango_units_to_double (baseline - ink_rect.y);
	bbox->depth = pango_units_to_double (ink_rect.height + ink_rect.y - baseline);
}

void
gmathml_view_show_text (GMathmlView *view, double x, double y, char const *text)
{
	GMathmlPresentationToken *token;
	PangoRectangle rect, ink_rect;
	PangoLayoutIter *iter;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_PRESENTATION_TOKEN (view->priv->current_element));

	token = GMATHML_PRESENTATION_TOKEN (view->priv->current_element);

	if (text == NULL)
		return;

	g_message ("View: show_text %s at %g, %g (size = %g) %s",
		   text, x, y, token->math_size.value,
		   gmathml_variant_to_string (token->math_variant.value));

	cairo_set_source_rgba (view->priv->cairo,
			       token->math_color.color.red,
			       token->math_color.color.green,
			       token->math_color.color.blue,
			       token->math_color.color.alpha);

	pango_font_description_set_size (view->priv->font_description,
					 token->math_size.value * PANGO_SCALE);
	switch (token->math_variant.value) {
		case GMATHML_VARIANT_NORMAL:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			break;
		case GMATHML_VARIANT_ITALIC:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_ITALIC);
			break;
		default:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			break;
	}
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
	pango_layout_get_extents (view->priv->pango_layout, &ink_rect, &rect);

	iter = pango_layout_get_iter (view->priv->pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	cairo_move_to (view->priv->cairo, x - pango_units_to_double (ink_rect.x), y - pango_units_to_double (baseline));
	pango_cairo_show_layout (view->priv->cairo, view->priv->pango_layout);
}

void
gmathml_view_show_bbox (GMathmlView *view, double x, double y, const GMathmlBbox *bbox)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (view->priv->debug) {
		cairo_move_to (view->priv->cairo, x, y);
		cairo_set_line_width (view->priv->cairo, 0.1);
		cairo_set_source_rgb (view->priv->cairo, 1,0,0);
		cairo_rectangle (view->priv->cairo, x, y, bbox->width, -bbox->height);
		cairo_rectangle (view->priv->cairo, x, y, bbox->width, bbox->depth);
		cairo_stroke (view->priv->cairo);
	}
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

void
gmathml_view_draw_fraction_line (GMathmlView *view,
				 double x, double y,
				 double width, double thickness,
				 GMathmlColor *color)
{
	cairo_t *cairo;
	int rounded_thickness;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	cairo = view->priv->cairo;

	rounded_thickness = thickness + 0.5;
	if ((rounded_thickness % 2) == 0) {
		y = (int) (y + 0.5);
		x = (int) (x + 0.5);
	} else {
		y = +0.5 + (int ) y;
		x = +0.5 + (int ) x;
	}

	g_message ("y = %g, thickness = %d", y, rounded_thickness);

	cairo_set_line_width (cairo, rounded_thickness);
	cairo_set_source_rgba (cairo, color->red, color->green, color->blue,
			       thickness < 1.0 ? thickness : color->alpha);
	cairo_move_to (cairo, x, y);
	cairo_line_to (cairo, x + width, y);
	cairo_stroke (cairo);
}

void
gmathml_view_draw_root (GMathmlView *view,
			double x, double y,
			double width, double height,
			double top_width,
			double thickness,
			GMathmlColor *color)
{
	cairo_t *cairo;
	int rounded_thickness;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	cairo = view->priv->cairo;

	rounded_thickness = thickness + 0.5;
	if (rounded_thickness <= 0)
		rounded_thickness = 1;

	cairo_save (cairo);
	cairo_set_line_cap (cairo, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_width (cairo, 2.0 * thickness);

	cairo_set_source_rgba (cairo, color->red, color->green, color->blue, color->alpha);
	cairo_move_to (cairo, x, y - height * 0.5);
	cairo_line_to (cairo, x + width * 0.3, y);
	cairo_stroke (cairo);

	cairo_set_line_width (cairo, thickness);
	cairo_line_to (cairo, x + width * 0.3, y);
	cairo_line_to (cairo, x + width, y - height);
	cairo_stroke (cairo);

	cairo_set_line_width (cairo, rounded_thickness);
	if (rounded_thickness % 2 == 0) {
		cairo_move_to (cairo, x + width, (int) (y - height - rounded_thickness * 0.5 + 0.5));
		cairo_line_to (cairo, x + width + top_width, (int) (y - height -rounded_thickness * 0.5 + 0.5));
	} else {
		cairo_move_to (cairo, x + width, +0.5 + (int) (y - height - rounded_thickness * 0.5));
		cairo_line_to (cairo, x + width + top_width, +0.5 + (int) (y - height -rounded_thickness * 0.5));
	}
	cairo_stroke (cairo);

	cairo_restore (cairo);
}

void
gmathml_view_measure (GMathmlView *view, double *width, double *height)
{
	GDomElement *root;
	const GMathmlBbox *bbox;

	if (width != NULL)
		*width = 0.0;
	if (height != NULL)
		*height = 0.0;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_MATH_ELEMENT (root));

	gmathml_element_update (GMATHML_ELEMENT (root), view,
				gmathml_math_element_get_default_style (GMATHML_MATH_ELEMENT (root)));

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view);

	if (bbox != NULL) {
		if (width != NULL)
			*width = bbox->width;
		if (height != NULL)
			*height = bbox->height + bbox->depth;
	}
}

void
gmathml_view_render (GMathmlView *view)
{
	GDomElement *root;
	cairo_t *cairo;
	const GMathmlBbox *bbox;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_MATH_ELEMENT (root));

	cairo = view->priv->cairo;

	view->priv->current_element = NULL;
	view->priv->elements = NULL;

	gmathml_element_update (GMATHML_ELEMENT (root), view,
				gmathml_math_element_get_default_style (GMATHML_MATH_ELEMENT (root)));

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view);

	g_message ("bbox = %g, %g, %g", bbox->width, bbox->height, bbox->depth);

	gmathml_element_layout (GMATHML_ELEMENT (root), view, 0, 0, bbox);

	cairo_save (cairo);
	cairo_translate (cairo, 0, bbox->height);
	gmathml_element_render (GMATHML_ELEMENT (root), view);
	cairo_restore (cairo);

	if (view->priv->elements != NULL) {
		g_warning ("[GMathmlView::render] dangling elements (check push/pop element calls)");
		g_slist_free (view->priv->elements);
		view->priv->elements = NULL;
	}
}

void
gmathml_view_set_debug (GMathmlView *view, gboolean debug)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	view->priv->debug = debug;
}

void
gmathml_view_set_cairo (GMathmlView *view, cairo_t *cairo)
{
	PangoContext *context;
	cairo_font_options_t *font_options;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (cairo != NULL);

	if (view->priv->cairo != NULL) {
		cairo_destroy (view->priv->cairo);
		g_object_unref (view->priv->pango_layout);
	}

	cairo_reference (cairo);
	view->priv->cairo = cairo;
	view->priv->pango_layout = pango_cairo_create_layout (cairo);
	view->priv->font_description = pango_font_description_new();

	context = pango_layout_get_context (view->priv->pango_layout);

	font_options = cairo_font_options_create ();

	cairo_font_options_set_antialias (font_options, CAIRO_ANTIALIAS_GRAY);

	pango_cairo_context_set_font_options (context, font_options);

	cairo_font_options_destroy (font_options);

	pango_font_description_set_family (view->priv->font_description, "Serif");
}

GMathmlView *
gmathml_view_new (GMathmlDocument *document, cairo_t *cairo)
{
	GMathmlView *view;

	g_return_val_if_fail (GMATHML_IS_DOCUMENT (document), NULL);

	view = g_object_new (GMATHML_TYPE_VIEW, NULL);

	g_object_ref (document);
	view->priv->document = document;

	gmathml_view_set_cairo (view, cairo);

	return view;
}

static void
gmathml_view_init (GMathmlView *view)
{
	view->priv = G_TYPE_INSTANCE_GET_PRIVATE (view, GMATHML_TYPE_VIEW, GMathmlViewPrivate);
}

static void
gmathml_view_finalize (GObject *object)
{
	GMathmlView *view = GMATHML_VIEW (object);

	g_object_unref (view->priv->document);
	g_object_unref (view->priv->pango_layout);
/*        g_object_unref (view->priv->font_description);*/
	cairo_destroy (view->priv->cairo);

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
