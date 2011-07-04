/* Lasem - SVG and Mathml library
 *
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmdebug.h>
#include <lsmsvgview.h>
#include <lsmsvgdocument.h>
#include <lsmsvgelement.h>
#include <lsmsvgsvgelement.h>
#include <lsmsvgradialgradientelement.h>
#include <lsmsvglineargradientelement.h>
#include <lsmsvgpatternelement.h>
#include <lsmsvgmarkerelement.h>
#include <lsmsvgclippathelement.h>
#include <lsmsvgmaskelement.h>
#include <lsmcairo.h>
#include <lsmstr.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib/gprintf.h>

#include <math.h>
#include <string.h>

static gboolean lsm_svg_view_circular_reference_check (LsmSvgView *view, LsmSvgElement *element);

static GObjectClass *parent_class;

typedef struct {
	gboolean is_text_path;
	gboolean is_extents_defined;
	LsmExtents extents;
	PangoLayout *pango_layout;
} LsmSvgViewPathInfos;

static LsmSvgViewPathInfos default_path_infos = {
	.is_text_path = FALSE,
	.is_extents_defined = FALSE,
	.extents = {0.0, 0.0, 0.0, 0.0},
	.pango_layout = NULL
};

struct _LsmSvgViewPatternData {
	cairo_t *old_cairo;

	cairo_pattern_t *pattern;

	LsmBox extents;

	double opacity;
};

double
lsm_svg_view_normalize_length (LsmSvgView *view, const LsmSvgLength *length, LsmSvgLengthDirection direction)
{
	double font_size;

	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), 0.0);

	/* TODO cache font size */
	if (view->viewbox_stack != NULL && view->style != NULL) {
		font_size = lsm_svg_length_normalize (&view->style->font_size->length, view->viewbox_stack->data,
						      0.0, LSM_SVG_LENGTH_DIRECTION_DIAGONAL);
	} else
		font_size = 0.0;

	return lsm_svg_length_normalize (length, view->viewbox_stack->data, font_size, direction);
}

static void
_start_pattern (LsmSvgView *view, const LsmBox *extents, double opacity)
{
	lsm_debug_render ("[LsmSvgView::start_pattern]");

	view->pattern_stack = g_slist_prepend (view->pattern_stack, view->pattern_data);

	view->pattern_data = g_new (LsmSvgViewPatternData, 1);
	view->pattern_data->old_cairo = view->dom_view.cairo;
	view->pattern_data->pattern = NULL;
	view->pattern_data->extents = *extents;
	view->pattern_data->opacity = opacity;

	view->dom_view.cairo = NULL;
}

static void
_end_pattern (LsmSvgView *view)
{
	g_return_if_fail (view->pattern_data != NULL);

	if (view->pattern_data->pattern != NULL)
		cairo_pattern_destroy (view->pattern_data->pattern);
	if (view->dom_view.cairo != NULL)
		cairo_destroy (view->dom_view.cairo);

	view->dom_view.cairo = view->pattern_data->old_cairo;

	g_free (view->pattern_data);

	if (view->pattern_stack != NULL) {
		view->pattern_data = view->pattern_stack->data;
		view->pattern_stack = g_slist_delete_link (view->pattern_stack, view->pattern_stack);
	} else
		view->pattern_data = NULL;

	lsm_debug_render ("[LsmSvgView::end_pattern]");
}

static void
_set_pattern (LsmSvgView *view, cairo_pattern_t *pattern)
{
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->pattern_data->pattern == NULL);

	view->pattern_data->pattern = pattern;
	view->last_stop_offset = 0.0;
}

void
lsm_svg_view_create_radial_gradient (LsmSvgView *view,
				     double cx, double cy,
				     double r,
				     double fx, double fy)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	_set_pattern (view, cairo_pattern_create_radial (fx, fy, 0, cx, cy, r));
}

void
lsm_svg_view_create_linear_gradient (LsmSvgView *view,
				     double x1, double y1,
				     double x2, double y2)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	_set_pattern (view, cairo_pattern_create_linear (x1, y1, x2, y2));
}

void
lsm_svg_view_add_gradient_color_stop (LsmSvgView *view, double offset)
{
	const LsmSvgStyle *style;
	const LsmSvgColor *color;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->pattern_data->pattern != NULL);

	if (offset > 1.0)
		offset = 1.0;

	if (offset < view->last_stop_offset)
		offset = view->last_stop_offset;
	else
		view->last_stop_offset = offset;

	style = view->style;

	lsm_debug_render ("[LsmSvgView::add_gradient_color_stop] opacity = %g", style->stop_opacity->value);

	color = &style->stop_color->value;

	if (color->red < 0.0 || color->blue < 0.0 || color->green < 0.0)
		color = &style->color->value;

	lsm_debug_render ("[LsmSvgView::add_gradient_color_stop] color = %2x%2x%2x",
		   (int) (255.0 * color->red),
		   (int) (255.0 * color->green),
		   (int) (255.0 * color->blue));

	cairo_pattern_add_color_stop_rgba (view->pattern_data->pattern, offset,
					   color->red,
					   color->green,
					   color->blue,
					   style->stop_opacity->value * view->pattern_data->opacity);
}

void
lsm_svg_view_set_gradient_properties (LsmSvgView *view,
				      LsmSvgSpreadMethod method,
				      LsmSvgPatternUnits units,
				      const LsmSvgMatrix *gradient_matrix)
{
	cairo_matrix_t matrix;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->pattern_data->pattern != NULL);

	switch (method) {
		case LSM_SVG_SPREAD_METHOD_REFLECT:
			cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_REFLECT);
			break;
		case LSM_SVG_SPREAD_METHOD_REPEAT:
			cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_REPEAT);
			break;
		default:
			cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_PAD);
	}

	if (gradient_matrix != NULL) {
		cairo_matrix_init (&matrix,
				   gradient_matrix->a, gradient_matrix->b,
				   gradient_matrix->c, gradient_matrix->d,
				   gradient_matrix->e, gradient_matrix->f);
		cairo_matrix_invert (&matrix);
	} else
		cairo_matrix_init_identity (&matrix);

	if (units == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX &&
	    view->pattern_data->extents.width > 0.0 &&
	    view->pattern_data->extents.height > 0.0) {
		cairo_matrix_scale (&matrix,
				    1.0 / view->pattern_data->extents.width,
				    1.0 / view->pattern_data->extents.height);
		cairo_matrix_translate (&matrix,
					-view->pattern_data->extents.x,
					-view->pattern_data->extents.y);

	}

	cairo_pattern_set_matrix (view->pattern_data->pattern, &matrix);
}

