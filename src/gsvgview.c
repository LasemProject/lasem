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
#include <gsvgutils.h>
#include <glib/gprintf.h>

#include <math.h>
#include <string.h>

static GObjectClass *parent_class;

/*
 * Code for _calc_angle and _cairo_elliptical_arc comes from the
 * goocanvas library (goocanvasutils.c)
 *
 * GooCanvas. Copyright (C) 2005 Damon Chaplin.
 */

static double
_calc_angle (double ux, double uy, double vx, double vy)
{
	double top, u_magnitude, v_magnitude, angle_cos, angle;

	top = ux * vx + uy * vy;
	u_magnitude = sqrt (ux * ux + uy * uy);
	v_magnitude = sqrt (vx * vx + vy * vy);
	angle_cos = top / (u_magnitude * v_magnitude);

	/* We check if the cosine is slightly out-of-bounds. */
	if (angle_cos >= 1.0)
		angle = 0.0;
	if (angle_cos <= -1.0)
		angle = M_PI;
	else
		angle = acos (angle_cos);

	if (ux * vy - uy * vx < 0)
		angle = - angle;

	return angle;
}

static void
_cairo_elliptical_arc (cairo_t *cairo, double rx, double ry, double x_axis_rotation,
		       gboolean large_arc_flag, gboolean sweep_flag, double x, double y)
{
	double x1, y1, x2, y2, lambda;
	double v1, v2, angle, angle_sin, angle_cos, x11, y11;
	double rx_squared, ry_squared, x11_squared, y11_squared, top, bottom;
	double c, cx1, cy1, cx, cy, start_angle, angle_delta;

	cairo_get_current_point (cairo, &x1, &y1);

	x2 = x;
	y2 = y;

	/* If the endpoints are exactly the same, just return (see SVG spec). */
	if (x1 == x2 && y1 == y2)
		return;

	/* If either rx or ry is 0, do a simple lineto (see SVG spec). */
	if (rx == 0.0 || ry == 0.0)
	{
		cairo_line_to (cairo, x2, y2);
		return;
	}

	/* Calculate x1' and y1' (as per SVG implementation notes). */
	v1 = (x1 - x2) / 2.0;
	v2 = (y1 - y2) / 2.0;

	angle = x_axis_rotation * (M_PI / 180.0);
	angle_sin = sin (angle);
	angle_cos = cos (angle);

	x11 = (angle_cos * v1) + (angle_sin * v2);
	y11 = - (angle_sin * v1) + (angle_cos * v2);

	/* Ensure rx and ry are positive and large enough. */
	rx = rx > 0.0 ? rx : - rx;
	ry = ry > 0.0 ? ry : - ry;
	lambda = (x11 * x11) / (rx * rx) + (y11 * y11) / (ry * ry);
	if (lambda > 1.0)
	{
		gdouble square_root = sqrt (lambda);
		rx *= square_root;
		ry *= square_root;
	}

	/* Calculate cx' and cy'. */
	rx_squared = rx * rx;
	ry_squared = ry * ry;
	x11_squared = x11 * x11;
	y11_squared = y11 * y11;

	top = (rx_squared * ry_squared) - (rx_squared * y11_squared)
		- (ry_squared * x11_squared);
	if (top < 0.0)
	{
		c = 0.0;
	}
	else
	{
		bottom = (rx_squared * y11_squared) + (ry_squared * x11_squared);
		c = sqrt (top / bottom);
	}

	if (large_arc_flag == sweep_flag)
		c = - c;

	cx1 = c * ((rx * y11) / ry);
	cy1 = c * (- (ry * x11) / rx);

	/* Calculate cx and cy. */
	cx = (angle_cos * cx1) - (angle_sin * cy1) + (x1 + x2) / 2;
	cy = (angle_sin * cx1) + (angle_cos * cy1) + (y1 + y2) / 2;

	/* Calculate the start and end angles. */
	v1 = (x11 - cx1) / rx;
	v2 = (y11 - cy1) / ry;

	start_angle = _calc_angle (1, 0, v1, v2);
	angle_delta = _calc_angle (v1, v2, (-x11 - cx1) / rx, (-y11 - cy1) / ry);

	if (sweep_flag == 0 && angle_delta > 0.0)
		angle_delta -= 2 * M_PI;
	else if (sweep_flag == 1 && angle_delta < 0.0)
		angle_delta += 2 * M_PI;

	/* Now draw the arc. */
	cairo_save (cairo);
	cairo_translate (cairo, cx, cy);
	cairo_rotate (cairo, angle);
	cairo_scale (cairo, rx, ry);

	if (angle_delta > 0.0)
		cairo_arc (cairo, 0.0, 0.0, 1.0,
			   start_angle, start_angle + angle_delta);
	else
		cairo_arc_negative (cairo, 0.0, 0.0, 1.0,
				    start_angle, start_angle + angle_delta);

	cairo_restore (cairo);
}

