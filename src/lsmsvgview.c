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

#include <lsmdebug.h>
#include <lsmsvgview.h>
#include <lsmsvgdocument.h>
#include <lsmsvgelement.h>
#include <lsmsvgsvgelement.h>
#include <lsmsvggradientelement.h>
#include <lsmsvgpatternelement.h>
#include <lsmsvgclippathelement.h>
#include <lsmsvgutils.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include <math.h>
#include <string.h>

static GObjectClass *parent_class;

struct _LsmSvgViewPatternData {
	cairo_t *old_cairo;

	cairo_pattern_t *pattern;
	cairo_matrix_t matrix;

	LsmBox extents;

	LsmSvgPatternUnits units;
	LsmSvgPatternUnits content_units;
	LsmSvgSpreadMethod spread_method;
};

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
_start_pattern (LsmSvgView *view)
{
	double x1, x2, y1, y2;
	lsm_debug ("[LsmSvgView::start_pattern]");

	view->pattern_stack = g_slist_prepend (view->pattern_stack, view->pattern_data);

	view->pattern_data = g_new (LsmSvgViewPatternData, 1);
	view->pattern_data->old_cairo = view->dom_view.cairo;
	view->pattern_data->pattern = NULL;
	view->pattern_data->content_units = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;
	view->pattern_data->units = LSM_SVG_PATTERN_UNITS_USER_SPACE_ON_USE;
	view->pattern_data->spread_method = LSM_SVG_SPREAD_METHOD_REPEAT;

	cairo_path_extents (view->dom_view.cairo, &x1, &y1, &x2, &y2);

	view->dom_view.cairo = NULL;

	view->pattern_data->extents.x = x1;
	view->pattern_data->extents.y = y1;
	view->pattern_data->extents.width = x2 - x1;
	view->pattern_data->extents.height = y2 - y1;

	cairo_matrix_init_identity (&view->pattern_data->matrix);
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

	lsm_debug ("[LsmSvgView::end_pattern]");
}

static void
_set_pattern (LsmSvgView *view, cairo_pattern_t *pattern)
{
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->pattern_data->pattern == NULL);

	view->pattern_data->pattern = pattern;
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
lsm_svg_view_add_gradient_color_stop (LsmSvgView *view, double offset, const LsmSvgColor *color, double opacity)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->pattern_data->pattern != NULL);
	g_return_if_fail (color != NULL);

	cairo_pattern_add_color_stop_rgba (view->pattern_data->pattern, offset,
					   color->red, color->green, color->blue, opacity);
}

void
lsm_svg_view_set_gradient_properties (LsmSvgView *view,
				      LsmSvgSpreadMethod method,
				      LsmSvgPatternUnits units,
				      const LsmSvgMatrix *matrix)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->pattern_data != NULL);

	view->pattern_data->units = units;
	view->pattern_data->spread_method = method;

	if (matrix != NULL) {
		cairo_matrix_init (&view->pattern_data->matrix,
				   matrix->a, matrix->b,
				   matrix->c, matrix->d,
				   matrix->e, matrix->f);
		cairo_matrix_invert (&view->pattern_data->matrix);
	} else
		cairo_matrix_init_identity (&view->pattern_data->matrix);
}

