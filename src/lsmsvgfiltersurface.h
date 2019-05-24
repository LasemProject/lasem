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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_SVG_FILTER_SURFACE_H
#define LSM_SVG_FILTER_SURFACE_H

#include <lsmtypes.h>
#include <lsmsvgtraits.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

typedef struct _LsmSvgFilterSurface LsmSvgFilterSurface;

#define LSM_TYPE_FILTER_SURFACE (lsm_svg_filter_surface_get_type())

GType lsm_svg_filter_surface_get_type (void);

LsmSvgFilterSurface * 	lsm_svg_filter_surface_new 		(const char *name, unsigned int width, unsigned int height,
								 const LsmBox *subregion);
LsmSvgFilterSurface * 	lsm_svg_filter_surface_new_with_content	(const char *name, cairo_surface_t *surface, const LsmBox *subregion);
LsmSvgFilterSurface *	lsm_svg_filter_surface_new_similar	(const char *name, LsmSvgFilterSurface *model, const LsmBox *subregion);

const char * 		lsm_svg_filter_surface_get_name 	(LsmSvgFilterSurface *surface);
cairo_surface_t *	lsm_svg_filter_surface_get_cairo_surface(LsmSvgFilterSurface *surface);
const LsmBox *		lsm_svg_filter_surface_get_subregion 	(LsmSvgFilterSurface *surface);
void 			lsm_svg_filter_surface_unref 		(LsmSvgFilterSurface *filter_surface);
LsmSvgFilterSurface *	lsm_svg_filter_surface_ref 		(LsmSvgFilterSurface *filter_surface);

void 			lsm_svg_filter_surface_alpha 		(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output);
void 			lsm_svg_filter_surface_blend 		(LsmSvgFilterSurface *input_1,
								 LsmSvgFilterSurface *input_2,
								 LsmSvgFilterSurface *output,
								 int blending_mode);
void 			lsm_svg_filter_surface_blur 		(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
								 double sx, double sy);
void 			lsm_svg_filter_surface_flood 		(LsmSvgFilterSurface *surface,
								 double red, double green, double blue, double opacity);
void 			lsm_svg_filter_surface_offset 		(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
								 int dx, int dy);
void 			lsm_svg_filter_surface_merge 		(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output);
void 			lsm_svg_filter_surface_tile 		(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output);
void 			lsm_svg_filter_surface_color_matrix 	(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
								 LsmSvgColorFilterType type, unsigned n_values, const double *values);
void 			lsm_svg_filter_surface_convolve_matrix 	(LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
								 unsigned x_order, unsigned y_order,
								 unsigned n_values, const double *values,
								 double divisor, double bias, unsigned target_x, unsigned target_y,
								 LsmSvgEdgeMode edge_mode, gboolean preserve_alpha);
void			lsm_svg_filter_surface_displacement_map (LsmSvgFilterSurface *input_1,
								 LsmSvgFilterSurface *input_2,
								 LsmSvgFilterSurface *output,
								 double x_scale, double y_scale,
								 LsmSvgChannelSelector x_channel_selector,
								 LsmSvgChannelSelector y_channel_selector);
void			lsm_svg_filter_surface_image 		(LsmSvgFilterSurface *output, GdkPixbuf *pixbuf,
								 LsmSvgPreserveAspectRatio preserve_aspect_ratio);
void 			lsm_svg_filter_surface_morphology 	(LsmSvgFilterSurface *input_surface, LsmSvgFilterSurface *output_surface,
								 LsmSvgMorphologyOperator op, double rx, double ry);
void 			lsm_svg_filter_surface_specular_lighting(LsmSvgFilterSurface *output_surface,
								 double surface_scale, double specular_constant, double specular_exponent,
								 double dx, double dy);
void			lsm_svg_filter_surface_turbulence 	(LsmSvgFilterSurface *output_surface,
								 double base_frequency_x, double base_frequency_y,
								 int n_octaves, double seed,
								 LsmSvgStitchTiles stitch_tiles, LsmSvgTurbulenceType type,
								 const cairo_matrix_t *transform);

G_END_DECLS

#endif