gboolean
lsm_svg_view_create_surface_pattern (LsmSvgView *view,
				     const LsmBox *viewport,
				     const LsmSvgMatrix *pattern_matrix,
				     LsmSvgViewSurfaceType surface_type)
{
	cairo_surface_t *surface;
	cairo_pattern_t *pattern;
	cairo_matrix_t matrix;
	double x1, y1, x2, y2;
	double device_width, device_height;
	double x_scale, y_scale;

	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), FALSE);
	g_return_val_if_fail (viewport != NULL, FALSE);
	g_return_val_if_fail (view->pattern_data != NULL, FALSE);
	g_return_val_if_fail (view->dom_view.cairo == NULL, FALSE);

	x1 = viewport->x;
	y1 = viewport->y;
	x2 = viewport->x + viewport->width;
	y2 = viewport->y;

	cairo_user_to_device (view->pattern_data->old_cairo, &x1, &y1);
	cairo_user_to_device (view->pattern_data->old_cairo, &x2, &y2);

	device_width = sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

	x2 = viewport->x;
	y2 = viewport->y + viewport->height;

	cairo_user_to_device (view->pattern_data->old_cairo, &x2, &y2);

	device_height = sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

	if (surface_type == LSM_SVG_VIEW_SURFACE_TYPE_IMAGE) {
		device_height = ceil (device_height);
		device_width = ceil (device_width);
	}

	x_scale = device_width / viewport->width;
	y_scale = device_height / viewport->height;

	lsm_debug_render ("[LsmSvgView::create_pattern] pattern size = %g ,%g at %g, %g (scale %g x %g)",
		   device_width, device_height, viewport->x, viewport->y, x_scale, y_scale);

	switch (surface_type) {
		case LSM_SVG_VIEW_SURFACE_TYPE_AUTO:
			surface = cairo_surface_create_similar (cairo_get_target (view->pattern_data->old_cairo),
								CAIRO_CONTENT_COLOR_ALPHA,
								device_width, device_height);
			break;
		default:
		case LSM_SVG_VIEW_SURFACE_TYPE_IMAGE:
			surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, device_width, device_height);
			break;
	}

	pattern = cairo_pattern_create_for_surface (surface);
	view->dom_view.cairo = cairo_create (surface);
	cairo_surface_destroy (surface);

	cairo_scale (view->dom_view.cairo, x_scale, y_scale);
	cairo_translate (view->dom_view.cairo, -viewport->x, -viewport->y);

	_set_pattern (view, pattern);

	if (pattern_matrix != NULL) {
		cairo_matrix_init (&matrix,
				   pattern_matrix->a, pattern_matrix->b,
				   pattern_matrix->c, pattern_matrix->d,
				   pattern_matrix->e + viewport->x, pattern_matrix->f + viewport->y);
		cairo_matrix_scale (&matrix, 1.0 / x_scale, 1.0 / y_scale);
		cairo_matrix_invert (&matrix);
	} else {
		cairo_matrix_init_scale (&matrix, x_scale, y_scale);
		cairo_matrix_translate (&matrix, -viewport->x, -viewport->y);
	}

	cairo_pattern_set_matrix (view->pattern_data->pattern, &matrix);
	cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_REPEAT);

	return TRUE;
}

typedef struct {
	cairo_t *cr;
	char *ptr;
	char last_command;
	double last_cp_x;
	double last_cp_y;
	double values[7];
} LsmSvgPathContext;

/*
 * Code for:
 *
 * 	_calc_angle 
 * 	_cairo_elliptical_arc 
 * 	_cairo_quadratic_curve_to
 * 	_emit_smooth_curve
 * 	_emit_smooth_quadratic_curve
 *
 * is adapted from the goocanvas library (goocanvasutils.c)
 *
 * GooCanvas. Copyright (C) 2005 Damon Chaplin.
 */

static void
_cairo_quadratic_curve_to (cairo_t *cr, double x1, double y1, double x, double y)
{
	double x0, y0, xx1, yy1, xx2, yy2;

	cairo_get_current_point (cr, &x0, &y0);

	/* We need to convert the quadratic into a cubic bezier. */

	xx1 = x0 + (x1 - x0) * 2.0 / 3.0;
	yy1 = y0 + (y1 - y0) * 2.0 / 3.0;

	xx2 = xx1 + (x - x0) / 3.0;
	yy2 = yy1 + (y - y0) / 3.0;

	cairo_curve_to (cr, xx1, yy1, xx2, yy2, x, y);
}

static void
_cairo_rel_quadratic_curve_to (cairo_t *cr, double dx1, double dy1, double dx, double dy)
{
	double x0, y0;

	cairo_get_current_point (cr, &x0, &y0);
	_cairo_quadratic_curve_to (cr, x0 + dx1, y0 + dy1, x0 + dx, y0 + dy);
}

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
_emit_function_1 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double))
{
	while (lsm_str_parse_double_list (&ctxt->ptr, 1, ctxt->values) == 1)
		cairo_func (ctxt->cr, ctxt->values[0]);
}

static void
_emit_function_2 (LsmSvgPathContext *ctxt,
		  void (*cairo_func_a) (cairo_t *, double, double),
		  void (*cairo_func_b) (cairo_t *, double, double))
{

	if (lsm_str_parse_double_list (&ctxt->ptr, 2, ctxt->values) == 2) {
		cairo_func_a (ctxt->cr, ctxt->values[0], ctxt->values[1]);

		/* Why oh why does the specification say Line is implied here ? */

		while (lsm_str_parse_double_list (&ctxt->ptr, 2, ctxt->values) == 2)
			cairo_func_b (ctxt->cr, ctxt->values[0], ctxt->values[1]);
	}
}

static void
_emit_function_4 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double, double, double, double))
{
	while (lsm_str_parse_double_list (&ctxt->ptr, 4, ctxt->values) == 4)
		cairo_func (ctxt->cr, ctxt->values[0], ctxt->values[1], ctxt->values[2], ctxt->values[3]);
}

static void
_emit_smooth_curve (LsmSvgPathContext *ctxt, gboolean relative)
{
	double x, y;
	double x0, y0;

	cairo_get_current_point (ctxt->cr, &x0, &y0);

	switch (ctxt->last_command) {
		case 'C':
			x = 2 * x0 - ctxt->values[2];
			y = 2 * y0 - ctxt->values[3];
			break;
		case 'c':
			x = 2 * x0 - (ctxt->values[2] + x0 - ctxt->values[4]);
			y = 2 * y0 - (ctxt->values[3] + y0 - ctxt->values[5]);
			break;
		case 'S':
			x = 2 * x0 - ctxt->values[0];
			y = 2 * y0 - ctxt->values[1];
			break;
		case 's':
			x = 2 * x0 - (ctxt->values[0] + x0 - ctxt->values[2]);
			y = 2 * y0 - (ctxt->values[1] + y0 - ctxt->values[3]);
			break;
		default: x = x0; y = y0; break;
	}

	while (lsm_str_parse_double_list (&ctxt->ptr, 4, ctxt->values) == 4) {
		if (relative) {
			cairo_get_current_point (ctxt->cr, &x0, &y0);
			cairo_curve_to (ctxt->cr,
					x, y,
					x0 + ctxt->values[0], y0 + ctxt->values[1],
					x0 + ctxt->values[2], y0 + ctxt->values[3]);
			x = 2 * (x0 + ctxt->values[2]) - (x0 + ctxt->values[0]);
			y = 2 * (y0 + ctxt->values[3]) - (y0 + ctxt->values[1]);
		} else {
			cairo_curve_to (ctxt->cr, x, y,
					ctxt->values[0], ctxt->values[1], ctxt->values[2], ctxt->values[3]);
			x = 2 * ctxt->values[2] - ctxt->values[0];
			y = 2 * ctxt->values[3] - ctxt->values[1];
		}
	}
}

static void
_emit_smooth_quadratic_curve (LsmSvgPathContext *ctxt, gboolean relative)
{
	double x, y;
	double x0, y0;

	cairo_get_current_point (ctxt->cr, &x0, &y0);

	switch (ctxt->last_command) {
		case 'Q':
			ctxt->last_cp_x = ctxt->values[0];
			ctxt->last_cp_y = ctxt->values[1];
			break;
		case 'q':
			ctxt->last_cp_x = ctxt->values[0] + x0 - ctxt->values[2];
			ctxt->last_cp_y = ctxt->values[1] + y0 - ctxt->values[3];
			break;
		case 'T':
		case 't':
			break;
		default: ctxt->last_cp_x = x0; ctxt->last_cp_y = y0; break;
	}

	while (lsm_str_parse_double_list (&ctxt->ptr, 2, ctxt->values) == 2) {
		x = 2 * x0 - ctxt->last_cp_x;
		y = 2 * y0 - ctxt->last_cp_y;
		if (relative) {
			cairo_get_current_point (ctxt->cr, &x0, &y0);
			_cairo_quadratic_curve_to (ctxt->cr, x, y, x0 + ctxt->values[0], y0 + ctxt->values[1]);
		} else {
			_cairo_quadratic_curve_to (ctxt->cr, x, y, ctxt->values[0], ctxt->values[1]);
		}
		ctxt->last_cp_x = x;
		ctxt->last_cp_y = y;
		cairo_get_current_point (ctxt->cr, &x0, &y0);
	}
}