static void
_cairo_rel_elliptical_arc (cairo_t *cairo, double rx, double ry, double x_axis_rotation,
			   gboolean large_arc_flag, gboolean sweep_flag, double dx, double dy)
{
	double x, y;

	cairo_get_current_point (cairo, &x, &y);

	_cairo_elliptical_arc (cairo, rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x +dx ,y + dy);
}

static void
_cairo_vertical (cairo_t *cairo, double y)
{
	double x0, y0;

	cairo_get_current_point (cairo, &x0, &y0);
	cairo_line_to (cairo, x0, y);
}

static void
_cairo_rel_vertical (cairo_t *cairo, double dy)
{
	double x0, y0;

	cairo_get_current_point (cairo, &x0, &y0);
	cairo_line_to (cairo, x0, y0 + dy);
}

static void
_cairo_horizontal (cairo_t *cairo, double x)
{
	double x0, y0;

	cairo_get_current_point (cairo, &x0, &y0);
	cairo_line_to (cairo, x, y0);
}

static void
_cairo_rel_horizontal (cairo_t *cairo, double dx)
{
	double x0, y0;

	cairo_get_current_point (cairo, &x0, &y0);
	cairo_line_to (cairo, x0 + dx, y0);
}

static void
_emit_function_1 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double))
{
	double value;

	gsvg_str_skip_spaces (path);

	while (gsvg_str_parse_double_list (path, 1, &value))
		cairo_func (cr, value);
}

static void
_emit_function_2 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double, double))
{
	double values[2];

	gsvg_str_skip_spaces (path);

	while (gsvg_str_parse_double_list (path, 2, values))
		cairo_func (cr, values[0], values[1]);
}

static void
_emit_function_6 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double, double, double ,double, double, double))
{
	double values[6];

	gsvg_str_skip_spaces (path);

	while (gsvg_str_parse_double_list (path, 6, values))
		cairo_func (cr, values[0], values[1], values[2], values[3], values[4], values[5]);
}

static void
_emit_function_7 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double, double, double ,gboolean, gboolean, double, double))
{
	double values[7];

	gsvg_str_skip_spaces (path);

	while (gsvg_str_parse_double_list (path, 7, values))
		cairo_func (cr, values[0], values[1], values[2],
			    values[3], values[4], values[5], values[6]);
}

static void
_emit_svg_path (cairo_t *cr, char const *path)
{
	char *ptr;

	g_return_if_fail (cr != NULL);

	if (path == NULL)
		return;

	ptr = (char *) path;

	gsvg_str_skip_spaces (&ptr);

	while (*ptr != '\0') {
		if (*ptr == 'M') {
			ptr++;
			_emit_function_2 (&ptr, cr, cairo_move_to);
		} else if (*ptr == 'm') {
			ptr++;
			_emit_function_2 (&ptr, cr, cairo_rel_move_to);
		} else if (*ptr == 'L') {
			ptr++;
			_emit_function_2 (&ptr, cr, cairo_line_to);
		} else if (*ptr == 'l') {
			ptr++;
			_emit_function_2 (&ptr, cr, cairo_rel_line_to);
		} else if (*ptr == 'C') {
			ptr++;
			_emit_function_6 (&ptr, cr, cairo_curve_to);
		} else if (*ptr == 'c') {
			ptr++;
			_emit_function_6 (&ptr, cr, cairo_rel_curve_to);
		} else if (*ptr == 'V') {
			ptr++;
			_emit_function_1 (&ptr, cr, _cairo_vertical);
		} else if (*ptr == 'v') {
			ptr++;
			_emit_function_1 (&ptr, cr, _cairo_rel_vertical);
		} else if (*ptr == 'H') {
			ptr++;
			_emit_function_1 (&ptr, cr, _cairo_horizontal);
		} else if (*ptr == 'h') {
			ptr++;
			_emit_function_1 (&ptr, cr, _cairo_rel_horizontal);
		} else if (*ptr == 'A') {
			ptr++;
			_emit_function_7 (&ptr, cr, _cairo_elliptical_arc);
		} else if (*ptr == 'a') {
			ptr++;
			_emit_function_7 (&ptr, cr, _cairo_rel_elliptical_arc);
		} else if (*ptr == 'Z' || *ptr == 'z') {
			ptr++;
			cairo_close_path (cr);
		} else
			ptr++;
	}
}

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

