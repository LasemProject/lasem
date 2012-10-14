/* Lasem
 *
 * Copyright © 2004 Caleb Moore
 * Copyright © 2012 Emmanuel Pacaud
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
#include <lsmsvgenums.h>
#include <lsmutils.h>
#include <math.h>
#include <string.h>

struct _LsmFilterSurface {
	char *name;
	cairo_surface_t *surface;
	unsigned int x0;
	unsigned int y0;
	unsigned int x1;
	unsigned int y1;

	gint ref_count;
};

LsmFilterSurface *
lsm_filter_surface_new (const char *name, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
	LsmFilterSurface *filter_surface;
	cairo_surface_t *surface;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, x1 - x0, y1 - y0);

	filter_surface = lsm_filter_surface_new_with_content (name, x0, y0, surface);

	cairo_surface_destroy (surface);

	return filter_surface;
}

LsmFilterSurface *
lsm_filter_surface_new_with_content (const char *name, unsigned int x0, unsigned int y0, cairo_surface_t *surface)
{
	LsmFilterSurface *filter_surface;

	g_return_val_if_fail (surface != NULL, NULL);
	g_return_val_if_fail (cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE, NULL);
	g_return_val_if_fail (cairo_image_surface_get_format (surface) == CAIRO_FORMAT_ARGB32, NULL);

	cairo_surface_reference (surface);

	filter_surface = g_new (LsmFilterSurface, 1);
	filter_surface->name = g_strdup (name);
	filter_surface->x0 = x0;
	filter_surface->y0 = y0;
	filter_surface->x1 = x0 + cairo_image_surface_get_width (surface);
	filter_surface->y1 = y0 + cairo_image_surface_get_height (surface);
	filter_surface->surface  = surface;
	filter_surface->ref_count = 1;

	return filter_surface;
}

LsmFilterSurface *
lsm_filter_surface_new_similar (const char *name, LsmFilterSurface *model)
{
	g_return_val_if_fail (model != NULL, NULL);

	return lsm_filter_surface_new (name, model->x0, model->y0, model->x1, model->y1);
}

const char *
lsm_filter_surface_get_name (LsmFilterSurface *surface)
{
	g_return_val_if_fail (surface != NULL, NULL);

	return surface->name;
}

cairo_surface_t *
lsm_filter_surface_get_cairo_surface (LsmFilterSurface *surface)
{
	g_return_val_if_fail (surface != NULL, NULL);

	return surface->surface;
}

LsmFilterSurface *
lsm_filter_surface_ref (LsmFilterSurface *filter_surface)
{
	g_return_val_if_fail (filter_surface != NULL, NULL);

	g_atomic_int_inc (&filter_surface->ref_count);

	return filter_surface;
}

void
lsm_filter_surface_unref (LsmFilterSurface *filter_surface)
{
	g_return_if_fail (filter_surface != NULL);
	g_return_if_fail (filter_surface->ref_count > 0);

	if (g_atomic_int_dec_and_test (&filter_surface->ref_count)) {
		cairo_surface_destroy (filter_surface->surface);
		g_free (filter_surface->name);
		g_free (filter_surface);
	}
}

G_DEFINE_BOXED_TYPE (LsmFilterSurface, lsm_filter_surface, lsm_filter_surface_ref, lsm_filter_surface_unref)

static void
box_blur (LsmFilterSurface *input,
	  LsmFilterSurface *output,
	  guchar * intermediate,
	  gint kw, gint kh)
{
	gint ch;
	gint x, y;
	gint rowstride;
	gint x1, y1, x2, y2;
	guchar *in_pixels;
	guchar *output_pixels;
	gint sum;

	in_pixels = cairo_image_surface_get_data (input->surface);
	output_pixels = cairo_image_surface_get_data (output->surface);

	rowstride = cairo_image_surface_get_stride (input->surface);

	x1 = input->x0;
	y1 = input->y0;
	x2 = input->x1;
	y2 = input->y1;

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
lsm_filter_surface_fast_blur (LsmFilterSurface *input,
			      LsmFilterSurface *output,
			      double sx, double sy)
{
	gint kx, ky;
	guchar *intermediate;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo_surface_flush (input->surface);

	kx = floor (sx * 3 * sqrt (2 * M_PI) / 4 + 0.5);
	ky = floor (sy * 3 * sqrt (2 * M_PI) / 4 + 0.5);

	if (kx < 1 && ky < 1)
		return;

	intermediate = g_new (guchar, MAX (kx, ky));

	box_blur (input, output, intermediate, kx, ky);
	box_blur (output, output, intermediate, kx, ky);
	box_blur (output, output, intermediate, kx, ky);

	g_free (intermediate);

	cairo_surface_mark_dirty (output->surface);
}

void
lsm_filter_surface_flood (LsmFilterSurface *surface, guint32 color, double opacity)
{
	guint8 int_opacity;
	char pixcolour[4];
	int i, x, y;
	int stride;
	unsigned char *pixels;
	int channelmap[4] = {0, 1, 2, 3};

	g_return_if_fail (surface != NULL);

	cairo_surface_flush (surface->surface);

	stride = cairo_image_surface_get_stride (surface->surface);
	pixels = cairo_image_surface_get_data (surface->surface);

	int_opacity = (double) (0.5 + (double) opacity * 255.0); 

	for (i = 0; i < 3; i++)
		pixcolour[i] = (int) (((unsigned char *)
				       (&color))[2 - i]) * int_opacity / 255;
	pixcolour[3] = int_opacity;

	for (y = surface->y0; y < surface->y1; y++)
		for (x = surface->x0; x < surface->x1; x++)
			for (i = 0; i < 4; i++) {
				pixels[4 * x + y * stride + channelmap[i]] = pixcolour[i];
			}

	cairo_surface_mark_dirty (surface->surface);
}

void
lsm_filter_surface_blend (LsmFilterSurface *input_1,
			  LsmFilterSurface *input_2,
			  LsmFilterSurface *output,
			  int blending_mode)
{
	cairo_t *cairo;
	cairo_operator_t op;

	g_return_if_fail (input_1 != NULL);
	g_return_if_fail (input_2 != NULL);
	g_return_if_fail (output != NULL);

	cairo_surface_flush (input_1->surface);
	cairo_surface_flush (input_2->surface);

	switch (blending_mode) {
		case LSM_SVG_BLENDING_MODE_MULTIPLY:
			op = CAIRO_OPERATOR_MULTIPLY;
			break;
		case LSM_SVG_BLENDING_MODE_SCREEN:
			op = CAIRO_OPERATOR_SCREEN;
			break;
		case LSM_SVG_BLENDING_MODE_DARKEN:
			op = CAIRO_OPERATOR_DARKEN;
			break;
		case LSM_SVG_BLENDING_MODE_LIGHTEN:
			op = CAIRO_OPERATOR_LIGHTEN;
			break;
		case LSM_SVG_BLENDING_MODE_IN:
			op = CAIRO_OPERATOR_IN;
			break;
		case LSM_SVG_BLENDING_MODE_OUT:
			op = CAIRO_OPERATOR_OUT;
			break;
		case LSM_SVG_BLENDING_MODE_ATOP:
			op = CAIRO_OPERATOR_ATOP;
			break;
		case LSM_SVG_BLENDING_MODE_XOR:
			op = CAIRO_OPERATOR_XOR;
			break;
		default:
			op = CAIRO_OPERATOR_OVER;
			break;
	}

	cairo = cairo_create (output->surface);

	cairo_set_source_surface (cairo, input_2->surface, 0, 0);
	cairo_paint (cairo);

	cairo_set_source_surface (cairo, input_1->surface, 0, 0);
	cairo_set_operator (cairo, op);
	cairo_paint (cairo);

	cairo_destroy (cairo);
}

void
lsm_filter_surface_offset (LsmFilterSurface *input,
			   LsmFilterSurface *output,
			   int dx, int dy)
{
	cairo_t *cairo;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo_surface_flush (input->surface);

	cairo = cairo_create (output->surface);
	cairo_set_source_surface (cairo, input->surface, dx, dy);
	cairo_paint (cairo);
	cairo_destroy (cairo);
}

/**
 * lsm_cairo_set_source_pixbuf:
 * @cr: a cairo context
 * @pixbuf: a #GdkPixbuf
 * @pixbuf_x: X coordinate of location to place upper left corner of @pixbuf
 * @pixbuf_y: Y coordinate of location to place upper left corner of @pixbuf
 *
 * Sets the given pixbuf as the source pattern for @cr.
 *
 * The pattern has an extend mode of %CAIRO_EXTEND_NONE and is aligned
 * so that the origin of @pixbuf is @pixbuf_x, @pixbuf_y.
 *
 * Since: 0.4
 */