static void
_emit_function_6 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double, double, double ,double, double, double))
{
	while (lsm_str_parse_double_list (&ctxt->ptr, 6, ctxt->values) == 6)
		cairo_func (ctxt->cr, ctxt->values[0], ctxt->values[1], ctxt->values[2],
			              ctxt->values[3], ctxt->values[4], ctxt->values[5]);
}

static void
_emit_function_7 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double, double, double ,gboolean, gboolean, double, double))
{
	while (lsm_str_parse_double_list (&ctxt->ptr, 7, ctxt->values) == 7)
		cairo_func (ctxt->cr, ctxt->values[0], ctxt->values[1], ctxt->values[2],
			              ctxt->values[3], ctxt->values[4], ctxt->values[5],
				      ctxt->values[6]);
}

static void
_emit_svg_path (cairo_t *cr, char const *path)
{
	LsmSvgPathContext ctxt;

	g_return_if_fail (cr != NULL);

	if (path == NULL)
		return;

	ctxt.cr = cr;
	ctxt.ptr = (char *) path;
	ctxt.last_command = '\0';
	cairo_get_current_point (cr,
				 &ctxt.values[0],
				 &ctxt.values[1]);
	ctxt.values[2] = ctxt.values[4] = ctxt.values[0];
	ctxt.values[3] = ctxt.values[5] = ctxt.values[1];

	lsm_str_skip_spaces (&ctxt.ptr);

	while (*ctxt.ptr != '\0') {
		char command;

		command = *ctxt.ptr;
		ctxt.ptr++;
		lsm_str_skip_spaces (&ctxt.ptr);

		if (!cairo_has_current_point (cr)) {
			cairo_move_to (cr, 0, 0);
			ctxt.values[2] = ctxt.values[4] = ctxt.values[0] = 0;
			ctxt.values[3] = ctxt.values[5] = ctxt.values[1] = 0;
		}

		switch (command) {
			case 'M': _emit_function_2 (&ctxt, cairo_move_to, cairo_line_to); break;
			case 'm': _emit_function_2 (&ctxt, cairo_rel_move_to, cairo_rel_line_to); break;
			case 'L': _emit_function_2 (&ctxt, cairo_line_to, cairo_line_to); break;
			case 'l': _emit_function_2 (&ctxt, cairo_rel_line_to, cairo_rel_line_to); break;
			case 'C': _emit_function_6 (&ctxt, cairo_curve_to); break;
			case 'c': _emit_function_6 (&ctxt, cairo_rel_curve_to); break;
			case 'S': _emit_smooth_curve (&ctxt, FALSE); break;
			case 's': _emit_smooth_curve (&ctxt, TRUE); break;
			case 'V': _emit_function_1 (&ctxt, _cairo_vertical); break;
			case 'v': _emit_function_1 (&ctxt, _cairo_rel_vertical); break;
			case 'H': _emit_function_1 (&ctxt, _cairo_horizontal); break;
			case 'h': _emit_function_1 (&ctxt, _cairo_rel_horizontal); break;
			case 'Q': _emit_function_4 (&ctxt, _cairo_quadratic_curve_to); break;
			case 'q': _emit_function_4 (&ctxt, _cairo_rel_quadratic_curve_to); break;
			case 'T': _emit_smooth_quadratic_curve (&ctxt, FALSE); break;
			case 't': _emit_smooth_quadratic_curve (&ctxt, TRUE); break;
			case 'A': _emit_function_7 (&ctxt, _cairo_elliptical_arc); break;
			case 'a': _emit_function_7 (&ctxt, _cairo_rel_elliptical_arc); break;
			case 'Z':
			case 'z': cairo_close_path (cr); break;
			default: break;
		}

		ctxt.last_command = command;
	}
}

typedef enum {
	LSM_SVG_VIEW_PAINT_OPERATION_FILL,
	LSM_SVG_VIEW_PAINT_OPERATION_STROKE
} LsmSvgViewPaintOperation;

static void
_paint_url (LsmSvgView *view,
	    LsmSvgViewPathInfos *path_infos,
	    LsmSvgViewPaintOperation operation,
	    const char *url, double opacity)
{
	cairo_t *cairo;
	LsmSvgElement *element;
	LsmBox extents;

	element = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document), url);
	if ((!LSM_IS_SVG_RADIAL_GRADIENT_ELEMENT (element) &&
	     !LSM_IS_SVG_LINEAR_GRADIENT_ELEMENT (element) &&
	     !LSM_IS_SVG_PATTERN_ELEMENT (element)) ||
	    lsm_svg_view_circular_reference_check (view, element))
		return;

	lsm_debug_render ("[LsmSvgView::_paint_url] Paint using '%s'", url);

	if (!path_infos->is_extents_defined) {
		cairo_path_extents (view->dom_view.cairo,
				    &path_infos->extents.x1,
				    &path_infos->extents.y1,
				    &path_infos->extents.x2,
				    &path_infos->extents.y2);
		path_infos->is_extents_defined = TRUE;
	}

	extents.x = path_infos->extents.x1;
	extents.y = path_infos->extents.y1;
	extents.width =  path_infos->extents.x2 - extents.x;
	extents.height = path_infos->extents.y2 - extents.y;

	lsm_debug_render ("[LsmSvgView::_paint_url] Pattern extents x = %g, y = %g, w = %g, h = %g",
		   extents.x, extents.y, extents.width, extents.height);

	_start_pattern (view, &extents, opacity);

	lsm_svg_element_force_render (LSM_SVG_ELEMENT (element), view);

	cairo = view->pattern_data->old_cairo;

	if (view->pattern_data->pattern) {
#if 0
		if (LSM_IS_SVG_PATTERN_ELEMENT (element)) {
			char *filename;

			filename = g_strdup_printf ("pattern-%s.png", url);
			cairo_surface_write_to_png (cairo_get_target (view->dom_view.cairo), filename);
			g_free (filename);
		}
#endif

		cairo_set_source (cairo, view->pattern_data->pattern);
	} else
		cairo_set_source_rgb (cairo, 0.0, 0.0, 0.0);

	_end_pattern (view);
}