void
lsm_svg_view_create_surface_pattern (LsmSvgView *view,
				     const LsmBox *viewport,
				     LsmSvgPatternUnits units,
				     LsmSvgPatternUnits content_units,
				     const LsmSvgMatrix *matrix)
{
	cairo_surface_t *surface;
	cairo_pattern_t *pattern;
	double width, height, x, y;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (viewport != NULL);
	g_return_if_fail (view->pattern_data != NULL);
	g_return_if_fail (view->dom_view.cairo == NULL);

	x = viewport->x;
	y = viewport->y;
	width = viewport->width;
	height = viewport->height;

	if (units == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX) {
		x *= view->pattern_data->extents.width;
		y *= view->pattern_data->extents.height;
		width *= view->pattern_data->extents.width;
		height *= view->pattern_data->extents.height;
	}

	lsm_debug ("[LsmSvgView::create_pattern] pattern size = %g ,%g",
		   width, height);

	if (height < 1 || width < 1)
		return;

	surface = cairo_surface_create_similar (cairo_get_target (view->pattern_data->old_cairo),
						CAIRO_CONTENT_COLOR_ALPHA,
						width, height);
	pattern = cairo_pattern_create_for_surface (surface);
	view->dom_view.cairo = cairo_create (surface);
	cairo_surface_destroy (surface);

	cairo_translate (view->dom_view.cairo, -x, -y);

	_set_pattern (view, pattern);

	view->pattern_data->units = units;
	view->pattern_data->content_units = content_units;

	if (matrix != NULL) {
		cairo_matrix_init (&view->pattern_data->matrix,
				   matrix->a, matrix->b,
				   matrix->c, matrix->d,
				   matrix->e + x, matrix->f + y);
		cairo_matrix_invert (&view->pattern_data->matrix);
	} else
		cairo_matrix_init_translate (&view->pattern_data->matrix, -x, -y);

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
 * is adpated from the goocanvas library (goocanvasutils.c)
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
	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 1, ctxt->values))
		cairo_func (ctxt->cr, ctxt->values[0]);
}

static void
_emit_function_2 (LsmSvgPathContext *ctxt,
		  void (*cairo_func_a) (cairo_t *, double, double),
		  void (*cairo_func_b) (cairo_t *, double, double))
{

	if (lsm_svg_str_parse_double_list (&ctxt->ptr, 2, ctxt->values)) {
		cairo_func_a (ctxt->cr, ctxt->values[0], ctxt->values[1]);

		/* Why oh why does the specification say Line is implied here ? */

		while (lsm_svg_str_parse_double_list (&ctxt->ptr, 2, ctxt->values))
			cairo_func_b (ctxt->cr, ctxt->values[0], ctxt->values[1]);
	}
}

static void
_emit_function_4 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double, double, double, double))
{
	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 4, ctxt->values))
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

	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 4, ctxt->values)) {
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

	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 2, ctxt->values)) {
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
	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 6, ctxt->values))
		cairo_func (ctxt->cr, ctxt->values[0], ctxt->values[1], ctxt->values[2],
			              ctxt->values[3], ctxt->values[4], ctxt->values[5]);
}

