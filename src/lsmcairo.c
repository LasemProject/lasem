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

void
lsm_cairo_box_user_to_device (cairo_t *cairo, LsmBox *to, const LsmBox *from)
{
	if (to == NULL)
		return;

	if (from == NULL || cairo == NULL) {
		to->x = 0;
		to->y = 0;
		to->width = 0;
		to->height = 0;
	}

	*to = *from;

	cairo_user_to_device (cairo, &to->x, &to->y);
	cairo_user_to_device_distance (cairo, &to->width, &to->height);
}

void
lsm_cairo_box_device_to_user (cairo_t *cairo, LsmBox *to, const LsmBox *from)
{
	if (to == NULL)
		return;

	if (from == NULL || cairo == NULL) {
		to->x = 0;
		to->y = 0;
		to->width = 0;
		to->height = 0;
	}

	*to = *from;

	cairo_device_to_user (cairo, &to->x, &to->y);
	cairo_device_to_user_distance (cairo, &to->width, &to->height);
}

/**
 * lsm_cairo_set_source_pixbuf:
 * @cairo: a cairo context
 * @pixbuf: a #GdkPixbuf
 * @pixbuf_x: X coordinate of location to place upper left corner of @pixbuf
 * @pixbuf_y: Y coordinate of location to place upper left corner of @pixbuf
 *
 * Sets the given pixbuf as the source pattern for @cairo.
 *
 * The pattern has an extend mode of %CAIRO_EXTEND_NONE and is aligned
 * so that the origin of @pixbuf is @pixbuf_x, @pixbuf_y.
 *
 * Since: 0.4
 */

void
lsm_cairo_set_source_pixbuf (cairo_t *cairo,
                             const GdkPixbuf *pixbuf,
                             double pixbuf_x,
                             double pixbuf_y)
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

	cairo_set_source_surface (cairo, surface, pixbuf_x, pixbuf_y);
	cairo_surface_destroy (surface);
}