static gboolean
_set_color (LsmSvgView *view,
	    LsmSvgViewPathInfos *path_infos,
	    LsmSvgViewPaintOperation operation,
	    const LsmSvgPaint *paint, double opacity)
{
	cairo_t *cairo = view->dom_view.cairo;

	switch (paint->type) {
		case LSM_SVG_PAINT_TYPE_NONE:
			return FALSE;
		case LSM_SVG_PAINT_TYPE_RGB_COLOR:
			cairo_set_source_rgba (cairo,
					       paint->color.red,
					       paint->color.green,
					       paint->color.blue,
					       opacity);
			break;
		case LSM_SVG_PAINT_TYPE_CURRENT_COLOR:
			cairo_set_source_rgba (cairo,
					       view->style->color->value.red,
					       view->style->color->value.green,
					       view->style->color->value.blue,
					       opacity);
			break;
		case LSM_SVG_PAINT_TYPE_URI:
		case LSM_SVG_PAINT_TYPE_URI_RGB_COLOR:
		case LSM_SVG_PAINT_TYPE_URI_CURRENT_COLOR:
		case LSM_SVG_PAINT_TYPE_URI_NONE:
			_paint_url (view, path_infos, operation, paint->url, opacity);
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

static void
paint_markers (LsmSvgView *view)
{
	const LsmSvgStyle *style;
	LsmSvgElement *marker;
	LsmSvgElement *marker_start;
	LsmSvgElement *marker_mid;
	LsmSvgElement *marker_end;
	cairo_t *cairo;
	cairo_path_t *path;
	cairo_path_data_t *data;
	cairo_path_data_t *next_data;
	double stroke_width;
	double prev_x, prev_y;
	double x = 0;
	double y = 0;
	double next_x, next_y;
	cairo_path_data_type_t type;
	cairo_path_data_type_t next_type;
	double angle;
	int i;

	style = view->style;

	if ((style->marker->value == NULL ||       strcmp (style->marker->value, "none") == 0) &&
	    (style->marker_mid->value == NULL ||   strcmp (style->marker_mid->value, "none") == 0) &&
	    (style->marker_end->value == NULL ||   strcmp (style->marker_end->value, "none") == 0) &&
	    (style->marker_start->value == NULL || strcmp (style->marker_start->value, "none") == 0))
		return;

	cairo = view->dom_view.cairo;

	marker = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
						      style->marker->value);
	marker_start = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
							    style->marker_start->value);
	marker_mid = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
							  style->marker_mid->value);
	marker_end = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
							  style->marker_end->value);
	stroke_width = lsm_svg_view_normalize_length (view, &view->style->stroke_width->length,
						      LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	if (marker != NULL && lsm_svg_view_circular_reference_check (view, marker))
		return;
	if (marker_start != NULL && lsm_svg_view_circular_reference_check (view, marker_start))
		return;
	if (marker_mid != NULL && lsm_svg_view_circular_reference_check (view, marker_mid))
		return;
	if (marker_end != NULL && lsm_svg_view_circular_reference_check (view, marker_end))
		return;

	if (marker_start == NULL)
		marker_start = marker;
	if (marker_mid == NULL)
		marker_mid = marker;
	if (marker_end == NULL)
		marker_end = marker;

	path = cairo_copy_path (cairo);
	cairo_new_path (cairo);

	if (path->num_data > 0) {
		next_data = &path->data[0];
		next_type = next_data->header.type;

		if (next_type == CAIRO_PATH_CURVE_TO) {
			next_x = next_data[3].point.x;
			next_y = next_data[3].point.y;
		} else {
			next_x = next_data[1].point.x;
			next_y = next_data[1].point.y;
		}

		for (i = 0; i < path->num_data; i += path->data[i].header.length) {
			data = next_data;

			prev_x = x;
			prev_y = y;
			x = next_x;
			y = next_y;
			type = next_type;

			if (i + path->data[i].header.length < path->num_data) {
				next_data = &path->data[i + path->data[i].header.length];
				next_type = next_data->header.type;

				if (next_type == CAIRO_PATH_CURVE_TO) {
					next_x = next_data[3].point.x;
					next_y = next_data[3].point.y;
				} else {
					next_x = next_data[1].point.x;
					next_y = next_data[1].point.y;
				}
			} else {
				next_data = NULL;
				next_type = CAIRO_PATH_MOVE_TO;
				next_x = 0.0;
				next_y = 0.0;
			}

			if (data->header.type == CAIRO_PATH_CLOSE_PATH) {
				marker = NULL;
			} else if (next_data == NULL ||
				   next_data->header.type == CAIRO_PATH_MOVE_TO) {
				marker = marker_end;
				if (type == CAIRO_PATH_CURVE_TO)
					angle = atan2 (y - data[2].point.y,
						       x - data[2].point.x);
				else
					angle = atan2 (y - prev_y, x - prev_x);
			} else if (data->header.type == CAIRO_PATH_MOVE_TO) {
				marker = marker_start;
				if (next_type == CAIRO_PATH_CURVE_TO)
					angle = atan2 (data[1].point.y - y,
						       data[1].point.x - x);
				else
					angle = atan2 (next_y - y, next_x - x);
			} else {
				double xdifin, ydifin, xdifout, ydifout, intot, outtot;

				marker = marker_mid;

				if (type == CAIRO_PATH_CURVE_TO) {
					xdifin = x - data[2].point.x;
					ydifin = y - data[2].point.y;
				} else {
					xdifin = x - prev_x;
					ydifin = y - prev_y;
				}
				if (next_type == CAIRO_PATH_CURVE_TO) {
					xdifout = data[3].point.x - x;
					ydifout = data[3].point.y - y;
				} else {
					xdifout = next_x - x;
					ydifout = next_y - y;
				}

				intot = sqrt (xdifin * xdifin + ydifin * ydifin);
				outtot = sqrt (xdifout * xdifout + ydifout * ydifout);

				xdifin /= intot;
				ydifin /= intot;
				xdifout /= outtot;
				ydifout /= outtot;

				angle = atan2 ((ydifin + ydifout) / 2, (xdifin + xdifout) / 2);
			}

			if (marker != NULL) {
				cairo_save (cairo);
				cairo_translate (cairo, x, y);
				lsm_svg_marker_element_render (LSM_SVG_MARKER_ELEMENT (marker), view,
							       stroke_width, angle);
				cairo_restore (cairo);
			}
		}

		cairo_path_destroy (path);
	}
}

static void
paint (LsmSvgView *view, LsmSvgViewPathInfos *path_infos)
{
	const LsmSvgStyle *style;
	LsmSvgElement *element;
	cairo_t *cairo;
	gboolean use_group;
	double group_opacity;

	element = view->element_stack->data;

	g_return_if_fail (LSM_IS_SVG_ELEMENT (element));

	cairo = view->dom_view.cairo;
	style = view->style;

	if (style->opacity != NULL) {
		group_opacity = style->opacity->value;

		use_group = style->fill->paint.type != LSM_SVG_PAINT_TYPE_NONE &&
			style->stroke->paint.type != LSM_SVG_PAINT_TYPE_NONE &&
			group_opacity < 1.0;
	} else {
		use_group = FALSE;
		group_opacity = 1.0;
	}

	/* FIXME Instead of push_group, restrict to the current path bounding box */
	if (use_group)
		cairo_push_group (cairo);

	if (_set_color (view,
			path_infos,
			LSM_SVG_VIEW_PAINT_OPERATION_FILL,
			&style->fill->paint,
			style->fill_opacity->value * (use_group ? 1.0 : group_opacity))) {

		if (path_infos->is_text_path) {
			pango_cairo_show_layout (cairo, path_infos->pango_layout);
		} else {
			cairo_set_fill_rule (cairo, style->fill_rule->value == LSM_SVG_FILL_RULE_EVEN_ODD ?
					     CAIRO_FILL_RULE_EVEN_ODD : CAIRO_FILL_RULE_WINDING);
			cairo_fill_preserve (cairo);
		}
	}

	if (_set_color (view,
			path_infos,
			LSM_SVG_VIEW_PAINT_OPERATION_STROKE,
			&style->stroke->paint,
			style->stroke_opacity->value * (use_group ? 1.0 : group_opacity))) {
		double line_width;

		if (path_infos->is_text_path) {
			pango_cairo_layout_path (cairo, path_infos->pango_layout);
		}

		switch (style->stroke_line_join->value) {
			case LSM_SVG_LINE_JOIN_MITER:
				cairo_set_line_join (cairo, CAIRO_LINE_JOIN_MITER);
				break;
			case LSM_SVG_LINE_JOIN_ROUND:
				cairo_set_line_join (cairo,CAIRO_LINE_JOIN_ROUND);
				break;
			default:
				cairo_set_line_join (cairo,CAIRO_LINE_JOIN_BEVEL);
		}

		switch (style->stroke_line_cap->value) {
			case LSM_SVG_LINE_CAP_BUTT:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_BUTT);
				break;
			case LSM_SVG_LINE_CAP_ROUND:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_ROUND);
				break;
			default:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_SQUARE);
		}

		line_width = lsm_svg_view_normalize_length (view, &style->stroke_width->length,
							    LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

		cairo_set_miter_limit (cairo, style->stroke_miter_limit->value);
		cairo_set_line_width (cairo, line_width);

		if (style->stroke_dash_array->value.n_dashes > 0) {
			double dash_offset;
			double *dashes;
			unsigned int i;

			dash_offset = lsm_svg_view_normalize_length (view, &style->stroke_dash_offset->length,
								     LSM_SVG_LENGTH_DIRECTION_DIAGONAL);
			dashes = g_new (double, style->stroke_dash_array->value.n_dashes);
			for (i = 0; i < style->stroke_dash_array->value.n_dashes; i++)
				dashes[i] = lsm_svg_view_normalize_length (view,
									   &style->stroke_dash_array->value.dashes[i],
									   LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

			cairo_set_dash (cairo, dashes, style->stroke_dash_array->value.n_dashes, dash_offset);
			g_free (dashes);
		} else
			cairo_set_dash (cairo, NULL, 0, 0.0);

		cairo_stroke_preserve (cairo);
	}

	paint_markers (view);

	cairo_new_path (cairo);

	if (use_group) {
		cairo_pop_group_to_source (cairo);
		cairo_paint_with_alpha (cairo, group_opacity);
	}
}