static void
_emit_function_7 (LsmSvgPathContext *ctxt,
		  void (*cairo_func) (cairo_t *, double, double, double ,gboolean, gboolean, double, double))
{
	while (lsm_svg_str_parse_double_list (&ctxt->ptr, 7, ctxt->values))
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

	lsm_svg_str_skip_spaces (&ctxt.ptr);

	while (*ctxt.ptr != '\0') {
		char command;

		command = *ctxt.ptr;
		ctxt.ptr++;
		lsm_svg_str_skip_spaces (&ctxt.ptr);

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

double
lsm_svg_view_normalize_length (LsmSvgView *view, const LsmSvgLength *length, LsmSvgLengthDirection direction)
{
	double font_size;

	g_return_val_if_fail (LSM_IS_SVG_VIEW (view), 0.0);

	/* TODO cache font size */
	if (view->text_stack != NULL && view->viewbox_stack != NULL) {
		LsmSvgTextAttributeBag *text;

		text = view->text_stack->data;
		font_size = lsm_svg_length_normalize (&text->font_size.length, view->viewbox_stack->data,
						      0.0, LSM_SVG_LENGTH_DIRECTION_DIAGONAL);
	} else
		font_size = 0.0;

	return lsm_svg_length_normalize (length, view->viewbox_stack->data, font_size, direction);
}

void
lsm_svg_view_push_viewbox (LsmSvgView *view, const LsmBox *viewbox)
{
	LsmSvgViewbox *svg_viewbox;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	lsm_debug ("[LsmSvgView::push_viewbox] viewbox = %g, %g, %g, %g",
		   viewbox->x, viewbox->y, viewbox->width, viewbox->height);

	svg_viewbox = lsm_svg_viewbox_new (view->resolution_ppi, viewbox);

	view->viewbox_stack = g_slist_prepend (view->viewbox_stack, svg_viewbox);
}

void
lsm_svg_view_pop_viewbox (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->viewbox_stack != NULL);

	lsm_debug ("[LsmSvgView::pop_viewbox]");

	lsm_svg_viewbox_free (view->viewbox_stack->data);
	view->viewbox_stack = g_slist_delete_link (view->viewbox_stack, view->viewbox_stack);
}

void
lsm_svg_view_push_viewport (LsmSvgView *view, const LsmBox *viewport, const LsmBox *viewbox,
			    const LsmSvgPreserveAspectRatio *aspect_ratio)
{
	cairo_t *cairo;
	double x_ratio, x_scale;
	double y_ratio, y_scale;
	double x, y;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (viewport != NULL);
	g_return_if_fail (aspect_ratio != NULL);

	x = viewport->x;
	y = viewport->y;

	if (viewbox != NULL) {
		x_ratio = viewbox->width  > 0.0 ? viewport->width  / viewbox->width  : 0.0;
		y_ratio = viewbox->height > 0.0 ? viewport->height / viewbox->height : 0.0;

		if (aspect_ratio->align > LSM_SVG_ALIGN_NONE) {
			if (aspect_ratio->meet_or_slice == LSM_SVG_MEET_OR_SLICE_MEET) {
				x_scale = MIN (x_ratio, y_ratio);
				y_scale = x_scale;
			} else {
				x_scale = MAX (x_ratio, y_ratio);
				y_scale = x_scale;
			}

			x -= viewbox->x;
			y -= viewbox->y;

			switch (aspect_ratio->align) {
				case LSM_SVG_ALIGN_X_MIN_Y_MIN:
					break;
				case LSM_SVG_ALIGN_X_MIN_Y_MID:
					y += (viewport->height- viewbox->height * y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MIN_Y_MAX:
					y += (viewport->height - viewbox->height * y_scale);
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MIN:
					x += (viewport->width - viewbox->width * x_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MID:
					x += (viewport->width - viewbox->width * x_scale) * 0.5;
					y += (viewport->height- viewbox->height * y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MID_Y_MAX:
					x += (viewport->width - viewbox->width * x_scale) * 0.5;
					y += (viewport->height - viewbox->height * y_scale);
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MIN:
					x += (viewport->width - viewbox->width * x_scale);
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MID:
					x += (viewport->width - viewbox->width * x_scale);
					y += (viewport->height- viewbox->height * y_scale) * 0.5;
					break;
				case LSM_SVG_ALIGN_X_MAX_Y_MAX:
					x += (viewport->width - viewbox->width * x_scale);
					y += (viewport->height - viewbox->height * y_scale);
					break;
				default:
					break;
			}
		} else {
			x_scale = x_ratio;
			y_scale = y_ratio;
		}

		lsm_debug ("[LsmSvgView::push_viewport] scale = %g, %g", x_scale, y_scale);

		lsm_svg_view_push_viewbox (view, viewbox);

	} else {
		x_scale = y_scale = 1.0;
		lsm_svg_view_push_viewbox (view, viewport);
	}

	cairo = view->dom_view.cairo;

	cairo_save (cairo);
	cairo_rectangle (cairo, viewport->x, viewport->y, viewport->width, viewport->height);
	cairo_clip (cairo);
	cairo_translate (cairo, x, y);
	cairo_scale (cairo, x_scale, y_scale);
}

void
lsm_svg_view_pop_viewport (LsmSvgView *view)
{
	cairo_restore (view->dom_view.cairo);

	lsm_svg_view_pop_viewbox (view);
}

void
lsm_svg_view_push_matrix (LsmSvgView *view, LsmSvgMatrix *matrix)
{
	cairo_matrix_t cr_matrix;
	cairo_matrix_t *ctm;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	ctm = g_new (cairo_matrix_t, 1);
	cairo_get_matrix (view->dom_view.cairo, ctm);

	view->matrix_stack = g_slist_prepend (view->matrix_stack, ctm);

	lsm_debug ("[LsmSvgView::push_matrix] New transform %g, %g, %g, %g, %g, %g",
		   matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);

	cairo_matrix_init (&cr_matrix, matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
	cairo_transform (view->dom_view.cairo, &cr_matrix);

	{
		cairo_matrix_t current_ctm;
		cairo_get_matrix (view->dom_view.cairo, &current_ctm);

		lsm_debug ("[LsmSvgView::push_matrix] Current ctm %g, %g, %g, %g, %g, %g",
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

		g_free (ctm);
		view->matrix_stack = g_slist_delete_link (view->matrix_stack, view->matrix_stack);
	}
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

static void
lsm_svg_view_push_clip (LsmSvgView *view, char *clip_path, LsmSvgFillRule clip_rule)
{
	LsmDomElement *element;
	LsmExtents extents;
	char *uri;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (!view->is_clipping);

	lsm_svg_element_get_extents (view->element_stack->data, view, &extents);

	uri = clip_path;

	lsm_debug ("[LsmSvgView::push_clip] Using '%s'", clip_path);

	cairo_save (view->dom_view.cairo);

	view->clip_extents.x = extents.x1;
	view->clip_extents.y = extents.y1;
	view->clip_extents.width  = extents.x2 - extents.x1;
	view->clip_extents.height = extents.y2 - extents.y1;

	if (strncmp (uri, "url(#", 5) ==0) {
		char *end;

		uri = g_strdup (uri + 5);
		for (end = uri; *end != '\0' && *end != ')'; end++);
		*end = '\0';

		element = lsm_dom_document_get_element_by_id (view->dom_view.document, uri);

		g_free (uri);

		if (element != NULL && LSM_IS_SVG_CLIP_PATH_ELEMENT (element)) {
			view->is_clipping = TRUE;
			lsm_svg_element_render_clip (LSM_SVG_ELEMENT (element), view);
			cairo_clip (view->dom_view.cairo);
			view->is_clipping = FALSE;
		}
	}
}

static void
lsm_svg_view_pop_clip (LsmSvgView *view)
{
	lsm_debug ("[LsmSvgView::pop_clip");

	cairo_restore (view->dom_view.cairo);
}

void
lsm_svg_view_push_group_opacity (LsmSvgView *view)
{
	LsmSvgMaskAttributeBag *mask;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->mask_stack != NULL);

	mask = view->mask_stack->data;

	if (mask->opacity.value < 1.0)
		cairo_push_group (view->dom_view.cairo);
}

void
lsm_svg_view_pop_group_opacity (LsmSvgView *view)
{
	LsmSvgMaskAttributeBag *mask;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->mask_stack != NULL);

	mask = view->mask_stack->data;

	if (mask->opacity.value < 1.0) {
		cairo_pop_group_to_source (view->dom_view.cairo);
		cairo_paint_with_alpha (view->dom_view.cairo, mask->opacity.value);
	}
}

void
lsm_svg_view_push_mask_attributes (LsmSvgView *view, const LsmSvgMaskAttributeBag *mask)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (mask != NULL);

	view->mask_stack = g_slist_prepend (view->mask_stack, (void *) mask);

	if (mask->clip_path.value != NULL)
		lsm_svg_view_push_clip (view, mask->clip_path.value, mask->clip_rule.value);
}

void
lsm_svg_view_pop_mask_attributes (LsmSvgView *view)
{
	LsmSvgMaskAttributeBag *mask;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->mask_stack != NULL);

	mask = view->mask_stack->data;

	if (mask->clip_path.value != NULL)
		lsm_svg_view_pop_clip (view);

	view->mask_stack = g_slist_delete_link (view->mask_stack, view->mask_stack);
}

void
lsm_svg_view_push_fill_attributes (LsmSvgView *view, const LsmSvgFillAttributeBag *fill)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (fill != NULL);

	view->fill_stack = g_slist_prepend (view->fill_stack, (void *) fill);
}

void
lsm_svg_view_pop_fill_attributes (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->fill_stack != NULL);

	view->fill_stack = g_slist_delete_link (view->fill_stack, view->fill_stack);
}

void
lsm_svg_view_push_stroke_attributes (LsmSvgView *view, const LsmSvgStrokeAttributeBag *stroke)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (stroke != NULL);

	view->stroke_stack = g_slist_prepend (view->stroke_stack, (void *) stroke);
}

