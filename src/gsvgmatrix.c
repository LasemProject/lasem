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

#include <gsvgmatrix.h>
#include <math.h>

void
gsvg_matrix_init (GSvgMatrix *matrix, double a, double b, double c, double d, double e, double f)
{
	matrix->a = a;
	matrix->b = b;
	matrix->c = c;
	matrix->d = d;
	matrix->e = e;
	matrix->f = f;
}

void
gsvg_matrix_init_identity (GSvgMatrix *matrix)
{
	gsvg_matrix_init (matrix, 1, 0, 0, 1, 0, 0);
}

void
gsvg_matrix_scale (GSvgMatrix *matrix, double sx, double sy)
{
	GSvgMatrix tmp;

	gsvg_matrix_init (&tmp, sx, 0, 0, sy, 0, 0);
	gsvg_matrix_multiply (matrix, &tmp, matrix);
}

void
gsvg_matrix_translate (GSvgMatrix *matrix, double tx, double ty)
{
	GSvgMatrix tmp;

	gsvg_matrix_init (&tmp, 1, 0, 0, 1, tx, ty);
	gsvg_matrix_multiply (matrix, &tmp, matrix);
}

void
gsvg_matrix_rotate (GSvgMatrix *matrix, double radians)
{
	GSvgMatrix tmp;
	double  s;
	double  c;

	s = sin (radians);
	c = cos (radians);

	gsvg_matrix_init (&tmp, c, s, -s, c, 0, 0);
	gsvg_matrix_multiply (matrix, &tmp, matrix);
}

void
gsvg_matrix_multiply (GSvgMatrix *result, const GSvgMatrix *a, const GSvgMatrix *b)
{
	GSvgMatrix r;

	r.a = a->a * b->a + a->b * b->c;
	r.b = a->a * b->b + a->b * b->d;

	r.c = a->c * b->a + a->d * b->c;
	r.d = a->c * b->b + a->d * b->d;

	r.e = a->e * b->a + a->f * b->c + b->e;
	r.f = a->e * b->b + a->f * b->d + b->f;

	*result = r;
}