static void
process_path (LsmSvgView *view, LsmSvgViewPathInfos *path_infos)
{
	g_return_if_fail (view->style != NULL);

	if (view->is_clipping) {
		if (path_infos->is_text_path)
			pango_cairo_layout_path (view->dom_view.cairo, path_infos->pango_layout);
		cairo_set_fill_rule (view->dom_view.cairo, view->style->clip_rule->value);
	} else
		paint (view, path_infos);
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
lsm_svg_view_show_rectangle (LsmSvgView *view,
			     double x, double y,
			     double w, double h,
			     double rx, double ry)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;
	cairo_t *cairo;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

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

	process_path (view, &path_infos);
}

void
lsm_svg_view_show_circle (LsmSvgView *view, double cx, double cy, double r)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_arc (view->dom_view.cairo, cx, cy, r, 0, 2 * M_PI);

	process_path (view, &path_infos);
}

#define LSM_SVG_ARC_MAGIC ((double) 0.5522847498) /* 4/3 * (1-cos 45)/sin 45 = 4/3 * sqrt(2) - 1 */

void
lsm_svg_view_show_ellipse (LsmSvgView *view, double cx, double cy, double rx, double ry)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;
	cairo_t *cairo;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	if (rx <= 0 || ry <= 0)
		return;

	cairo = view->dom_view.cairo;

	cairo_move_to (cairo, cx + rx, cy);
	cairo_curve_to (cairo, cx + rx, cy + LSM_SVG_ARC_MAGIC * ry, cx + LSM_SVG_ARC_MAGIC * rx, cy + ry, cx, cy + ry);
	cairo_curve_to (cairo, cx - LSM_SVG_ARC_MAGIC * rx, cy + ry, cx - rx, cy + LSM_SVG_ARC_MAGIC * ry, cx - rx, cy);
	cairo_curve_to (cairo, cx - rx, cy - LSM_SVG_ARC_MAGIC * ry, cx - LSM_SVG_ARC_MAGIC * rx, cy - ry, cx, cy - ry);
	cairo_curve_to (cairo, cx + LSM_SVG_ARC_MAGIC * rx, cy - ry, cx + rx, cy - LSM_SVG_ARC_MAGIC * ry, cx + rx, cy);
	cairo_close_path (cairo);

	process_path (view, &path_infos);
}

void
lsm_svg_view_show_path (LsmSvgView *view,
		     const char *d)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	_emit_svg_path (view->dom_view.cairo, d);

	process_path (view, &path_infos);
}

void
lsm_svg_view_calculate_path_extents (LsmSvgView *view,
				     const char *path,
				     double *x1, double *y1,
				     double *x2, double *y2)
{
	double xx1, yy1, xx2, yy2;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_new_path (view->dom_view.cairo);
	_emit_svg_path (view->dom_view.cairo, path);
	cairo_path_extents (view->dom_view.cairo, &xx1, &yy1, &xx2, &yy2);
	cairo_new_path (view->dom_view.cairo);

	if (x1 != NULL)
		*x1 = xx1;
	if (y1 != NULL)
		*y1 = yy1;
	if (x2 != NULL)
		*x2 = xx2;
	if (y2 != NULL)
		*y2 = yy2;
}

void
lsm_svg_view_show_line (LsmSvgView *view, double x1, double y1, double x2, double y2)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_move_to (view->dom_view.cairo, x1, y1);
	cairo_line_to (view->dom_view.cairo, x2, y2);

	process_path (view, &path_infos);
}

static void
_show_points (LsmSvgView *view, const char *points, gboolean close_path)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;
	char *str;
	double values[2];
	unsigned int n_values;
	unsigned int count = 0;

	if (points == NULL)
		return;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	str = (char *) points;

	do {
		n_values = lsm_str_parse_double_list (&str, 2, values);
		if (n_values == 2) {
			if (count == 0)
				cairo_move_to (view->dom_view.cairo, values[0], values[1]);
			else
				cairo_line_to (view->dom_view.cairo, values[0], values[1]);
		} else if (n_values != 0) {
			cairo_new_path (view->dom_view.cairo);
			return;
		}
		count++;
	} while (n_values == 2);

	if (close_path)
		cairo_close_path (view->dom_view.cairo);

	process_path (view, &path_infos);
}

void
lsm_svg_view_show_polyline (LsmSvgView *view, const char *points)
{
	_show_points (view, points, FALSE);
}

void
lsm_svg_view_show_polygon (LsmSvgView *view, const char *points)
{
	_show_points (view, points, TRUE);
}