void
lsm_svg_view_pop_stroke_attributes (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->stroke_stack != NULL);

	view->stroke_stack = g_slist_delete_link (view->stroke_stack, view->stroke_stack);
}

void
lsm_svg_view_push_text_attributes (LsmSvgView *view, const LsmSvgTextAttributeBag *text)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (text != NULL);

	view->text_stack = g_slist_prepend (view->text_stack, (void *) text);
}

void
lsm_svg_view_pop_text_attributes (LsmSvgView *view)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (view->text_stack != NULL);

	view->text_stack = g_slist_delete_link (view->text_stack, view->text_stack);
}

typedef enum {
	LSM_SVG_VIEW_PAINT_OPERATION_FILL,
	LSM_SVG_VIEW_PAINT_OPERATION_STROKE
} LsmSvgViewPaintOperation;

static void
_paint_uri (LsmSvgView *view, LsmSvgViewPaintOperation operation, const char *uri)
{
	cairo_t *cairo;
	LsmDomElement *element;

	element = lsm_dom_document_get_element_by_id (view->dom_view.document, uri);
	if (!LSM_IS_SVG_GRADIENT_ELEMENT (element) &&
	    !LSM_IS_SVG_PATTERN_ELEMENT (element))
		return;

	_start_pattern (view);

	lsm_svg_element_render_paint (LSM_SVG_ELEMENT (element), view);

	cairo = view->pattern_data->old_cairo;

	if (view->pattern_data->pattern) {
#if 0
		if (LSM_IS_SVG_PATTERN_ELEMENT (element)) {
			char *filename;

			filename = g_strdup_printf ("%s.png", uri);
			cairo_surface_write_to_png (cairo_get_target (view->dom_view.cairo), filename);
			g_free (filename);
		}
#endif

		if (LSM_IS_SVG_GRADIENT_ELEMENT (element) &&
		    view->pattern_data->units == LSM_SVG_PATTERN_UNITS_OBJECT_BOUNDING_BOX) {
			cairo_matrix_t matrix;

			if (view->pattern_data->extents.width <= 0.0 ||
			    view->pattern_data->extents.height <= 0.0) {
				_end_pattern (view);
				return;
			}

			cairo_matrix_init_scale (&matrix,
						 1.0 / view->pattern_data->extents.width,
						 1.0 / view->pattern_data->extents.height);
			cairo_matrix_translate (&matrix,
						-view->pattern_data->extents.x,
						-view->pattern_data->extents.y);

			cairo_matrix_multiply (&matrix, &view->pattern_data->matrix, &matrix);
			cairo_pattern_set_matrix (view->pattern_data->pattern, &matrix);
		} else
			cairo_pattern_set_matrix (view->pattern_data->pattern, &view->pattern_data->matrix);

		switch (view->pattern_data->spread_method) {
			case LSM_SVG_SPREAD_METHOD_REFLECT:
				cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_REFLECT);
				break;
			case LSM_SVG_SPREAD_METHOD_REPEAT:
				cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_REPEAT);
				break;
			default:
				cairo_pattern_set_extend (view->pattern_data->pattern, CAIRO_EXTEND_PAD);
		}

		cairo_set_source (cairo, view->pattern_data->pattern);
	} else
		cairo_set_source_rgb (cairo, 0.0, 0.0, 0.0);

	_end_pattern (view);
}

