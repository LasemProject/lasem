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
#include <gsvgsvgelement.h>
#include <gsvgcairo.h>
#include <glib/gprintf.h>

#include <math.h>
#include <string.h>

static GObjectClass *parent_class;

void
gsvg_view_show_rectangle (GSvgView *view,
			  double x, double y,
			  double width, double height)
{
	cairo_rectangle (view->dom_view.cairo, x, y, width, height);
	cairo_stroke (view->dom_view.cairo);
}

void
gsvg_view_show_path (GSvgView *view,
		     const char *d)
{
	gsvg_cairo_emit_svg_path (view->dom_view.cairo, d);
	cairo_stroke (view->dom_view.cairo);
}

static void
gsvg_view_measure (GDomView *view, double *width, double *height)
{
	GSvgSvgElement *svg_element;

	svg_element = gsvg_document_get_svg_element (GSVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	gsvg_svg_element_measure (svg_element, width, height);
}

static void
gsvg_view_render (GDomView *view, double x, double y)
{
	GSvgSvgElement *svg_element;

	svg_element = gsvg_document_get_svg_element (GSVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	gsvg_svg_element_update (svg_element);

	gsvg_element_render (GSVG_ELEMENT (svg_element), GSVG_VIEW (view));
}

GSvgView *
gsvg_view_new (GSvgDocument *document)
{
	GSvgView *view;

	view = g_object_new (GSVG_TYPE_VIEW, NULL);

	gdom_view_set_document (GDOM_VIEW (view), GDOM_DOCUMENT (document));

	return view;
}

static void
gsvg_view_init (GSvgView *view)
{
}

static void
gsvg_view_finalize (GObject *object)
{
	parent_class->finalize (object);
}

static void
gsvg_view_class_init (GSvgViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);
	GDomViewClass *d_view_class = GDOM_VIEW_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = gsvg_view_finalize;

	d_view_class->measure = gsvg_view_measure;
	d_view_class->render = gsvg_view_render;
}

G_DEFINE_TYPE (GSvgView, gsvg_view, GDOM_TYPE_VIEW)