void
lsm_svg_view_show_text (LsmSvgView *view, char const *string, double x, double y)
{
	LsmSvgViewPathInfos path_infos = default_path_infos;
	const LsmSvgStyle *style;
	PangoLayout *pango_layout;
	PangoFontDescription *font_description;
	PangoStretch font_stretch;
	PangoStyle font_style;
	PangoLayoutIter *iter;
	PangoRectangle rectangle;
	int font_size;
	int baseline;
	double x1, y1;

	if (string == NULL)
		return;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	lsm_debug_render ("[LsmSvgView::show_text] Show '%s' at %g,%g", string, x, y);

	style = view->style;

	if (view->is_pango_layout_in_use) {
		PangoContext *pango_context;

		pango_context = pango_layout_get_context (view->pango_layout);
		view->pango_layout_stack = g_slist_prepend (view->pango_layout_stack, view->pango_layout);
		view->pango_layout = pango_layout_new (pango_context);

		lsm_debug_render ("[LsmSvgView::show_text] Create a new pango layout");
	} else
		view->is_pango_layout_in_use = TRUE;

	pango_layout = view->pango_layout;
	font_description = view->dom_view.font_description;

	font_size = PANGO_SCALE * lsm_svg_view_normalize_length (view, &style->font_size->length,
								 LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	if (font_size < 0)
		font_size = 0;

	pango_font_description_set_family (font_description, style->font_family->value);
	pango_font_description_set_size (font_description, font_size);
	pango_font_description_set_weight (font_description, style->font_weight->value);

	switch (style->font_stretch->value) {
		case LSM_SVG_FONT_STRETCH_ULTRA_CONDENSED:
			font_stretch = PANGO_STRETCH_ULTRA_CONDENSED;
			break;
		case LSM_SVG_FONT_STRETCH_EXTRA_CONDENSED:
			font_stretch = PANGO_STRETCH_EXTRA_CONDENSED;
			break;
		case LSM_SVG_FONT_STRETCH_CONDENSED:
			font_stretch = PANGO_STRETCH_CONDENSED;
			break;
		case LSM_SVG_FONT_STRETCH_SEMI_CONDENSED:
			font_stretch = PANGO_STRETCH_SEMI_CONDENSED;
			break;
		case LSM_SVG_FONT_STRETCH_SEMI_EXPANDED:
			font_stretch = PANGO_STRETCH_SEMI_EXPANDED;
			break;
		case LSM_SVG_FONT_STRETCH_EXPANDED:
			font_stretch = PANGO_STRETCH_EXPANDED;
			break;
		case LSM_SVG_FONT_STRETCH_EXTRA_EXPANDED:
			font_stretch = PANGO_STRETCH_EXTRA_EXPANDED;
			break;
		case LSM_SVG_FONT_STRETCH_ULTRA_EXPANDED:
			font_stretch = PANGO_STRETCH_ULTRA_EXPANDED;
			break;
		case LSM_SVG_FONT_STRETCH_NORMAL:
		default:
			font_stretch = PANGO_STRETCH_NORMAL;
			break;
	}
	pango_font_description_set_stretch (font_description, font_stretch);

	switch (style->font_style->value) {
		case LSM_SVG_FONT_STYLE_OBLIQUE:
			font_style = PANGO_STYLE_OBLIQUE;
			break;
		case LSM_SVG_FONT_STYLE_ITALIC:
			font_style = PANGO_STYLE_ITALIC;
			break;
		case LSM_SVG_FONT_STYLE_NORMAL:
		default:
			font_style = PANGO_STYLE_NORMAL;
			break;
	}
	pango_font_description_set_style (font_description, font_style);

	pango_layout_set_text (pango_layout, string, -1);
	pango_layout_set_font_description (pango_layout, font_description);
	pango_layout_get_extents (pango_layout, &rectangle, NULL);

	iter = pango_layout_get_iter (pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	x1 = x - pango_units_to_double (rectangle.x);
	y1 = y - pango_units_to_double (baseline);

	switch (style->text_anchor->value) {
		case LSM_SVG_TEXT_ANCHOR_END:
			x1 -= pango_units_to_double (rectangle.width);
			break;
		case LSM_SVG_TEXT_ANCHOR_MIDDLE:
			x1 -= pango_units_to_double (rectangle.width) / 2.0;
			break;
		case LSM_SVG_TEXT_ANCHOR_START:
		default:
			break;
	}

	cairo_move_to (view->dom_view.cairo, x1, y1);

	path_infos.is_text_path = TRUE;
	path_infos.is_extents_defined = TRUE;
	path_infos.extents.x1 = x1;
	path_infos.extents.y1 = y1;
	path_infos.extents.x2 = x1 + pango_units_to_double (rectangle.width);
	path_infos.extents.y2 = y1 + pango_units_to_double (rectangle.height);
	path_infos.pango_layout = pango_layout;

	process_path (view, &path_infos);

	if (pango_layout != view->pango_layout) {
		lsm_debug_render ("[LsmSvgView::show_text] Free the child pango layout");

		if (view->pango_layout != NULL) {
			g_object_unref (view->pango_layout);

			view->pango_layout = view->pango_layout_stack->data;
			view->pango_layout_stack = g_slist_delete_link (view->pango_layout_stack,
									view->pango_layout_stack);
		} else
			g_warning ("[LsmSvgView::show_text] Pango layout stack empty");
	}

	view->is_pango_layout_in_use = FALSE;
}

void
lsm_svg_view_show_pixbuf (LsmSvgView *view, GdkPixbuf *pixbuf)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (GDK_IS_PIXBUF (pixbuf));

	lsm_cairo_set_source_pixbuf (view->dom_view.cairo, pixbuf, 0, 0);
	cairo_paint (view->dom_view.cairo);
}

void
lsm_svg_view_push_viewbox (LsmSvgView *view, const LsmBox *viewbox)
{
	LsmSvgViewbox *svg_viewbox;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	lsm_debug_render ("[LsmSvgView::push_viewbox] viewbox = %g, %g, %g, %g",
		   viewbox->x, viewbox->y, viewbox->width, viewbox->height);

	svg_viewbox = lsm_svg_viewbox_new (view->resolution_ppi, viewbox);

	view->viewbox_stack = g_slist_prepend (view->viewbox_stack, svg_viewbox);
}

void
lsm_svg_view_pop_viewbox (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->viewbox_stack != NULL);

	lsm_debug_render ("[LsmSvgView::pop_viewbox]");

	lsm_svg_viewbox_free (view->viewbox_stack->data);
	view->viewbox_stack = g_slist_delete_link (view->viewbox_stack, view->viewbox_stack);
}

