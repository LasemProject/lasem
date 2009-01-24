/*
 * Copyright © 2009 Emmanuel Pacaud
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
#include <gsvgview.h>
#include <gsvgdocument.h>
#include <gsvgelement.h>
#include <glib/gprintf.h>

#include <math.h>
#include <string.h>

static GObjectClass *parent_class;

struct _GSvgViewPrivate {

	GSvgDocument *document;

	PangoFontDescription *	font_description;
	PangoLayout*		pango_layout;
	cairo_t *		cairo;

	gboolean debug;
};

void
gsvg_view_measure (GSvgView *view, double *width, double *height)
{
}

void
gsvg_view_render (GSvgView *view, double x, double y)
{
}

void
gsvg_view_set_debug (GSvgView *view, gboolean debug)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	view->priv->debug = debug;
}

void
gsvg_view_set_cairo (GSvgView *view, cairo_t *cairo)
{
	PangoContext *context;
	PangoFontDescription *font_description;
	cairo_font_options_t *font_options;
	const cairo_font_options_t *current_font_options;
	cairo_surface_t *surface;
	cairo_surface_type_t type;

	g_return_if_fail (GSVG_IS_VIEW (view));

	if (view->priv->cairo == cairo)
		return;

	if (view->priv->cairo != NULL) {
		cairo_destroy (view->priv->cairo);
		g_object_unref (view->priv->pango_layout);
	}

	if (cairo == NULL) {
		view->priv->cairo = NULL;
		view->priv->pango_layout = NULL;

		return;
	}

	font_description = view->priv->font_description;

	cairo_reference (cairo);
	view->priv->cairo = cairo;
	view->priv->pango_layout = pango_cairo_create_layout (cairo);

	surface = cairo_get_target (cairo);

	type = cairo_surface_get_type (surface);

	context = pango_layout_get_context (view->priv->pango_layout);
	pango_cairo_context_set_resolution (context, 72);

	current_font_options = pango_cairo_context_get_font_options (context);
	if (current_font_options == NULL)
		font_options = cairo_font_options_create ();
	else
		font_options = cairo_font_options_copy (current_font_options);
	cairo_font_options_set_hint_metrics (font_options, CAIRO_HINT_METRICS_OFF);
	pango_cairo_context_set_font_options (context, font_options);
	cairo_font_options_destroy (font_options);
}

void
gsvg_view_set_document (GSvgView *view, GSvgDocument *document)
{
	g_return_if_fail (GSVG_IS_VIEW (view));
	g_return_if_fail (document == NULL || GSVG_IS_DOCUMENT (document));

	if (view->priv->document == document)
		return;

	if (view->priv->document != NULL)
		g_object_unref (view->priv->document);

	if (document != NULL)
	    g_object_ref (document);

	view->priv->document = document;
}

GSvgView *
gsvg_view_new (GSvgDocument *document, cairo_t *cairo)
{
	GSvgView *view;

	view = g_object_new (GSVG_TYPE_VIEW, NULL);

	gsvg_view_set_document (view, document);
	gsvg_view_set_cairo (view, cairo);

	return view;
}

static void
gsvg_view_init (GSvgView *view)
{
	view->priv = G_TYPE_INSTANCE_GET_PRIVATE (view, GSVG_TYPE_VIEW, GSvgViewPrivate);
	view->priv->font_description = pango_font_description_new ();

	view->priv->pango_layout = NULL;
	view->priv->cairo = NULL;
}

static void
gsvg_view_finalize (GObject *object)
{
	GSvgView *view = GSVG_VIEW (object);

	g_object_unref (view->priv->document);

	if (view->priv->pango_layout != NULL)
		g_object_unref (view->priv->pango_layout);
	if (view->priv->cairo != NULL)
		cairo_destroy (view->priv->cairo);

	pango_font_description_free (view->priv->font_description);

	parent_class->finalize (object);
}

static void
gsvg_view_class_init (GSvgViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = gsvg_view_finalize;

	g_type_class_add_private (object_class, sizeof (GSvgViewPrivate));
}

G_DEFINE_TYPE (GSvgView, gsvg_view, G_TYPE_OBJECT)
