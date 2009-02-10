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

#include <gsvgcairo.h>
#include <math.h>

static void
skip_spaces (char **path)
{
	while (**path == ' ')
		(*path)++;
}

static void
skip_comma_and_spaces (char **path)
{
	while (**path == ' ' || **path == ',')
		(*path)++;
}

static gboolean
parse_value (char **path, double *x)
{
	char *end, *c;
	gboolean integer_part = FALSE;
	gboolean fractional_part = FALSE;
	gboolean exponent_part = FALSE;
	double mantissa = 0.0;
	double exponent =0.0;
	double divisor;
	gboolean mantissa_sign = 1.0;
	gboolean exponent_sign = 1.0;

	c = *path;

	if (*c == '-') {
		mantissa_sign = -1.0;
		c++;
	} else if (*c == '+')
		c++;

	if (*c >= '0' && *c <= '9') {
		integer_part = TRUE;
		mantissa = *c - '0';
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa = mantissa * 10.0 + *c - '0';
			c++;
		}
	}


	if (*c == '.')
		c++;
	else if (!integer_part)
		return FALSE;

	if (*c >= '0' && *c <= '9') {
		fractional_part = TRUE;
		mantissa += (*c - '0') * 0.1;
		divisor = 0.01;
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa += (*c - '0') * divisor;
			divisor *= 0.1;
			c++;
		}
	}

	if (!fractional_part && !integer_part)
		return FALSE;

	end = c;

	if (*c == 'E' || *c == 'e') {
		c++;

		if (*c == '-') {
			exponent_sign = -1.0;
			c++;
		} else if (*c == '+')
			c++;

		if (*c >= '0' && *c <= '9') {
			exponent_part = TRUE;
			exponent = *c - '0';
			c++;

			while (*c >= '0' && *c <= '9') {
				exponent = exponent * 10.0 + *c - '0';
				c++;
			}
		}

	}

	if (exponent_part) {
		end = c;
		*x = mantissa_sign * mantissa * pow (10.0, exponent_sign * exponent);
	} else
		*x = mantissa_sign * mantissa;

	*path = end;

	return TRUE;
}

static gboolean
parse_values (char **path, unsigned int n_values, double *values)
{
	char *ptr = *path;
	unsigned int i;

	skip_comma_and_spaces (path);

	for (i = 0; i < n_values; i++) {
		if (!parse_value (path, &values[i])) {
			*path = ptr;
			return FALSE;
		}
		skip_comma_and_spaces (path);
	}

	return TRUE;
}

static void
emit_function_2 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double, double))
{
	double values[2];

	skip_spaces (path);

	while (parse_values (path, 2, values))
		cairo_func (cr, values[0], values[1]);
}

static void
emit_function_6 (char **path, cairo_t *cr,
		 void (*cairo_func) (cairo_t *, double, double, double ,double, double, double))
{
	double values[6];

	skip_spaces (path);

	while (parse_values (path, 6, values))
		cairo_func (cr, values[0], values[1], values[2], values[3], values[4], values[5]);
}

/**
 * gsvg_cairo_emit_svg_path:
 * @cr: a cairo context
 * @path: a SVG path
 *
 * Emits a path described as a SVG path string (d property of path elements) to 
 * a cairo context.
 **/

void
gsvg_cairo_emit_svg_path (cairo_t *cr, char const *path)
{
	char *ptr;

	g_return_if_fail (cr != NULL);

	if (path == NULL)
		return;

	ptr = (char *) path;

	skip_spaces (&ptr);

	while (*ptr != '\0') {
		if (*ptr == 'M') {
			ptr++;
			emit_function_2 (&ptr, cr, cairo_move_to);
		} else if (*ptr == 'm') {
			ptr++;
			emit_function_2 (&ptr, cr, cairo_rel_move_to);
		} else if (*ptr == 'L') {
			ptr++;
			emit_function_2 (&ptr, cr, cairo_line_to);
		} else if (*ptr == 'l') {
			ptr++;
			emit_function_2 (&ptr, cr, cairo_rel_line_to);
		} else if (*ptr == 'C') {
			ptr++;
			emit_function_6 (&ptr, cr, cairo_curve_to);
		} else if (*ptr == 'c') {
			ptr++;
			emit_function_6 (&ptr, cr, cairo_rel_curve_to);
		} else if (*ptr == 'Z' || *ptr == 'z') {
			ptr++;
			cairo_close_path (cr);
		} else
			ptr++;
	}
}