static gboolean
_set_color (LsmSvgView *view, LsmSvgViewPaintOperation operation,
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
		case LSM_SVG_PAINT_TYPE_URI:
		case LSM_SVG_PAINT_TYPE_URI_RGB_COLOR:
		case LSM_SVG_PAINT_TYPE_URI_CURRENT_COLOR:
		case LSM_SVG_PAINT_TYPE_URI_NONE:
			_paint_uri (view, operation, paint->uri);
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

static void
paint (LsmSvgView *view)
{
	LsmSvgFillAttributeBag *fill;
	LsmSvgStrokeAttributeBag *stroke;
	LsmSvgGraphic *graphic;
	cairo_t *cairo;
	gboolean use_group;
	double group_opacity;

	g_return_if_fail (view->fill_stack != NULL);
	g_return_if_fail (view->stroke_stack != NULL);
	g_return_if_fail (view->element_stack != NULL);

	graphic = view->element_stack->data;

	g_return_if_fail (LSM_IS_SVG_GRAPHIC (graphic));

	cairo = view->dom_view.cairo;
	fill = view->fill_stack->data;
	stroke = view->stroke_stack->data;

	if (view->mask_stack != NULL && view->mask_stack->data == graphic->mask) {
		LsmSvgMaskAttributeBag *mask;

		mask = view->mask_stack->data;
		group_opacity = mask->opacity.value;

		use_group = fill->paint.paint.type != LSM_SVG_PAINT_TYPE_NONE &&
			stroke->paint.paint.type != LSM_SVG_PAINT_TYPE_NONE &&
			group_opacity < 1.0;
	} else {
		use_group = FALSE;
		group_opacity = 1.0;
	}

	/* FIXME Instead of push_group, restrict to the current path bounding box */
	if (use_group)
		cairo_push_group (cairo);

	if (_set_color (view, LSM_SVG_VIEW_PAINT_OPERATION_FILL,
			&fill->paint.paint, fill->opacity.value * group_opacity)) {
		cairo_set_fill_rule (cairo, fill->rule.value == LSM_SVG_FILL_RULE_EVEN_ODD ?
				     CAIRO_FILL_RULE_EVEN_ODD : CAIRO_FILL_RULE_WINDING);
		cairo_fill_preserve (cairo);
	}

	if (_set_color (view, LSM_SVG_VIEW_PAINT_OPERATION_STROKE,
			&stroke->paint.paint, stroke->opacity.value * group_opacity)) {
		double line_width;

		switch (stroke->line_join.value) {
			case LSM_SVG_LINE_JOIN_MITER:
				cairo_set_line_join (cairo, CAIRO_LINE_JOIN_MITER);
				break;
			case LSM_SVG_LINE_JOIN_ROUND:
				cairo_set_line_join (cairo,CAIRO_LINE_JOIN_ROUND);
				break;
			default:
				cairo_set_line_join (cairo,CAIRO_LINE_JOIN_BEVEL);
		}

		switch (stroke->line_cap.value) {
			case LSM_SVG_LINE_CAP_BUTT:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_BUTT);
				break;
			case LSM_SVG_LINE_CAP_ROUND:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_ROUND);
				break;
			default:
				cairo_set_line_cap (cairo, CAIRO_LINE_CAP_SQUARE);
		}

		line_width = lsm_svg_view_normalize_length (view, &stroke->width.length,
							    LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

		cairo_set_miter_limit (cairo, stroke->miter_limit.value);
		cairo_set_line_width (cairo, line_width);

		if (stroke->dash_array.value != NULL &&
		    stroke->dash_array.value->n_dashes > 0) {
			double dash_offset;
			double *dashes;
			unsigned int i;

			dash_offset = lsm_svg_view_normalize_length (view, &stroke->dash_offset.length,
								     LSM_SVG_LENGTH_DIRECTION_DIAGONAL);
			dashes = g_new(double, stroke->dash_array.value->n_dashes);
			for (i = 0; i < stroke->dash_array.value->n_dashes; i++)
				dashes[i] = lsm_svg_view_normalize_length (view,
									   &stroke->dash_array.value->dashes[i],
									   LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

			cairo_set_dash (cairo,
					dashes,
					stroke->dash_array.value->n_dashes,
					dash_offset);

			g_free (dashes);
		} else
			cairo_set_dash (cairo, NULL, 0, 0.0);

		cairo_stroke (cairo);
	}

	cairo_new_path (cairo);

	if (use_group) {
		cairo_pop_group_to_source (cairo);
		cairo_paint_with_alpha (cairo, group_opacity);
	}
}

static void
process_path (LsmSvgView *view)
{
	if (view->is_clipping) {
		LsmSvgMaskAttributeBag *mask;

		mask = view->mask_stack->data;
		cairo_set_fill_rule (view->dom_view.cairo, mask->clip_rule.value);
	} else
		paint (view);
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

	process_path (view);
}

void
lsm_svg_view_show_circle (LsmSvgView *view, double cx, double cy, double r)
{

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_arc (view->dom_view.cairo, cx, cy, r, 0, 2 * M_PI);

	process_path (view);
}

#define LSM_SVG_ARC_MAGIC ((double) 0.5522847498) /* 4/3 * (1-cos 45)/sin 45 = 4/3 * sqrt(2) - 1 */

void
lsm_svg_view_show_ellipse (LsmSvgView *view, double cx, double cy, double rx, double ry)
{
	cairo_t *cairo;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	if (rx <= 0 || ry <= 0)
		return;

	cairo = view->dom_view.cairo;

	cairo_move_to (cairo, cx + rx, cy);
	cairo_curve_to (cairo, cx + rx, cy - LSM_SVG_ARC_MAGIC * ry, cx + LSM_SVG_ARC_MAGIC * rx, cy - ry, cx, cy - ry);
	cairo_curve_to (cairo, cx - LSM_SVG_ARC_MAGIC * rx, cy - ry, cx - rx, cy - LSM_SVG_ARC_MAGIC * ry, cx - rx, cy);
	cairo_curve_to (cairo, cx - rx, cy + LSM_SVG_ARC_MAGIC * ry, cx - LSM_SVG_ARC_MAGIC * rx, cy + ry, cx, cy + ry);
	cairo_curve_to (cairo, cx + LSM_SVG_ARC_MAGIC * rx, cy + ry, cx + rx, cy + LSM_SVG_ARC_MAGIC * ry, cx + rx, cy);
	cairo_close_path (cairo);

	process_path (view);
}

void
lsm_svg_view_show_path (LsmSvgView *view,
		     const char *d)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	_emit_svg_path (view->dom_view.cairo, d);

	process_path (view);
}

