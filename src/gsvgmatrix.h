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

#ifndef GSVG_MATRIX_H
#define GSVG_MATRIX_H

#include <gsvg.h>

typedef struct {
	double a, b, c, d, e, f;
} GSvgMatrix;

void 	gsvg_matrix_init 		(GSvgMatrix *matrix, double a, double b,
					 double c, double d, double e, double f);
void 	gsvg_matrix_init_identity 	(GSvgMatrix *matrix);
void 	gsvg_matrix_init_scale 		(GSvgMatrix *matrix, double sx, double sy);
void 	gsvg_matrix_scale 		(GSvgMatrix *matrix, double sx, double sy);
void 	gsvg_matrix_init_translate 	(GSvgMatrix *matrix, double tx, double ty);
void 	gsvg_matrix_translate 		(GSvgMatrix *matrix, double tx, double ty);
void 	gsvg_matrix_init_rotate 	(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_rotate 		(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_init_skew_x 	(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_skew_x 		(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_init_skew_y 	(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_skew_y 		(GSvgMatrix *matrix, double radians);
void 	gsvg_matrix_multiply 		(GSvgMatrix *result, const GSvgMatrix *a, const GSvgMatrix *b);

#endif