static const LsmBox *
_compute_viewbox_scale (const LsmBox *viewport, const LsmBox *viewbox,
			const LsmSvgPreserveAspectRatio *aspect_ratio,
			double *x_offset, double *y_offset,
			double *x_scale, double *y_scale)
{
	if (viewbox != NULL) {
		double x, y;
		double x_ratio;
		double y_ratio;

		x_ratio = viewbox->width  > 0.0 ? viewport->width  / viewbox->width  : 0.0;
		y_ratio = viewbox->height > 0.0 ? viewport->height / viewbox->height : 0.0;

		if (aspect_ratio != NULL && aspect_ratio->align > LSM_SVG_ALIGN_NONE) {
			if (aspect_ratio->meet_or_slice == LSM_SVG_MEET_OR_SLICE_MEET) {
				*x_scale = MIN (x_ratio, y_ratio);
				*y_scale = *x_scale;
			} else {
				*x_scale = MAX (x_ratio, y_ratio);
				*y_scale = *x_scale;
			}

			x = -viewbox->x * *x_scale;
			y = -viewbox->y * *y_scale;

			switch (aspect_ratio->align) {
				case LSM_SVG_ALIGN_X_MIN_Y_MIN:
					break;
				case LSM_SVG_ALIGN_X_MIN_Y_MID:
					y += (viewport->height- viewbox->height * *y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MIN_Y_MAX:
					y += (viewport->height - viewbox->height * *y_scale);
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MIN:
					x += (viewport->width - viewbox->width * *x_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MID:
					x += (viewport->width - viewbox->width * *x_scale) * 0.5;
					y += (viewport->height- viewbox->height * *y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MAX:
					x += (viewport->width - viewbox->width * *x_scale) * 0.5;
					y += (viewport->height - viewbox->height * *y_scale);
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MIN:
					x += (viewport->width - viewbox->width * *x_scale);
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MID:
					x += (viewport->width - viewbox->width * *x_scale);
					y += (viewport->height- viewbox->height * *y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MAX:
					x += (viewport->width - viewbox->width * *x_scale);
					y += (viewport->height - viewbox->height * *y_scale);
					break;
				default:
					break;
			}

			*x_offset = x;
			*y_offset = y;

		} else {
			*x_scale = x_ratio;
			*y_scale = y_ratio;

			*x_offset = -viewbox->x * *x_scale;
			*y_offset = -viewbox->y * *y_scale;
		}

		lsm_debug_render ("[LsmSvgView::_compute_viewbox_scale] scale = %g, %g", *x_scale, *y_scale);

		return viewbox;
	}

	*x_scale = *y_scale = 1.0;
	*x_offset = 0.0;
	*y_offset = 0.0;

	return viewport;
}

void
lsm_svg_view_push_viewport (LsmSvgView *view, const LsmBox *viewport, const LsmBox *viewbox,
			    const LsmSvgPreserveAspectRatio *aspect_ratio)
{
	cairo_t *cairo;
	const LsmBox *actual_viewbox;
	double x_offset, y_offset;
	double x_scale, y_scale;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (viewport != NULL);

	actual_viewbox = _compute_viewbox_scale (viewport, viewbox, aspect_ratio,
						 &x_offset, &y_offset, &x_scale, &y_scale);
	lsm_svg_view_push_viewbox (view, actual_viewbox);

	cairo = view->dom_view.cairo;

	cairo_save (cairo);
#if 0
	if (view->dom_view.debug) {
		cairo_save (cairo);
		cairo_set_line_width (cairo, 1.0);
		cairo_set_source_rgb (cairo, 0.0, 0.0, 0.0);
		cairo_rectangle (cairo, viewport->x, viewport->y, viewport->width, viewport->height);
		cairo_stroke (cairo);
		cairo_restore (cairo);
	}
#endif
	cairo_rectangle (cairo, viewport->x, viewport->y, viewport->width, viewport->height);
	cairo_clip (cairo);
	cairo_translate (cairo, viewport->x + x_offset, viewport->y + y_offset);
	cairo_scale (cairo, x_scale, y_scale);
}

void
lsm_svg_view_pop_viewport (LsmSvgView *view)
{
	cairo_restore (view->dom_view.cairo);

	lsm_svg_view_pop_viewbox (view);
}

void
lsm_svg_view_viewbox_to_viewport (LsmSvgView *view, const LsmBox *viewport, const LsmBox *viewbox,
				  const LsmSvgPreserveAspectRatio *aspect_ratio,
				  double *x, double *y)
{
	double x_offset, y_offset;
	double x_scale, y_scale;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (viewport != NULL);

	_compute_viewbox_scale (viewport, viewbox, aspect_ratio,
				&x_offset, &y_offset, &x_scale, &y_scale);

	if (x != NULL)
		*x = *x * x_scale + x_offset;

	if (y != NULL)
		*y = *y * y_scale + y_offset;
}

void
lsm_svg_view_push_matrix (LsmSvgView *view, const LsmSvgMatrix *matrix)
{
	cairo_matrix_t cr_matrix;
	cairo_matrix_t *ctm;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	ctm = g_new (cairo_matrix_t, 1);
	cairo_get_matrix (view->dom_view.cairo, ctm);

	view->matrix_stack = g_slist_prepend (view->matrix_stack, ctm);

	lsm_debug_render ("[LsmSvgView::push_matrix] New transform %g, %g, %g, %g, %g, %g",
		   matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);

	cairo_matrix_init (&cr_matrix, matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
	cairo_transform (view->dom_view.cairo, &cr_matrix);

	{
		cairo_matrix_t current_ctm;
		cairo_get_matrix (view->dom_view.cairo, &current_ctm);

		lsm_debug_render ("[LsmSvgView::push_matrix] Current ctm %g, %g, %g, %g, %g, %g",
			   current_ctm.xx, current_ctm.xy, current_ctm.yx, current_ctm.yy,
			   current_ctm.x0, current_ctm.y0);
	}
}

void
lsm_svg_view_pop_matrix (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	if (view->matrix_stack != NULL) {
		cairo_matrix_t *ctm;

		ctm = view->matrix_stack->data;

		cairo_set_matrix (view->dom_view.cairo, ctm);

		lsm_debug_render ("[LsmSvgView::pop_matrix] Restore ctm %g, %g, %g, %g, %g, %g",
			   ctm->xx, ctm->xy, ctm->yx, ctm->yy,
			   ctm->x0, ctm->y0);

		g_free (ctm);
		view->matrix_stack = g_slist_delete_link (view->matrix_stack, view->matrix_stack);
	}
}

void
lsm_svg_view_push_group_opacity (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->style != NULL);

	if (view->style->opacity->value < 1.0)
		cairo_push_group (view->dom_view.cairo);
}

void
lsm_svg_view_pop_group_opacity (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->style != NULL);

	if (view->style->opacity->value < 1.0) {
		cairo_pop_group_to_source (view->dom_view.cairo);
		cairo_paint_with_alpha (view->dom_view.cairo, view->style->opacity->value);
	}
}

static void
lsm_svg_view_push_clip (LsmSvgView *view)
{
	LsmSvgElement *element;
	LsmExtents extents;
	char *url;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (!view->is_clipping);

	lsm_svg_element_get_extents (view->element_stack->data, view, &extents);

	url = view->style->clip_path->value;

	lsm_debug_render ("[LsmSvgView::push_clip] Using '%s'", url);

	cairo_save (view->dom_view.cairo);

	view->clip_extents.x = extents.x1;
	view->clip_extents.y = extents.y1;
	view->clip_extents.width  = extents.x2 - extents.x1;
	view->clip_extents.height = extents.y2 - extents.y1;

	element = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document), url);
	if (LSM_IS_SVG_CLIP_PATH_ELEMENT (element) &&
	    !lsm_svg_view_circular_reference_check (view, element)) {
		view->is_clipping = TRUE;
		lsm_svg_element_force_render (LSM_SVG_ELEMENT (element), view);
		cairo_clip (view->dom_view.cairo);
		view->is_clipping = FALSE;
	}
}

static void
lsm_svg_view_pop_clip (LsmSvgView *view)
{
	lsm_debug_render ("[LsmSvgView::pop_clip");

	cairo_restore (view->dom_view.cairo);
}

static void
lsm_svg_view_push_mask (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_push_group (view->dom_view.cairo);
}

static void
lsm_svg_view_pop_mask (LsmSvgView *view)
{
	LsmSvgElement *mask_element;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	mask_element = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
							    view->style->mask->value);

	if (LSM_IS_SVG_MASK_ELEMENT (mask_element) &&
	    !lsm_svg_view_circular_reference_check (view, mask_element)) {
		LsmExtents extents;
		LsmBox mask_extents;
		cairo_t *cairo;

		lsm_svg_element_get_extents (view->element_stack->data, view, &extents);

		mask_extents.x = extents.x1;
		mask_extents.y = extents.y1;
		mask_extents.width = extents.x2 - extents.x1;
		mask_extents.height = extents.y2 - extents.y1;

		cairo = view->dom_view.cairo;

		_start_pattern (view, &mask_extents, 1.0);

		lsm_svg_element_force_render (LSM_SVG_ELEMENT (mask_element), view);

		cairo_pop_group_to_source (cairo);
		if (view->pattern_data->pattern != NULL) {
			cairo_surface_t *surface;
			int width, height, row, i, stride;
			unsigned char *pixels;

			cairo_pattern_get_surface (view->pattern_data->pattern, &surface);
			pixels = cairo_image_surface_get_data (surface);
			height = cairo_image_surface_get_height (surface);
			width = cairo_image_surface_get_width (surface);
			stride = cairo_image_surface_get_stride (surface);

			for (row = 0; row < height; row++) {
				guint8 *row_data = (pixels + (row * stride));
				for (i = 0; i < width; i++) {
					guint32 *pixel = (guint32 *) row_data + i;
					*pixel = ((((*pixel & 0x00ff0000) >> 16) * 13817 +
						   ((*pixel & 0x0000ff00) >> 8) * 46518 +
						   ((*pixel & 0x000000ff)) * 4688) * 0xff
						  /* * opacity */);
				}
			}

			cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_NONE);
#if 0
			{
				char *filename;

				filename = g_strdup_printf ("mask-%s.png", view->style->mask->value);
				cairo_surface_write_to_png (cairo_get_target (view->dom_view.cairo), filename);
				g_free (filename);
			}
#endif
			cairo_mask (cairo, view->pattern_data->pattern);
		} else {
			cairo_paint (cairo);
		}

		_end_pattern (view);
	} else {
		cairo_pop_group_to_source (view->dom_view.cairo);
		cairo_paint (view->dom_view.cairo);
	}
}

void
lsm_svg_view_push_filter (LsmSvgView *view)
{
#if 0
	LsmExtents extents;
	LsmBox source_extents;
	gboolean result;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->element_stack != NULL);

	lsm_svg_element_get_extents (view->element_stack->data, view, &extents);

	source_extents.x = extents.x1;
	source_extents.y = extents.y1;
	source_extents.width = extents.x2 - extents.x1;
	source_extents.height = extents.y2 - extents.y1;

	_start_pattern (view, &source_extents, 1.0);

	result = lsm_svg_view_create_surface_pattern (view,
						      &source_extents,
						      NULL,
						      LSM_SVG_VIEW_SURFACE_TYPE_IMAGE);
#endif
}

