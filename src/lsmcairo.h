/* Lasem
 *
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
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_CAIRO_H
#define LSM_CAIRO_H

#include <lsmtypes.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

typedef struct _LsmFilterSurface LsmFilterSurface;

#define LSM_TYPE_FILTER_SURFACE (lsm_filter_surface_get_type())

GType lsm_filter_surface_get_type (void);

LsmFilterSurface * 	lsm_filter_surface_new 			(const char *name, unsigned int width, unsigned int height,
								 const LsmBox *subregion);
LsmFilterSurface * 	lsm_filter_surface_new_with_content 	(const char *name, cairo_surface_t *surface, const LsmBox *subregion);
LsmFilterSurface *	lsm_filter_surface_new_similar		(const char *name, LsmFilterSurface *model, const LsmBox *subregion);

void 			lsm_filter_surface_copy_data 		(LsmFilterSurface *to, LsmFilterSurface *from);
const char * 		lsm_filter_surface_get_name 		(LsmFilterSurface *surface);
cairo_surface_t *	lsm_filter_surface_get_cairo_surface	(LsmFilterSurface *surface);
const LsmBox *		lsm_filter_surface_get_subregion 	(LsmFilterSurface *surface);
void 			lsm_filter_surface_unref 		(LsmFilterSurface *filter_surface);
LsmFilterSurface *	lsm_filter_surface_ref 			(LsmFilterSurface *filter_surface);

void 			lsm_filter_surface_alpha 		(LsmFilterSurface *input, LsmFilterSurface *output);
void 			lsm_filter_surface_blend 		(LsmFilterSurface *input_1,
								 LsmFilterSurface *input_2,
								 LsmFilterSurface *output,
								 int blending_mode);
void 			lsm_filter_surface_fast_blur 		(LsmFilterSurface *input, LsmFilterSurface *output, double sx, double sy);
void 			lsm_filter_surface_flood 		(LsmFilterSurface *surface,
								 double red, double green, double blue, double opacity);
void 			lsm_filter_surface_offset 		(LsmFilterSurface *input, LsmFilterSurface *output,
								 int dx, int dy);
void 			lsm_filter_surface_merge 		(LsmFilterSurface *input, LsmFilterSurface *output);
void 			lsm_filter_surface_tile 		(LsmFilterSurface *input, LsmFilterSurface *output);

void 			lsm_cairo_box_user_to_device 		(cairo_t *cairo, LsmBox *to, const LsmBox *from);
void 			lsm_cairo_box_device_to_user 		(cairo_t *cairo, LsmBox *to, const LsmBox *from);
void 			lsm_cairo_set_source_pixbuf 		(cairo_t *cairo, const GdkPixbuf *pixbuf,
								 double pixbuf_x, double pixbuf_y);

G_END_DECLS

#endif
