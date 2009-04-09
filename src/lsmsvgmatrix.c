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

#include <lsmsvgmatrix.h>
#include <math.h>

void
lsm_svg_matrix_init (LsmSvgMatrix *matrix, double a, double b, double c, double d, double e, double f)
{
	matrix->a = a;
	matrix->b = b;
	matrix->c = c;
	matrix->d = d;
	matrix->e = e;
	matrix->f = f;
}

void
lsm_svg_matrix_init_identity (LsmSvgMatrix *matrix)
{
	lsm_svg_matrix_init (matrix, 1, 0, 0, 1, 0, 0);
}

void
lsm_svg_matrix_init_scale (LsmSvgMatrix *matrix, double sx, double sy)
{
	lsm_svg_matrix_init (matrix, sx, 0, 0, sy, 0, 0);
}

void
lsm_svg_matrix_scale (LsmSvgMatrix *matrix, double sx, double sy)
{
	LsmSvgMatrix tmp;

	lsm_svg_matrix_init_scale (&tmp, sx, sy);
	lsm_svg_matrix_multiply (matrix, &tmp, matrix);
}

void
lsm_svg_matrix_init_translate (LsmSvgMatrix *matrix, double tx, double ty)
{
	lsm_svg_matrix_init (matrix, 1, 0, 0, 1, tx, ty);
}

void
lsm_svg_matrix_translate (LsmSvgMatrix *matrix, double tx, double ty)
{
	LsmSvgMatrix tmp;

	lsm_svg_matrix_init_translate (&tmp, tx, ty);
	lsm_svg_matrix_multiply (matrix, &tmp, matrix);
}

void
lsm_svg_matrix_init_rotate (LsmSvgMatrix *matrix, double radians)
{
	double  s;
	double  c;

	s = sin (radians);
	c = cos (radians);

	lsm_svg_matrix_init (matrix, c, s, -s, c, 0, 0);
}

void
lsm_svg_matrix_rotate (LsmSvgMatrix *matrix, double radians)
{
	LsmSvgMatrix tmp;

	lsm_svg_matrix_init_rotate (&tmp,radians);
	lsm_svg_matrix_multiply (matrix, &tmp, matrix);
}

void
lsm_svg_matrix_init_skew_x (LsmSvgMatrix *matrix, double radians)
{
	lsm_svg_matrix_init (matrix, 1, 0, tan(radians), 1, 0, 0);
}

void
lsm_svg_matrix_skew_x (LsmSvgMatrix *matrix, double radians)
{
	LsmSvgMatrix tmp;

	lsm_svg_matrix_init_skew_x (&tmp,radians);
	lsm_svg_matrix_multiply (matrix, &tmp, matrix);
}

void
lsm_svg_matrix_init_skew_y (LsmSvgMatrix *matrix, double radians)
{
	lsm_svg_matrix_init (matrix, 1, tan(radians), 0, 1, 0, 0);
}

void
lsm_svg_matrix_skew_y (LsmSvgMatrix *matrix, double radians)
{
	LsmSvgMatrix tmp;

	lsm_svg_matrix_init_skew_y (&tmp,radians);
	lsm_svg_matrix_multiply (matrix, &tmp, matrix);
}

void
lsm_svg_matrix_multiply (LsmSvgMatrix *result, const LsmSvgMatrix *a, const LsmSvgMatrix *b)
{
	LsmSvgMatrix r;

	r.a = a->a * b->a + a->b * b->c;
	r.b = a->a * b->b + a->b * b->d;

	r.c = a->c * b->a + a->d * b->c;
	r.d = a->c * b->b + a->d * b->d;

	r.e = a->e * b->a + a->f * b->c + b->e;
	r.f = a->e * b->b + a->f * b->d + b->f;

	*result = r;
}