void
lsm_svg_view_pop_filter (LsmSvgView *view)
{
#if 0
	LsmSvgElement *filter_element;
	LsmFilterSurface *filter_surface;
	cairo_surface_t *surface;
	cairo_t *cairo;
	GSList *iter;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	filter_element = lsm_svg_document_get_element_by_url (LSM_SVG_DOCUMENT (view->dom_view.document),
							      view->style->filter->value);

TODO: lsm_svg_view_circular_reference_check

	cairo = view->pattern_data->old_cairo;

	view->filter_surfaces = NULL;

	cairo_pattern_get_surface (view->pattern_data->pattern, &surface);
	filter_surface = lsm_filter_surface_new_with_content ("SourceGraphic", 0, 0, surface);

	view->filter_surfaces = g_slist_prepend (view->filter_surfaces, filter_surface);

#if 1
	{
		char *filename;
		static int count = 0;

		filename = g_strdup_printf ("filter-%04d-%s.png", count++, view->style->filter->value);
		cairo_surface_write_to_png (cairo_get_target (view->dom_view.cairo), filename);
		g_free (filename);
	}
#endif

#if 1
	cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_NONE);
	cairo_set_source (cairo, view->pattern_data->pattern);
	cairo_paint (cairo);
#endif

	for (iter = view->filter_surfaces; iter != NULL; iter = iter->next)
		lsm_filter_surface_free (iter->data);
	g_slist_free (view->filter_surfaces);
	view->filter_surfaces = NULL;

	_end_pattern (view);
#endif
}

void
lsm_svg_view_apply_gaussian_blur (LsmSvgView *view, const char *input, const char *output,
				  double x, double y, double w, double h,
				  double std_x, double std_y)
{
}

void
lsm_svg_view_push_element (LsmSvgView *view, const LsmSvgElement *element)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (LSM_IS_SVG_ELEMENT (element));

	view->element_stack = g_slist_prepend (view->element_stack, (void *) element);
}

void
lsm_svg_view_pop_element (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->element_stack != NULL);

	view->element_stack = g_slist_delete_link (view->element_stack, view->element_stack);
}

static gboolean
lsm_svg_view_circular_reference_check (LsmSvgView *view, LsmSvgElement *element)
{
	GSList *iter;

	for (iter = view->element_stack; iter != NULL; iter = iter->next)
		if (iter->data == element) {
			lsm_debug_render ("[LsmSvgView::circular_reference_check] "
				   "Circular reference to %s (id = %s)",
				   lsm_dom_element_get_tag_name (LSM_DOM_ELEMENT (element)),
				   lsm_dom_element_get_attribute (LSM_DOM_ELEMENT (element), "id"));
			return TRUE;
		}

	return FALSE;
}

void
lsm_svg_view_push_style	(LsmSvgView *view, const LsmSvgStyle *style)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (style != NULL);

	view->style_stack = g_slist_prepend (view->style_stack, (void *) style);
	view->style = style;

	if (g_strcmp0 (style->filter->value, "none") != 0) {
		lsm_debug_render ("[LsmSvgView::push_style] Start filter '%s'", style->filter->value);
		lsm_svg_view_push_filter (view);
	}

	if (g_strcmp0 (style->clip_path->value, "none") != 0) {
		lsm_debug_render ("[LsmSvgView::push_style] Start clip '%s'", style->clip_path->value);
		lsm_svg_view_push_clip (view);
	}

	if (g_strcmp0 (style->mask->value, "none") != 0) {
		lsm_debug_render ("[LsmSvgView::push_style] Start mask '%s'", style->mask->value);
		lsm_svg_view_push_mask (view);
	}
}

void lsm_svg_view_pop_style (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->style_stack != NULL);

	if (g_strcmp0 (view->style->mask->value, "none") != 0)
		lsm_svg_view_pop_mask (view);

	if (g_strcmp0 (view->style->clip_path->value, "none") != 0)
		lsm_svg_view_pop_clip (view);

	if (g_strcmp0 (view->style->filter->value, "none") != 0) {
		lsm_svg_view_pop_filter (view);
	}

	view->style_stack = g_slist_delete_link (view->style_stack, view->style_stack);
	view->style = view->style_stack != NULL ? view->style_stack->data : NULL;
}

LsmSvgStyle *
lsm_svg_view_get_current_style (LsmSvgView *view)
{
	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), NULL);

	return (LsmSvgStyle *) view->style;
}

const LsmBox *
lsm_svg_view_get_pattern_extents (LsmSvgView *view)
{
	static LsmBox null_extents = {.x = 0.0, .y = 0.0, .width = 0.0, .height = 0.0};

	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), &null_extents);
	g_return_val_if_fail (view->pattern_data != NULL, &null_extents);

	return &view->pattern_data->extents;
}

const LsmBox *
lsm_svg_view_get_clip_extents (LsmSvgView *view)
{
	static LsmBox null_extents = {.x = 0.0, .y = 0.0, .width = 0.0, .height = 0.0};

	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), &null_extents);
	g_return_val_if_fail (view->is_clipping, &null_extents);

	return &view->clip_extents;
}

static void
lsm_svg_view_measure (LsmDomView *view, double *width, double *height, double *baseline)
{
	LsmSvgSvgElement *svg_element;

	svg_element = lsm_svg_document_get_root_element (LSM_SVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	lsm_svg_svg_element_measure (svg_element, width, height);
	if (baseline)
		*baseline = *height;
}

static void
lsm_svg_view_render (LsmDomView *view)
{
	LsmSvgView *svg_view;
	LsmSvgSvgElement *svg_element;

	svg_view = LSM_SVG_VIEW (view);

	svg_element = lsm_svg_document_get_root_element (LSM_SVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	svg_view->style_stack = NULL;
	svg_view->element_stack = NULL;
	svg_view->viewbox_stack = NULL;
	svg_view->matrix_stack = NULL;
	svg_view->pango_layout_stack = NULL;

	svg_view->is_clipping = FALSE;
	svg_view->is_pango_layout_in_use = FALSE;
	svg_view->pango_layout = view->pango_layout;

	svg_view->resolution_ppi = lsm_dom_document_get_resolution (view->document);

	lsm_svg_svg_element_render  (svg_element, svg_view);

	if (svg_view->is_pango_layout_in_use)
		g_warning ("[LsmSvgView::render] Unfinished text redenring");

	if (svg_view->is_clipping)
		g_warning ("[LsmSvgView::render] Unfinished clipping");

	if (svg_view->pango_layout_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling pango_layout in stack");
		g_slist_free (svg_view->pango_layout_stack);
		svg_view->pango_layout_stack = NULL;
	}

	if (svg_view->matrix_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling matrix in stack");
		g_slist_free (svg_view->matrix_stack);
		svg_view->matrix_stack = NULL;
	}
	if (svg_view->viewbox_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling viewport in stack");
		g_slist_free (svg_view->viewbox_stack);
		svg_view->viewbox_stack = NULL;
	}
	if (svg_view->element_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling element in stack");
		g_slist_free (svg_view->element_stack);
		svg_view->element_stack = NULL;
	}
	if (svg_view->style_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling style in stack");
		g_slist_free (svg_view->style_stack);
		svg_view->style_stack = NULL;
	}
}

LsmSvgView *
lsm_svg_view_new (LsmSvgDocument *document)
{
	LsmSvgView *view;

	view = g_object_new (LSM_TYPE_SVG_VIEW, NULL);

	lsm_dom_view_set_document (LSM_DOM_VIEW (view), LSM_DOM_DOCUMENT (document));

	return view;
}

static void
lsm_svg_view_init (LsmSvgView *view)
{
}

static void
lsm_svg_view_finalize (GObject *object)
{
	parent_class->finalize (object);
}

static void
lsm_svg_view_class_init (LsmSvgViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);
	LsmDomViewClass *d_view_class = LSM_DOM_VIEW_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = lsm_svg_view_finalize;

	d_view_class->measure = lsm_svg_view_measure;
	d_view_class->render = lsm_svg_view_render;
}

G_DEFINE_TYPE (LsmSvgView, lsm_svg_view, LSM_TYPE_DOM_VIEW)