void
lsm_cairo_set_source_pixbuf (cairo_t         *cr,
                             const GdkPixbuf *pixbuf,
                             gdouble          pixbuf_x,
                             gdouble          pixbuf_y)
{
	gint width = gdk_pixbuf_get_width (pixbuf);
	gint height = gdk_pixbuf_get_height (pixbuf);
	guchar *gdk_pixels = gdk_pixbuf_get_pixels (pixbuf);
	int gdk_rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	int n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	int cairo_stride;
	guchar *cairo_pixels;
	cairo_format_t format;
	cairo_surface_t *surface;
	static const cairo_user_data_key_t key;
	int j;

	if (n_channels == 3)
		format = CAIRO_FORMAT_RGB24;
	else
		format = CAIRO_FORMAT_ARGB32;

	cairo_stride = cairo_format_stride_for_width (format, width);
	cairo_pixels = g_malloc (height * cairo_stride);
	surface = cairo_image_surface_create_for_data ((unsigned char *)cairo_pixels,
						       format,
						       width, height, cairo_stride);

	cairo_surface_set_user_data (surface, &key,
				     cairo_pixels, (cairo_destroy_func_t)g_free);

	for (j = height; j; j--)
	{
		guchar *p = gdk_pixels;
		guchar *q = cairo_pixels;

		if (n_channels == 3)
		{
			guchar *end = p + 3 * width;

			while (p < end)
			{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
				q[0] = p[2];
				q[1] = p[1];
				q[2] = p[0];
#else
				q[1] = p[0];
				q[2] = p[1];
				q[3] = p[2];
#endif
				p += 3;
				q += 4;
			}
		}
		else
		{
			guchar *end = p + 4 * width;
			guint t1,t2,t3;

#define MULT(d,c,a,t) G_STMT_START { t = c * a + 0x7f; d = ((t >> 8) + t) >> 8; } G_STMT_END

			while (p < end)
			{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
				MULT(q[0], p[2], p[3], t1);
				MULT(q[1], p[1], p[3], t2);
				MULT(q[2], p[0], p[3], t3);
				q[3] = p[3];
#else
				q[0] = p[3];
				MULT(q[1], p[0], p[3], t1);
				MULT(q[2], p[1], p[3], t2);
				MULT(q[3], p[2], p[3], t3);
#endif

				p += 4;
				q += 4;
			}

#undef MULT
		}

		gdk_pixels += gdk_rowstride;
		cairo_pixels += cairo_stride;
	}

	cairo_set_source_surface (cr, surface, pixbuf_x, pixbuf_y);
	cairo_surface_destroy (surface);
}

