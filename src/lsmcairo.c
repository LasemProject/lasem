/*
 * Copyright © 2004 Caleb Moore
 * Copyright © 2010 Emmanuel Pacaud
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
 * Authors:
 *	Caleb Moore <c.moore@student.unsw.edu.au>
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmcairo.h>
#include <math.h>

static void
box_blur (cairo_surface_t *in,
	  cairo_surface_t *output,
	  guchar * intermediate,
	  gint kw, gint kh, gint x1, gint y1, gint x2, gint y2)
{
	gint ch;
	gint x, y;
	gint rowstride, height, width;

	guchar *in_pixels;
	guchar *output_pixels;

	gint sum;

	height = cairo_image_surface_get_height (in);
	width = cairo_image_surface_get_width (in);

	in_pixels = cairo_image_surface_get_data (in);
	output_pixels = cairo_image_surface_get_data (output);

	rowstride = cairo_image_surface_get_stride (in);

	if (kw > x2 - x1)
		kw = x2 - x1;

	if (kh > y2 - y1)
		kh = y2 - y1;


	if (kw >= 1) {
		for (ch = 0; ch < 4; ch++) {
#if 0
			switch (ch) {
				case 0:
					if (!op.Rused)
						continue;
				case 1:
					if (!op.Gused)
						continue;
				case 2:
					if (!op.Bused)
						continue;
				case 3:
					if (!op.Aused)
						continue;
			}
#endif
			for (y = y1; y < y2; y++) {
				sum = 0;
				for (x = x1; x < x1 + kw; x++) {
					sum += (intermediate[x % kw] = in_pixels[4 * x + y * rowstride + ch]);

					if (x - kw / 2 >= 0 && x - kw / 2 < x2)
						output_pixels[4 * (x - kw / 2) + y * rowstride + ch] = sum / kw;
				}
				for (x = x1 + kw; x < x2; x++) {
					sum -= intermediate[x % kw];
					sum += (intermediate[x % kw] = in_pixels[4 * x + y * rowstride + ch]);
					output_pixels[4 * (x - kw / 2) + y * rowstride + ch] = sum / kw;
				}
				for (x = x2; x < x2 + kw; x++) {
					sum -= intermediate[x % kw];

					if (x - kw / 2 >= 0 && x - kw / 2 < x2)
						output_pixels[4 * (x - kw / 2) + y * rowstride + ch] = sum / kw;
				}
			}
		}
		in_pixels = output_pixels;
	}

	if (kh >= 1) {
		for (ch = 0; ch < 4; ch++) {
#if 0
			switch (ch) {
				case 0:
					if (!op.Rused)
						continue;
				case 1:
					if (!op.Gused)
						continue;
				case 2:
					if (!op.Bused)
						continue;
				case 3:
					if (!op.Aused)
						continue;
			}
#endif
			for (x = x1; x < x2; x++) {
				sum = 0;

				for (y = y1; y < y1 + kh; y++) {
					sum += (intermediate[y % kh] = in_pixels[4 * x + y * rowstride + ch]);

					if (y - kh / 2 >= 0 && y - kh / 2 < y2)
						output_pixels[4 * x + (y - kh / 2) * rowstride + ch] = sum / kh;
				}
				for (; y < y2; y++) {
					sum -= intermediate[y % kh];
					sum += (intermediate[y % kh] = in_pixels[4 * x + y * rowstride + ch]);
					output_pixels[4 * x + (y - kh / 2) * rowstride + ch] = sum / kh;
				}
				for (; y < y2 + kh; y++) {
					sum -= intermediate[y % kh];

					if (y - kh / 2 >= 0 && y - kh / 2 < y2)
						output_pixels[4 * x + (y - kh / 2) * rowstride + ch] = sum / kh;
				}
			}
		}
	}
}

void
lsm_cairo_fast_blur (cairo_surface_t * in, cairo_surface_t * output,
		     double sx, double sy, double x1, double y1, double x2, double y2)
{
	gint kx, ky;
	guchar *intermediate;

	g_return_if_fail (in != NULL);
	g_return_if_fail (output != NULL);

	g_return_if_fail (cairo_surface_get_type (in) == CAIRO_SURFACE_TYPE_IMAGE);
	g_return_if_fail (cairo_surface_get_type (output) == CAIRO_SURFACE_TYPE_IMAGE);

	g_return_if_fail (cairo_image_surface_get_format (in) == CAIRO_FORMAT_ARGB32);
	g_return_if_fail (cairo_image_surface_get_format (output) == CAIRO_FORMAT_ARGB32);

	kx = floor (sx * 3 * sqrt (2 * M_PI) / 4 + 0.5);
	ky = floor (sy * 3 * sqrt (2 * M_PI) / 4 + 0.5);

	if (kx < 1 && ky < 1)
		return;

	intermediate = g_new (guchar, MAX (kx, ky));

	box_blur (in, output, intermediate, kx, ky, x1, y1, x2, y2);
	box_blur (output, output, intermediate, kx, ky, x1, y1, x2, y2);
	box_blur (output, output, intermediate, kx, ky, x1, y1, x2, y2);

	g_free (intermediate);
}