void
lsm_svg_view_show_line (LsmSvgView *view, double x1, double y1, double x2, double y2)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	cairo_move_to (view->dom_view.cairo, x1, y1);
	cairo_line_to (view->dom_view.cairo, x2, y2);

	process_path (view);
}

void
lsm_svg_view_show_polyline (LsmSvgView *view, const char *points)
{
	char *str;
	double values[2];

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	str = (char *) points;

	if (lsm_svg_str_parse_double_list (&str, 2, values)) {
		cairo_move_to (view->dom_view.cairo, values[0], values[1]);
		while (lsm_svg_str_parse_double_list (&str, 2, values))
			cairo_line_to (view->dom_view.cairo, values[0], values[1]);
	}

	process_path (view);
}

void
lsm_svg_view_show_polygon (LsmSvgView *view, const char *points)
{
	char *str;
	double values[2];

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	str = (char *) points;

	if (lsm_svg_str_parse_double_list (&str, 2, values)) {
		cairo_move_to (view->dom_view.cairo, values[0], values[1]);
		while (lsm_svg_str_parse_double_list (&str, 2, values))
			cairo_line_to (view->dom_view.cairo, values[0], values[1]);
	}

	cairo_close_path (view->dom_view.cairo);

	process_path (view);
}

void
lsm_svg_view_show_text (LsmSvgView *view, char const *string, double x, double y)
{
	PangoLayout *pango_layout;
	PangoFontDescription *font_description;
	PangoLayoutIter *iter;
	PangoRectangle ink_rect;
	LsmSvgTextAttributeBag *text;
	double font_size;
	int baseline;

	if (string == NULL)
		return;

	g_return_if_fail (LSM_IS_SVG_VIEW (view));

	text = view->text_stack->data;
	g_return_if_fail (text != NULL);

	pango_layout = view->dom_view.pango_layout;
	font_description = view->dom_view.font_description;

	font_size = lsm_svg_view_normalize_length (view, &text->font_size.length, LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	pango_font_description_set_family (font_description, text->font_family.value);
	pango_font_description_set_size (font_description, font_size * PANGO_SCALE);

	pango_layout_set_text (pango_layout, string, -1);
	pango_layout_set_font_description (pango_layout, font_description);
	pango_layout_get_extents (pango_layout, &ink_rect, NULL);

	iter = pango_layout_get_iter (pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	cairo_move_to (view->dom_view.cairo,
		       x - pango_units_to_double (ink_rect.x),
		       y - pango_units_to_double (baseline));

	pango_cairo_layout_path (view->dom_view.cairo, pango_layout);

	process_path (view);
}

void
lsm_svg_view_show_pixbuf (LsmSvgView *view, GdkPixbuf *pixbuf)
{
	g_return_if_fail (LSM_IS_SVG_VIEW (view));
	g_return_if_fail (GDK_IS_PIXBUF (pixbuf));

	gdk_cairo_set_source_pixbuf (view->dom_view.cairo, pixbuf, 0, 0);
	cairo_paint (view->dom_view.cairo);
}

static void
lsm_svg_view_measure (LsmDomView *view, double *width, double *height)
{
	LsmSvgSvgElement *svg_element;

	svg_element = lsm_svg_document_get_root_element (LSM_SVG_DOCUMENT (view->document));
	if (svg_element == NULL)
		return;

	lsm_svg_svg_element_update (svg_element);

	lsm_svg_svg_element_measure (svg_element, width, height);
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

	lsm_svg_svg_element_update (svg_element);

	svg_view->element_stack = NULL;
	svg_view->viewbox_stack = NULL;
	svg_view->mask_stack = NULL;
	svg_view->fill_stack = NULL;
	svg_view->stroke_stack = NULL;
	svg_view->text_stack = NULL;
	svg_view->matrix_stack = NULL;

	svg_view->is_clipping = FALSE;

	svg_view->resolution_ppi = lsm_dom_document_get_resolution (view->document);

	lsm_svg_svg_element_render  (svg_element, svg_view);

	if (svg_view->is_clipping)
		g_warning ("[LsmSvgView::render] Unfinished clipping");

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
	if (svg_view->fill_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling fill attributes in stack");
		g_slist_free (svg_view->fill_stack);
		svg_view->fill_stack = NULL;
	}
	if (svg_view->stroke_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling stroke attributes in stack");
		g_slist_free (svg_view->stroke_stack);
		svg_view->stroke_stack = NULL;
	}
	if (svg_view->mask_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling mask attributes in stack");
		g_slist_free (svg_view->mask_stack);
		svg_view->mask_stack = NULL;
	}
	if (svg_view->text_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling text attribute in stack");
		g_slist_free (svg_view->text_stack);
		svg_view->text_stack = NULL;
	}
	if (svg_view->element_stack != NULL) {
		g_warning ("[LsmSvgView::render] Dangling element in stack");
		g_slist_free (svg_view->element_stack);
		svg_view->element_stack = NULL;
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