/*
 * Code for show_rectangle and show ellipse is inspired from
 * the librsvg library (rsvg-shapes.c)
 *
 * Copyright (C) 2000 Eazel, Inc.
 * Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>
 *
 * Authors: Raph Levien <raph@artofcode.com>, 
 *          Dom Lachowicz <cinamod@hotmail.com>, 
 *          Caleb Moore <c.moore@student.unsw.edu.au>
 */

void
gsvg_view_show_rectangle (GSvgView *view,
			  double x, double y,
			  double w, double h,
			  double rx, double ry)
{
	cairo_t *cairo;

	g_return_if_fail (GSVG_IS_VIEW (view));

	cairo = view->dom_view.cairo;

	if (rx <= 0.0 || ry <= 0.0)
		cairo_rectangle (cairo, x, y, w, h);
	else {

		if (rx > fabs (w / 2.))
			rx = fabs (w / 2.);
		if (ry > fabs (h / 2.))
			ry = fabs (h / 2.);

		cairo_move_to (cairo, x + rx, y);
		cairo_line_to (cairo, x + w - rx, y);
		_cairo_elliptical_arc (cairo, rx, ry, 0.0, FALSE, TRUE, x + w, y + ry);
		cairo_line_to (cairo, x + w, y + h -ry);
		_cairo_elliptical_arc (cairo, rx, ry, 0.0, FALSE, TRUE, x + w - rx, y + h);
		cairo_line_to (cairo, x + rx, y + h);
		_cairo_elliptical_arc (cairo, rx, ry, 0.0, FALSE, TRUE, x, y + h - ry);
		cairo_line_to (cairo, x, y + ry);
		_cairo_elliptical_arc (cairo, rx, ry, 0.0, FALSE, TRUE, x + rx, y);
		cairo_close_path (cairo);
	}

	_paint (view);
}

void
gsvg_view_show_circle (GSvgView *view, double cx, double cy, double r)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	cairo_arc (view->dom_view.cairo, cx, cy, r, 0, 2 * M_PI);

	_paint (view);
}

#define GSVG_ARC_MAGIC ((double) 0.5522847498) /* 4/3 * (1-cos 45)/sin 45 = 4/3 * sqrt(2) - 1 */

void
gsvg_view_show_ellipse (GSvgView *view, double cx, double cy, double rx, double ry)
{
	cairo_t *cairo;

	g_return_if_fail (GSVG_IS_VIEW (view));

	if (rx <= 0 || ry <= 0)
		return;

	cairo = view->dom_view.cairo;

	cairo_move_to (cairo, cx + rx, cy);
	cairo_curve_to (cairo, cx + rx, cy - GSVG_ARC_MAGIC * ry, cx + GSVG_ARC_MAGIC * rx, cy - ry, cx, cy - ry);
	cairo_curve_to (cairo, cx - GSVG_ARC_MAGIC * rx, cy - ry, cx - rx, cy - GSVG_ARC_MAGIC * ry, cx - rx, cy);
	cairo_curve_to (cairo, cx - rx, cy + GSVG_ARC_MAGIC * ry, cx - GSVG_ARC_MAGIC * rx, cy + ry, cx, cy + ry);
	cairo_curve_to (cairo, cx + GSVG_ARC_MAGIC * rx, cy + ry, cx + rx, cy + GSVG_ARC_MAGIC * ry, cx + rx, cy);
	cairo_close_path (cairo);

	_paint (view);
}

void
gsvg_view_show_path (GSvgView *view,
		     const char *d)
{
	g_return_if_fail (GSVG_IS_VIEW (view));

	_emit_svg_path (view->dom_view.cairo, d);

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
