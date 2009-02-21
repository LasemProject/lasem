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
gsvg_view_push_transform (GSvgView *view, const GSvgMatrix *matrix)
{
	cairo_matrix_t cr_matrix;

	g_return_if_fail (GSVG_IS_VIEW (view));

	cairo_matrix_init (&cr_matrix, matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
	cairo_save (view->dom_view.cairo);
	cairo_transform (view->dom_view.cairo, &cr_matrix);
}

void
gsvg_view_pop_transform	(GSvgView *view)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	cairo_restore (view->dom_view.cairo);
}

void
gsvg_view_push_fill_attributes (GSvgView *view, GSvgFillAttributeBag *fill)
{
	g_return_if_fail (GSVG_IS_VIEW (view));
	g_return_if_fail (fill != NULL);

	view->fill_stack = g_slist_prepend (view->fill_stack, fill);
}

void
gsvg_view_pop_fill_attributes (GSvgView *view)
{
	g_return_if_fail (GSVG_IS_VIEW (view));
	g_return_if_fail (view->fill_stack != NULL);

	view->fill_stack = g_slist_delete_link (view->fill_stack, view->fill_stack);
}

void
gsvg_view_push_stroke_attributes (GSvgView *view, GSvgStrokeAttributeBag *stroke)
{
	g_return_if_fail (GSVG_IS_VIEW (view));
	g_return_if_fail (stroke != NULL);

	view->stroke_stack = g_slist_prepend (view->stroke_stack, stroke);
}

void
gsvg_view_pop_stroke_attributes (GSvgView *view)
{
	g_return_if_fail (GSVG_IS_VIEW (view));
	g_return_if_fail (view->stroke_stack != NULL);

	view->stroke_stack = g_slist_delete_link (view->stroke_stack, view->stroke_stack);
}

static gboolean
_set_color (cairo_t *cairo, const GSvgPaint *paint, double opacity)
{
	switch (paint->type) {
		case GSVG_PAINT_TYPE_NONE:
			return FALSE;
		case GSVG_PAINT_TYPE_RGB_COLOR:
			cairo_set_source_rgba (cairo,
					       paint->color.red,
					       paint->color.green,
					       paint->color.blue,
					       opacity);
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

static void
_paint (GSvgView *view)
{
	GSvgFillAttributeBag *fill;
	GSvgStrokeAttributeBag *stroke;
	cairo_t *cairo;

	g_return_if_fail (view->fill_stack != NULL);
	g_return_if_fail (view->stroke_stack != NULL);

	cairo = view->dom_view.cairo;
	fill = view->fill_stack->data;
	stroke = view->stroke_stack->data;

	if (_set_color (cairo, &fill->paint.paint, fill->opacity.value))
		cairo_fill_preserve (view->dom_view.cairo);

	if (_set_color (cairo, &stroke->paint.paint, stroke->opacity.value)) {
		cairo_set_line_width (cairo, stroke->width.length.value);
		cairo_stroke (cairo);
	}

	cairo_new_path (cairo);
}

void
gsvg_view_show_rectangle (GSvgView *view,
			  double x, double y,
			  double width, double height)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	cairo_rectangle (view->dom_view.cairo, x, y, width, height);

	_paint (view);
}

void
gsvg_view_show_path (GSvgView *view,
		     const char *d)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	gsvg_cairo_emit_svg_path (view->dom_view.cairo, d);

	_paint (view);
}

static void
gsvg_view_measure (GDomView *view, double *width, double *height)
{
	GSvgSvgElement *svg_element;

	svg_element = gsvg_document_get_svg_element (GSVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	gsvg_svg_element_update (svg_element);

	gsvg_svg_element_measure (svg_element, width, height);
}

static void
gsvg_view_render (GDomView *view, double x, double y)
{
	GSvgView *svg_view;
	GSvgSvgElement *svg_element;

	svg_view = GSVG_VIEW (view);

	svg_element = gsvg_document_get_svg_element (GSVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	gsvg_svg_element_update (svg_element);

	svg_view->fill_stack = NULL;
	svg_view->stroke_stack = NULL;

	gsvg_element_render (GSVG_ELEMENT (svg_element), GSVG_VIEW (view));

	if (svg_view->fill_stack != NULL) {
		g_warning ("[GSvgView::render] Dangling fill attribute in stack");
		g_slist_free (svg_view->fill_stack);
		svg_view->fill_stack = NULL;
	}
	if (svg_view->stroke_stack != NULL) {
		g_warning ("[GSvgView::render] Dangling stroke attribute in stack");
		g_slist_free (svg_view->stroke_stack);
		svg_view->stroke_stack = NULL;
	}
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
