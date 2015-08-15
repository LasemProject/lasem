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
 *	Mario Klingemann <mario@quasimondo.com>
 *
 * Image manipulation routines in this file are an adaptation of the code
 * of librsvg: https://git.gnome.org/browse/librsvg
 */

#include <lsmsvgfiltersurface.h>
#include <lsmsvgenums.h>
#include <lsmutils.h>
#include <math.h>
#include <string.h>

static const int channelmap[4] = {2, 1, 0, 3};

struct _LsmSvgFilterSurface {
	char *name;
	cairo_surface_t *surface;
	LsmBox subregion;

	gint ref_count;
};

LsmSvgFilterSurface *
lsm_svg_filter_surface_new (const char *name, unsigned int width, unsigned int height, const LsmBox *subregion)
{
	LsmSvgFilterSurface *filter_surface;
	cairo_surface_t *surface;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);

	filter_surface = lsm_svg_filter_surface_new_with_content (name, surface, subregion);

	cairo_surface_destroy (surface);

	return filter_surface;
}

LsmSvgFilterSurface *
lsm_svg_filter_surface_new_with_content (const char *name, cairo_surface_t *surface, const LsmBox *subregion)
{
	LsmSvgFilterSurface *filter_surface;
	LsmBox null_subregion = {0, 0, 0, 0};

	if (surface == NULL ||
	    cairo_surface_get_type (surface) != CAIRO_SURFACE_TYPE_IMAGE ||
	    cairo_image_surface_get_format (surface) != CAIRO_FORMAT_ARGB32) {
		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 0, 0);
		subregion = &null_subregion;
	} else if (subregion == NULL)
		subregion = &null_subregion;

	cairo_surface_reference (surface);

	filter_surface = g_new (LsmSvgFilterSurface, 1);
	filter_surface->name = g_strdup (name);
	filter_surface->subregion = *subregion;
	filter_surface->surface  = surface;
	filter_surface->ref_count = 1;

	return filter_surface;
}

LsmSvgFilterSurface *
lsm_svg_filter_surface_new_similar (const char *name, LsmSvgFilterSurface *model, const LsmBox *subregion)
{
	if (model == NULL)
		return lsm_svg_filter_surface_new (name, 0, 0, subregion);

	return lsm_svg_filter_surface_new (name,
				       cairo_image_surface_get_width (model->surface),
				       cairo_image_surface_get_height (model->surface),
				       subregion != NULL ? subregion : &model->subregion);
}

const char *
lsm_svg_filter_surface_get_name (LsmSvgFilterSurface *surface)
{
	g_return_val_if_fail (surface != NULL, NULL);

	return surface->name;
}

cairo_surface_t *
lsm_svg_filter_surface_get_cairo_surface (LsmSvgFilterSurface *surface)
{
	g_return_val_if_fail (surface != NULL, NULL);

	return surface->surface;
}

const LsmBox *
lsm_svg_filter_surface_get_subregion (LsmSvgFilterSurface *surface)
{
	static const LsmBox null_subregion = {0, 0, 0, 0};

	if (surface == NULL)
		return &null_subregion;

	return &surface->subregion;
}

LsmSvgFilterSurface *
lsm_svg_filter_surface_ref (LsmSvgFilterSurface *filter_surface)
{
	g_return_val_if_fail (filter_surface != NULL, NULL);

	g_atomic_int_inc (&filter_surface->ref_count);

	return filter_surface;
}

void
lsm_svg_filter_surface_unref (LsmSvgFilterSurface *filter_surface)
{
	g_return_if_fail (filter_surface != NULL);
	g_return_if_fail (filter_surface->ref_count > 0);

	if (g_atomic_int_dec_and_test (&filter_surface->ref_count)) {
		cairo_surface_destroy (filter_surface->surface);
		g_free (filter_surface->name);
		g_free (filter_surface);
	}
}

G_DEFINE_BOXED_TYPE (LsmSvgFilterSurface, lsm_svg_filter_surface, lsm_svg_filter_surface_ref, lsm_svg_filter_surface_unref)

/*
 * The stack blur algorithm was invented by Mario Klingemann <mario@quasimondo.com>
 * http://incubator.quasimondo.com/processing/fast_blur_deluxe.php
 *
 * Compared to Mario's original source code, the lookup table is removed as the benefit
 * doesn't worth the memory usage in case of large radiuses. Also, the following code adds
 * alpha channel support and different radius for vertical and horizontal directions.
 */

static void
stack_blur (cairo_surface_t *input, cairo_surface_t *output, int rx, int ry)
{
	guint32 *input_pixels;
	guint32 *output_pixels;
	int w, h, wm, hm, wh, div;
	int *r, *g, *b, *a;

	int rsum,gsum,bsum,asum,x,y,i,p,yp,yi,yw;
	int *vmin;
	int divsum;

	yw=yi=0;

	int *stack;
	int stackpointer;
	int stackstart;
	int *sir;
	int rbs;
	int r1;
	int routsum,goutsum,boutsum, aoutsum;
	int rinsum,ginsum,binsum, ainsum;
	int rowstride;

	g_return_if_fail (rx > 0 || ry > 0);

	input_pixels = (guint32 *) cairo_image_surface_get_data (input);
	output_pixels = (guint32 *) cairo_image_surface_get_data (output);
	rowstride = cairo_image_surface_get_stride (input);
	w = cairo_image_surface_get_width (input);
	h = cairo_image_surface_get_height (input);

	g_return_if_fail (cairo_image_surface_get_width (output) == w);
	g_return_if_fail (cairo_image_surface_get_height (output) == h);
	g_return_if_fail (cairo_image_surface_get_stride (output) == rowstride);

	wm = w - 1;
	hm = h - 1;
	wh = w * h;
	
	r = g_new (int, wh);
	g = g_new (int, wh);
	b = g_new (int, wh);
	a = g_new (int, wh);

	div = 2 * rx + 1;
	divsum = (div + 1) >> 1; 
	divsum *= divsum;
	stack = g_new (int, div * 4);
	r1 = rx + 1;

	vmin = g_new (int, w);
	for (y =0; y < h; y++) {
		rinsum=ginsum=binsum=ainsum=routsum=goutsum=boutsum=aoutsum=rsum=gsum=bsum=asum=0;
		yi = y * rowstride / 4;

		for (i = -rx; i <= rx; i++) {
			p = input_pixels [yi + MIN (wm, MAX (i, 0))];
			sir = &stack [4 * (i + rx)];
			sir[0]=(p & 0x00ff0000)>>16;
			sir[1]=(p & 0x0000ff00)>>8;
			sir[2]=(p & 0x000000ff);
			sir[3]=(p & 0xff0000ff)>>24;
			rbs= r1 - ABS (i);
			rsum+=sir[0]*rbs;
			gsum+=sir[1]*rbs;
			bsum+=sir[2]*rbs;
			asum+=sir[3]*rbs;
			if (i>0){
				rinsum+=sir[0];
				ginsum+=sir[1];
				binsum+=sir[2];
				ainsum+=sir[3];
			} else {
				routsum+=sir[0];
				goutsum+=sir[1];
				boutsum+=sir[2];
				aoutsum+=sir[3];
			}
		}
		stackpointer=rx;

		for (x=0;x<w;x++){

			r[yi] = rsum / divsum;
			g[yi] = gsum / divsum;
			b[yi] = bsum / divsum;
			a[yi] = asum / divsum;
			rsum-=routsum;
			gsum-=goutsum;
			bsum-=boutsum;
			asum-=aoutsum;

			stackstart=stackpointer-rx+div;
			sir = &stack [4 * (stackstart % div)];

			routsum-=sir[0];
			goutsum-=sir[1];
			boutsum-=sir[2];
			aoutsum-=sir[3];

			if(y==0){
				vmin[x]=MIN (x + rx + 1, wm);
			}
			p = input_pixels [yw + vmin[x]];

			sir[0]=(p & 0x00ff0000)>>16;
			sir[1]=(p & 0x0000ff00)>>8;
			sir[2]=(p & 0x000000ff);
			sir[3]=(p & 0xff000000)>>24;

			rinsum+=sir[0];
			ginsum+=sir[1];
			binsum+=sir[2];
			ainsum+=sir[3];

			rsum+=rinsum;
			gsum+=ginsum;
			bsum+=binsum;
			asum+=ainsum;

			stackpointer = (stackpointer + 1) % div;
			sir = &stack [4 * ((stackpointer) % div)];

			routsum+=sir[0];
			goutsum+=sir[1];
			boutsum+=sir[2];
			aoutsum+=sir[3];

			rinsum-=sir[0];
			ginsum-=sir[1];
			binsum-=sir[2];
			ainsum-=sir[3];

			yi++;
		}
		yw+=w;
	}
	g_free (vmin);
	g_free (stack);

	div = 2 * ry + 1;
	divsum = (div + 1) >> 1; 
	divsum *= divsum;

	stack = g_new0 (int, div * 4);
	r1 = ry + 1;

	vmin = g_new0 (int, h);
	for (x=0;x<w;x++){
		rinsum=ginsum=binsum=ainsum=routsum=goutsum=boutsum=aoutsum=rsum=gsum=bsum=asum=0;
		yp=-ry*w;
		for(i=-ry;i<=ry;i++){
			yi= MAX(0,yp)+x;

			sir = &stack [4 * (i + ry)];

			sir[0]=r[yi];
			sir[1]=g[yi];
			sir[2]=b[yi];
			sir[3]=a[yi];

			rbs=r1 - ABS(i);

			rsum+=r[yi]*rbs;
			gsum+=g[yi]*rbs;
			bsum+=b[yi]*rbs;
			asum+=a[yi]*rbs;

			if (i>0){
				rinsum+=sir[0];
				ginsum+=sir[1];
				binsum+=sir[2];
				ainsum+=sir[3];
			} else {
				routsum+=sir[0];
				goutsum+=sir[1];
				boutsum+=sir[2];
				aoutsum+=sir[3];
			}

			if(i<hm){
				yp+=w;
			}
		}
		yi=x;
		stackpointer=ry;
		for (y=0;y<h;y++){
			output_pixels [yi] = 
				((asum / divsum) << 24) |
				((rsum / divsum) << 16) |
				((gsum / divsum) << 8) |
				(bsum / divsum);
			rsum-=routsum;
			gsum-=goutsum;
			bsum-=boutsum;
			asum-=aoutsum;

			stackstart=stackpointer-ry+div;
			sir = &stack [4 * (stackstart % div)];

			routsum-=sir[0];
			goutsum-=sir[1];
			boutsum-=sir[2];
			aoutsum-=sir[3];

			if(x==0) {
				vmin[y] = MIN(y+r1,hm)*w;
			}
			p=x+vmin[y];

			sir[0]=r[p];
			sir[1]=g[p];
			sir[2]=b[p];
			sir[3]=a[p];

			rinsum+=sir[0];
			ginsum+=sir[1];
			binsum+=sir[2];
			ainsum+=sir[3];

			rsum+=rinsum;
			gsum+=ginsum;
			bsum+=binsum;
			asum+=ainsum;

			stackpointer=(stackpointer+1)%div;
			sir = &stack[4 * stackpointer];

			routsum+=sir[0];
			goutsum+=sir[1];
			boutsum+=sir[2];
			aoutsum+=sir[3];

			rinsum-=sir[0];
			ginsum-=sir[1];
			binsum-=sir[2];
			ainsum-=sir[3];

			yi+= rowstride / 4;
		}
	}
	g_free (vmin);
	g_free (stack);

	g_free (r);
	g_free (g);
	g_free (b);
	g_free (a);
}

void
lsm_svg_filter_surface_blur (LsmSvgFilterSurface *input,
			     LsmSvgFilterSurface *output,
			     double sx, double sy)
{
	int kx, ky;
	int width, height;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo_surface_flush (input->surface);

	kx = floor (sx * 3 * sqrt (2 * M_PI) / 4 + 0.5);
	ky = floor (sy * 3 * sqrt (2 * M_PI) / 4 + 0.5);

	width = cairo_image_surface_get_width (input->surface);
	height = cairo_image_surface_get_height (input->surface);

	if (width != cairo_image_surface_get_width (output->surface) ||
	    height != cairo_image_surface_get_height (output->surface))
		return;

	if (width < 1 || height < 1)
		return;

	if (kx > 1 || ky > 1) {
		int x1, y1, x2, y2;
		cairo_surface_t *blur_surface;
		gboolean do_clip = FALSE;

		x1 = output->subregion.x - kx;
		y1 = output->subregion.y - ky;
		x2 = output->subregion.width + output->subregion.x + kx;
		y2 = output->subregion.height + output->subregion.y + ky;
		x1 = CLAMP (x1, 0, width);
		y1 = CLAMP (y1, 0, height);
		x2 = CLAMP (x2, x1, width);
		y2 = CLAMP (y2, y1, height);


		if (input->subregion.x < output->subregion.x ||
		    input->subregion.y < output->subregion.y ||
		    input->subregion.width > output->subregion.width ||
		    input->subregion.height > output->subregion.height) {
			do_clip = TRUE;	
			blur_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
		} else
			blur_surface = output->surface;

		stack_blur (input->surface, blur_surface, kx, ky);

		cairo_surface_mark_dirty (blur_surface);

		if (do_clip) {
			cairo_t *cairo;

			cairo = cairo_create (output->surface);
			cairo_rectangle (cairo,
					 output->subregion.x, output->subregion.y,
					 output->subregion.width, output->subregion.height);
			cairo_clip (cairo);
			cairo_set_source_surface (cairo, blur_surface, 0, 0);
			cairo_paint (cairo);
			cairo_destroy (cairo);

			cairo_surface_destroy (blur_surface);
		}
	} else {
		cairo_t *cairo;

		cairo = cairo_create (output->surface);
		cairo_rectangle (cairo,
				 output->subregion.x, output->subregion.y,
				 output->subregion.width, output->subregion.height);
		cairo_clip (cairo);
		cairo_set_source_surface (cairo, input->surface, 0, 0);
		cairo_paint (cairo);
		cairo_destroy (cairo);
	}
}

void
lsm_svg_filter_surface_flood (LsmSvgFilterSurface *surface,
			      double red,
			      double green,
			      double blue,
			      double opacity)
{
	cairo_t *cairo;

	g_return_if_fail (surface != NULL);

	cairo = cairo_create (surface->surface);
	cairo_rectangle (cairo, surface->subregion.x, surface->subregion.y, surface->subregion.width, surface->subregion.height);
	cairo_clip (cairo);
	cairo_set_source_rgba (cairo, red, green, blue, opacity);
	cairo_paint (cairo);
}

void
lsm_svg_filter_surface_blend (LsmSvgFilterSurface *input_1,
			      LsmSvgFilterSurface *input_2,
			      LsmSvgFilterSurface *output,
			      int blending_mode)
{
	cairo_t *cairo;
	cairo_operator_t op;

	g_return_if_fail (input_1 != NULL);
	g_return_if_fail (input_2 != NULL);
	g_return_if_fail (output != NULL);

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
	cairo_rectangle (cairo, output->subregion.x, output->subregion.y, output->subregion.width, output->subregion.height);
	cairo_clip (cairo);
	cairo_set_source_surface (cairo, input_2->surface, 0, 0);
	cairo_paint (cairo);
	cairo_set_source_surface (cairo, input_1->surface, 0, 0);
	cairo_set_operator (cairo, op);
	cairo_paint (cairo);
	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_offset (LsmSvgFilterSurface *input,
			       LsmSvgFilterSurface *output,
			       int dx, int dy)
{
	cairo_t *cairo;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo = cairo_create (output->surface);
	cairo_rectangle (cairo, output->subregion.x, output->subregion.y, output->subregion.width, output->subregion.height);
	cairo_clip (cairo);
	cairo_set_source_surface (cairo, input->surface, dx, dy);
	cairo_paint (cairo);
	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_merge (LsmSvgFilterSurface *input,
			      LsmSvgFilterSurface *output)
{
	cairo_t *cairo;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo = cairo_create (output->surface);
	cairo_rectangle (cairo, output->subregion.x, output->subregion.y, output->subregion.width, output->subregion.height);
	cairo_clip (cairo);
	cairo_set_source_surface (cairo, input->surface, 0, 0);
	cairo_paint (cairo);
	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_tile (LsmSvgFilterSurface *input, LsmSvgFilterSurface *output)
{
	cairo_t *cairo;
	cairo_surface_t *surface;
	cairo_pattern_t *pattern;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, input->subregion.width, input->subregion.height);
	cairo = cairo_create (surface);
	cairo_set_source_surface (cairo, input->surface, -input->subregion.x, -input->subregion.y);
	cairo_paint (cairo);
	cairo_destroy (cairo);

	cairo = cairo_create (output->surface);
	cairo_rectangle (cairo, output->subregion.x, output->subregion.y, output->subregion.width, output->subregion.height);
	cairo_clip (cairo);
	cairo_set_source_surface (cairo, surface, input->subregion.x, input->subregion.y);
	pattern = cairo_get_source (cairo);
	cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
	cairo_paint (cairo);
	cairo_destroy (cairo);
	cairo_surface_destroy (surface);
}

void
lsm_svg_filter_surface_alpha (LsmSvgFilterSurface *input, LsmSvgFilterSurface *output)
{
	cairo_t *cairo;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	cairo = cairo_create (output->surface);
	cairo_set_source_rgb (cairo, 0, 0, 0);
	cairo_mask_surface (cairo, input->surface, 0, 0);

	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_color_matrix (LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
				     LsmSvgColorFilterType type, unsigned n_values, const double *values)
{
	cairo_t *cairo;
	unsigned i;
	int matrix[20];
	double cosval;
	double sinval;
	int ch;
	gint x, y, x1, x2, y1, y2;
	gint width, height;
	gint rowstride;
	gint sum;
	guchar *in_pixels;
	guchar *output_pixels;
	double setting;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);
	g_return_if_fail (values != NULL || n_values < 1);

	width = cairo_image_surface_get_width (input->surface);
	height = cairo_image_surface_get_height (input->surface);

	if (width != cairo_image_surface_get_width (output->surface) ||
	    height != cairo_image_surface_get_height (output->surface))
		return;

	if (height < 1 || width < 1)
		return;

	memset (matrix, 0, sizeof (*matrix) * G_N_ELEMENTS (matrix));

	switch (type) {
		case LSM_SVG_COLOR_FILTER_TYPE_MATRIX:
			for (i = 0; i < G_N_ELEMENTS (matrix) && i < n_values; i++)
				matrix[i] = values[i] * 255.0;
			break;
		case LSM_SVG_COLOR_FILTER_TYPE_SATURATE:
			if (n_values > 0)
				setting = values[0];
			else
				setting = 1.0;

			matrix[0] =  255.0 * (0.213 + 0.787 * setting);
			matrix[1] =  255.0 * (0.715 - 0.715 * setting);
			matrix[2] =  255.0 * (0.072 - 0.072 * setting);
			matrix[5] =  255.0 * (0.213 - 0.213 * setting);
			matrix[6] =  255.0 * (0.715 + 0.285 * setting);
			matrix[7] =  255.0 * (0.072 - 0.072 * setting);
			matrix[10] = 255.0 * (0.213 - 0.213 * setting);
			matrix[11] = 255.0 * (0.715 - 0.715 * setting);
			matrix[12] = 255.0 * (0.072 + 0.928 * setting);
			matrix[18] = 255;
			break;
		case LSM_SVG_COLOR_FILTER_TYPE_HUE_ROTATE:
			if (n_values > 0)
				setting = values[0];
			else
				setting = 0.0;

			cosval = cos (setting);
			sinval = sin (setting);

			matrix[0] = (0.213 + cosval * 0.787 + sinval * -0.213) * 255.;
			matrix[1] = (0.715 + cosval * -0.715 + sinval * -0.715) * 255.;
			matrix[2] = (0.072 + cosval * -0.072 + sinval * 0.928) * 255.;
			matrix[5] = (0.213 + cosval * -0.213 + sinval * 0.143) * 255.;
			matrix[6] = (0.715 + cosval * 0.285 + sinval * 0.140) * 255.;
			matrix[7] = (0.072 + cosval * -0.072 + sinval * -0.283) * 255.;
			matrix[10] = (0.213 + cosval * -0.213 + sinval * -0.787) * 255.;
			matrix[11] = (0.715 + cosval * -0.715 + sinval * 0.715) * 255.;
			matrix[12] = (0.072 + cosval * 0.928 + sinval * 0.072) * 255.;
			matrix[18] = 255;
			break;
		case LSM_SVG_COLOR_FILTER_TYPE_LUMINANCE_TO_ALPHA:
			matrix[15] = 0.2125 * 255.;
			matrix[16] = 0.7154 * 255.;
			matrix[17] = 0.0721 * 255.;
			break;
		default:
			return;
	}

	cairo_surface_flush (input->surface);
	cairo = cairo_create (output->surface);

	in_pixels = cairo_image_surface_get_data (input->surface);
	output_pixels = cairo_image_surface_get_data (output->surface);
	rowstride = cairo_image_surface_get_stride (input->surface);

	x1 = CLAMP (input->subregion.x, 0, width);
	x2 = CLAMP (input->subregion.x + input->subregion.width, 0, width);
	y1 = CLAMP (input->subregion.y, 0, height);
	y2 = CLAMP (input->subregion.y + input->subregion.height, 0, height);

	for (y = y1; y < y2; y++)
		for (x = x1; x < x2; x++) {
			int umch;
			int alpha = in_pixels[4 * x + y * rowstride + channelmap[3]];
			if (!alpha)
				for (umch = 0; umch < 4; umch++) {
					sum = matrix[umch * 5 + 4];
					if (sum > 255)
						sum = 255;
					if (sum < 0)
						sum = 0;
					output_pixels[4 * x + y * rowstride + channelmap[umch]] = sum;
				} else
					for (umch = 0; umch < 4; umch++) {
						int umi;
						ch = channelmap[umch];
						sum = 0;
						for (umi = 0; umi < 4; umi++) {
							i = channelmap[umi];
							if (umi != 3)
								sum += matrix[umch * 5 + umi] *
									in_pixels[4 * x + y * rowstride + i] / alpha;
							else
								sum += matrix[umch * 5 + umi] *
									in_pixels[4 * x + y * rowstride + i] / 255;
						}
						sum += matrix[umch * 5 + 4];



						if (sum > 255)
							sum = 255;
						if (sum < 0)
							sum = 0;

						output_pixels[4 * x + y * rowstride + ch] = sum;
					}
				for (umch = 0; umch < 3; umch++) {
					ch = channelmap[umch];
					output_pixels[4 * x + y * rowstride + ch] =
						output_pixels[4 * x + y * rowstride + ch] *
						output_pixels[4 * x + y * rowstride + channelmap[3]] / 255;
				}
		}

	cairo_surface_mark_dirty (output->surface);

	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_convolve_matrix (LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
					 unsigned order_x, unsigned order_y, unsigned n_values, const double *values,
					 double divisor, double bias, unsigned target_x, unsigned target_y,
					 LsmSvgEdgeMode edge_mode, gboolean preserve_alpha)
{
	cairo_t *cairo;
	int ch;
	gint x, y, x1, x2, y1, y2;
	gint width, height;
	gint rowstride;
	guchar *in_pixels;
	guchar *output_pixels;
	double kval, sum;
	guchar sval;
	int sx, sy, kx, ky;
	/* TODO d */
	double dx = 1.0, dy = 1.0;
	int umch, i, j;
	gint tempresult;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);
	g_return_if_fail (values != NULL || n_values < 1);

	if (divisor <= 0.0)
		return;

	width = cairo_image_surface_get_width (input->surface);
	height = cairo_image_surface_get_height (input->surface);

	if (width != cairo_image_surface_get_width (output->surface) ||
	    height != cairo_image_surface_get_height (output->surface))
		return;

	if (height < 1 || width < 1)
		return;

	if (order_y * order_x != n_values)
		return;

	if (target_x > order_x || target_y > order_y)
		return;

	x1 = CLAMP (input->subregion.x, 0, width);
	x2 = CLAMP (input->subregion.x + input->subregion.width, 0, width);
	y1 = CLAMP (input->subregion.y, 0, height);
	y2 = CLAMP (input->subregion.y + input->subregion.height, 0, height);

	cairo_surface_flush (input->surface);
	cairo = cairo_create (output->surface);

	in_pixels = cairo_image_surface_get_data (input->surface);
	output_pixels = cairo_image_surface_get_data (output->surface);
	rowstride = cairo_image_surface_get_stride (input->surface);

	for (y = y1; y < y2; y++)
		for (x = x1; x < x2; x++) {
			for (umch = 0; umch < 3 + !preserve_alpha; umch++) {
				ch = channelmap[umch];
				sum = 0;
				for (i = 0; i < order_y; i++)
					for (j = 0; j < order_x; j++) {
						int alpha;
						sx = x - target_x + j * dx;
						sy = y - target_y + i * dy;
						if (edge_mode == LSM_SVG_EDGE_MODE_DUPLICATE) {
							if (sx < x1)
								sx = x1;
							if (sx >= x2)
								sx = x2 - 1;
							if (sy < y1)
								sy = y1;
							if (sy >= y2)
								sy = y2 - 1;
						} else if (edge_mode == LSM_SVG_EDGE_MODE_WRAP) {
							if (sx < x1 || (sx >= x2))
								sx = x1 + (sx - x1) % (x2 - x1);
							if (sy < y1 || (sy >= y2))
								sy = y1 + (sy - y1) % (y2 - y1);
						} else if (edge_mode == LSM_SVG_EDGE_MODE_NONE)
							if (sx < x1 || (sx >= x2) ||
							    sy < y1 || (sy >= y2))
								continue;

						kx = order_x - j - 1;
						ky = order_y - i - 1;
						alpha = in_pixels[4 * sx + sy * rowstride + 3];
						if (ch == 3)
							sval = alpha;
						else if (alpha)
							sval = in_pixels[4 * sx + sy * rowstride + ch] * 255 / alpha;
						else
							sval = 0;
						kval = values[kx + ky * order_x];
						sum += (double) sval *kval;
					}
				tempresult = sum / divisor + bias;

				if (tempresult > 255)
					tempresult = 255;
				if (tempresult < 0)
					tempresult = 0;

				output_pixels[4 * x + y * rowstride + ch] = tempresult;
			}
			if (preserve_alpha)
				output_pixels[4 * x + y * rowstride + channelmap[3]] =
					in_pixels[4 * x + y * rowstride + channelmap[3]];
			for (umch = 0; umch < 3; umch++) {
				ch = channelmap[umch];
				output_pixels[4 * x + y * rowstride + ch] =
					output_pixels[4 * x + y * rowstride + ch] *
					output_pixels[4 * x + y * rowstride + channelmap[3]] / 255;
			}
		}

	cairo_surface_mark_dirty (output->surface);

	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_specular_lighting (LsmSvgFilterSurface *output,
					  double surface_scale, double specular_constant, double specular_exponent,
					  double dx, double dy)
{
	cairo_t *cairo;
	int width, height;
#if 0
	guchar *output_pixels;
	gint rowstride;
	gint x1, x2, y1, y2;
#endif

	g_return_if_fail (output != NULL);

	width = cairo_image_surface_get_width (output->surface);
	height = cairo_image_surface_get_height (output->surface);

	if (height < 1 || width < 1)
		return;

	cairo = cairo_create (output->surface);

#if 0
	output_pixels = cairo_image_surface_get_data (output->surface);
	rowstride = cairo_image_surface_get_stride (output->surface);

	x1 = CLAMP (output->subregion.x, 0, width);
	x2 = CLAMP (output->subregion.x + output->subregion.width, 0, width);
	y1 = CLAMP (output->subregion.y, 0, height);
	y2 = CLAMP (output->subregion.y + output->subregion.height, 0, height);
#endif

	cairo_surface_mark_dirty (output->surface);

	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_image (LsmSvgFilterSurface *output, GdkPixbuf *pixbuf,
			      LsmSvgPreserveAspectRatio preserve_aspect_ratio)
{
	cairo_t *cairo;
	int width, height;
	double zoom_x, zoom_y;

	g_return_if_fail (output != NULL);
	g_return_if_fail (GDK_IS_PIXBUF (pixbuf));
	
	if (output->subregion.width < 1 ||
	    output->subregion.height < 1)
		return;

	/* TODO Implement preserve_aspect_ratio support */

	width = gdk_pixbuf_get_height (pixbuf);
	height = gdk_pixbuf_get_width (pixbuf);

	if (height < 1 || width < 1)
		return;

	zoom_x = (double) output->subregion.width / (double) width;
	zoom_y = (double) output->subregion.height / (double) height;

	cairo = cairo_create (output->surface);
	
	cairo_translate (cairo, output->subregion.x, output->subregion.y);
	cairo_scale (cairo, zoom_x, zoom_y);
	lsm_cairo_set_source_pixbuf (cairo, pixbuf, 0, 0);
	cairo_paint (cairo);

	cairo_destroy (cairo);
}

void
lsm_svg_filter_surface_morphology (LsmSvgFilterSurface *input, LsmSvgFilterSurface *output,
				   LsmSvgMorphologyOperator op, double rx, double ry)
{
	cairo_t *cairo;
	int i, j;
	int ch, extreme;
	gint x, y, x1, x2, y1, y2;
	gint width, height;
	gint rowstride;
	guchar *in_pixels;
	guchar *output_pixels;
	gint kx, ky;
	guchar val;

	g_return_if_fail (input != NULL);
	g_return_if_fail (output != NULL);

	width = cairo_image_surface_get_width (input->surface);
	height = cairo_image_surface_get_height (input->surface);

	if (width != cairo_image_surface_get_width (output->surface) ||
	    height != cairo_image_surface_get_height (output->surface))
		return;

	if (height < 1 || width < 1)
		return;

	kx = rx;
	ky = ry;

	if (kx < 1 && ky < 1)
		return;

	cairo_surface_flush (input->surface);
	cairo = cairo_create (output->surface);

	in_pixels = cairo_image_surface_get_data (input->surface);
	output_pixels = cairo_image_surface_get_data (output->surface);
	rowstride = cairo_image_surface_get_stride (input->surface);

	x1 = CLAMP (input->subregion.x, 0, width);
	x2 = CLAMP (input->subregion.x + input->subregion.width, 0, width);
	y1 = CLAMP (input->subregion.y, 0, height);
	y2 = CLAMP (input->subregion.y + input->subregion.height, 0, height);

	for (y = y1; y < y2; y++)
		for (x = x1; x < x2; x++)
			for (ch = 0; ch < 4; ch++) {
				if (op == LSM_SVG_MORPHOLOGY_OPERATOR_ERODE)
					extreme = 255;
				else
					extreme = 0;
				for (i = -ky; i < ky + 1; i++)
					for (j = -kx; j < kx + 1; j++) {
						if (y + i >= height || y + i < 0 || x + j >= width || x + j < 0)
							continue;

						val = in_pixels[(y + i) * rowstride + (x + j) * 4 + ch];


						if (op == LSM_SVG_MORPHOLOGY_OPERATOR_ERODE) {
							if (extreme > val)
								extreme = val;
						} else {
							if (extreme < val)
								extreme = val;
						}

					}
				output_pixels[y * rowstride + x * 4 + ch] = extreme;
			}

	cairo_surface_mark_dirty (output->surface);

	cairo_destroy (cairo);
}

/* Produces results in the range [1, 2**31 - 2].
   Algorithm is: r = (a * r) mod m
   where a = 16807 and m = 2**31 - 1 = 2147483647
   See [Park & Miller], CACM vol. 31 no. 10 p. 1195, Oct. 1988
   To test: the algorithm should produce the result 1043618065
   as the 10,000th generated number if the original seed is 1.
*/
#define feTurbulence_RAND_m 2147483647  /* 2**31 - 1 */
#define feTurbulence_RAND_a 16807       /* 7**5; primitive root of m */
#define feTurbulence_RAND_q 127773      /* m / a */
#define feTurbulence_RAND_r 2836        /* m % a */
#define feTurbulence_BSize 0x100
#define feTurbulence_BM 0xff
#define feTurbulence_PerlinN 0x1000
#define feTurbulence_NP 12      /* 2^PerlinN */
#define feTurbulence_NM 0xfff

typedef struct {
	double base_frequency_x;
	double base_frequency_y;
	int n_octaves;
	double seed;
	LsmSvgStitchTiles stitch_tiles;
	LsmSvgTurbulenceType type;

	int uLatticeSelector[feTurbulence_BSize + feTurbulence_BSize + 2];
	double fGradient[4][feTurbulence_BSize + feTurbulence_BSize + 2][2];
} LsmSvgTurbulence;

struct feTurbulence_StitchInfo {
	int nWidth;                 /* How much to subtract to wrap for stitching. */
	int nHeight;
	int nWrapX;                 /* Minimum value to wrap. */
	int nWrapY;
};

static long
feTurbulence_setup_seed (int lSeed)
{
	if (lSeed <= 0)
		lSeed = -(lSeed % (feTurbulence_RAND_m - 1)) + 1;
	if (lSeed > feTurbulence_RAND_m - 1)
		lSeed = feTurbulence_RAND_m - 1;
	return lSeed;
}

static long
feTurbulence_random (int lSeed)
{
	long result;

	result =
		feTurbulence_RAND_a * (lSeed % feTurbulence_RAND_q) -
		feTurbulence_RAND_r * (lSeed / feTurbulence_RAND_q);
	if (result <= 0)
		result += feTurbulence_RAND_m;
	return result;
}

static void
feTurbulence_init (LsmSvgTurbulence *turbulence)
{
	double s;
	int i, j, k, lSeed;

	lSeed = feTurbulence_setup_seed (turbulence->seed);
	for (k = 0; k < 4; k++) {
		for (i = 0; i < feTurbulence_BSize; i++) {
			turbulence->uLatticeSelector[i] = i;
			for (j = 0; j < 2; j++)
				turbulence->fGradient[k][i][j] =
					(double) (((lSeed =
						    feTurbulence_random (lSeed)) % (feTurbulence_BSize +
										    feTurbulence_BSize)) -
						  feTurbulence_BSize) / feTurbulence_BSize;
			s = (double) (sqrt
				      (turbulence->fGradient[k][i][0] * turbulence->fGradient[k][i][0] +
				       turbulence->fGradient[k][i][1] * turbulence->fGradient[k][i][1]));
			turbulence->fGradient[k][i][0] /= s;
			turbulence->fGradient[k][i][1] /= s;
		}
	}

	while (--i) {
		k = turbulence->uLatticeSelector[i];
		turbulence->uLatticeSelector[i] = turbulence->uLatticeSelector[j =
			(lSeed =
			 feTurbulence_random (lSeed)) %
			feTurbulence_BSize];
		turbulence->uLatticeSelector[j] = k;
	}

	for (i = 0; i < feTurbulence_BSize + 2; i++) {
		turbulence->uLatticeSelector[feTurbulence_BSize + i] = turbulence->uLatticeSelector[i];
		for (k = 0; k < 4; k++)
			for (j = 0; j < 2; j++)
				turbulence->fGradient[k][feTurbulence_BSize + i][j] = turbulence->fGradient[k][i][j];
	}
}

#define feTurbulence_s_curve(t) ( t * t * (3. - 2. * t) )
#define feTurbulence_lerp(t, a, b) ( a + t * (b - a) )

static double
feTurbulence_noise2 (LsmSvgTurbulence * turbulence,
                     int nColorChannel, double vec[2], struct feTurbulence_StitchInfo *pStitchInfo)
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	register int i, j;

	t = vec[0] + feTurbulence_PerlinN;
	bx0 = (int) t;
	bx1 = bx0 + 1;
	rx0 = t - (int) t;
	rx1 = rx0 - 1.0f;
	t = vec[1] + feTurbulence_PerlinN;
	by0 = (int) t;
	by1 = by0 + 1;
	ry0 = t - (int) t;
	ry1 = ry0 - 1.0f;

	/* If stitching, adjust lattice points accordingly. */
	if (pStitchInfo != NULL) {
		if (bx0 >= pStitchInfo->nWrapX)
			bx0 -= pStitchInfo->nWidth;
		if (bx1 >= pStitchInfo->nWrapX)
			bx1 -= pStitchInfo->nWidth;
		if (by0 >= pStitchInfo->nWrapY)
			by0 -= pStitchInfo->nHeight;
		if (by1 >= pStitchInfo->nWrapY)
			by1 -= pStitchInfo->nHeight;
	}

	bx0 &= feTurbulence_BM;
	bx1 &= feTurbulence_BM;
	by0 &= feTurbulence_BM;
	by1 &= feTurbulence_BM;
	i = turbulence->uLatticeSelector[bx0];
	j = turbulence->uLatticeSelector[bx1];
	b00 = turbulence->uLatticeSelector[i + by0];
	b10 = turbulence->uLatticeSelector[j + by0];
	b01 = turbulence->uLatticeSelector[i + by1];
	b11 = turbulence->uLatticeSelector[j + by1];
	sx = (double) (feTurbulence_s_curve (rx0));
	sy = (double) (feTurbulence_s_curve (ry0));
	q = turbulence->fGradient[nColorChannel][b00];
	u = rx0 * q[0] + ry0 * q[1];
	q = turbulence->fGradient[nColorChannel][b10];
	v = rx1 * q[0] + ry0 * q[1];
	a = feTurbulence_lerp (sx, u, v);
	q = turbulence->fGradient[nColorChannel][b01];
	u = rx0 * q[0] + ry1 * q[1];
	q = turbulence->fGradient[nColorChannel][b11];
	v = rx1 * q[0] + ry1 * q[1];
	b = feTurbulence_lerp (sx, u, v);

	return feTurbulence_lerp (sy, a, b);
}

static double
feTurbulence_turbulence (LsmSvgTurbulence * turbulence,
                         int nColorChannel, double *point,
                         double fTileX, double fTileY, double fTileWidth, double fTileHeight)
{
	struct feTurbulence_StitchInfo stitch;
	struct feTurbulence_StitchInfo *pStitchInfo = NULL; /* Not stitching when NULL. */

	double fSum = 0.0f, vec[2], ratio = 1.;
	int nOctave;

	/* Adjust the base frequencies if necessary for stitching. */
	if (turbulence->stitch_tiles == LSM_SVG_STITCH_TILES_STITCH) {
		/* When stitching tiled turbulence, the frequencies must be adjusted
		   so that the tile borders will be continuous. */
		if (turbulence->base_frequency_x != 0.0) {
			double fLoFreq = (double) (floor (fTileWidth * turbulence->base_frequency_x)) / fTileWidth;
			double fHiFreq = (double) (ceil (fTileWidth * turbulence->base_frequency_x)) / fTileWidth;
			if (turbulence->base_frequency_x / fLoFreq < fHiFreq / turbulence->base_frequency_x)
				turbulence->base_frequency_x = fLoFreq;
			else
				turbulence->base_frequency_x = fHiFreq;
		}

		if (turbulence->base_frequency_y != 0.0) {
			double fLoFreq = (double) (floor (fTileHeight * turbulence->base_frequency_y)) / fTileHeight;
			double fHiFreq = (double) (ceil (fTileHeight * turbulence->base_frequency_y)) / fTileHeight;
			if (turbulence->base_frequency_y / fLoFreq < fHiFreq / turbulence->base_frequency_y)
				turbulence->base_frequency_y = fLoFreq;
			else
				turbulence->base_frequency_y = fHiFreq;
		}

		/* Set up initial stitch values. */
		pStitchInfo = &stitch;
		stitch.nWidth = (int) (fTileWidth * turbulence->base_frequency_x + 0.5f);
		stitch.nWrapX = fTileX * turbulence->base_frequency_x + feTurbulence_PerlinN + stitch.nWidth;
		stitch.nHeight = (int) (fTileHeight * turbulence->base_frequency_y + 0.5f);
		stitch.nWrapY = fTileY * turbulence->base_frequency_y + feTurbulence_PerlinN + stitch.nHeight;
	}

	vec[0] = point[0] * turbulence->base_frequency_x;
	vec[1] = point[1] * turbulence->base_frequency_y;

	for (nOctave = 0; nOctave < turbulence->n_octaves; nOctave++) {
		if (turbulence->type == LSM_SVG_TURBULENCE_TYPE_FRACTAL_NOISE)
			fSum +=
				(double) (feTurbulence_noise2 (turbulence, nColorChannel, vec, pStitchInfo) / ratio);
		else
			fSum +=
				(double) (fabs (feTurbulence_noise2 (turbulence, nColorChannel, vec, pStitchInfo)) /
					  ratio);

		vec[0] *= 2;
		vec[1] *= 2;
		ratio *= 2;

		if (pStitchInfo != NULL) {
			/* Update stitch values. Subtracting PerlinN before the multiplication and
			   adding it afterward simplifies to subtracting it once. */
			stitch.nWidth *= 2;
			stitch.nWrapX = 2 * stitch.nWrapX - feTurbulence_PerlinN;
			stitch.nHeight *= 2;
			stitch.nWrapY = 2 * stitch.nWrapY - feTurbulence_PerlinN;
		}
	}

	return fSum;
}
void
lsm_svg_filter_surface_turbulence (LsmSvgFilterSurface *output,
				   double base_frequency_x, double base_frequency_y,
				   int n_octaves, double seed,
				   LsmSvgStitchTiles stitch_tiles, LsmSvgTurbulenceType type,
				   const cairo_matrix_t *transform)
{
	LsmSvgTurbulence turbulence;
	cairo_t *cairo;
	gint x, y, x1, x2, y1, y2;
	gint width, height, tileWidth, tileHeight;
	gint rowstride;
	guchar *output_pixels;
	cairo_matrix_t affine;

	g_return_if_fail (output != NULL);
	g_return_if_fail (transform != NULL);

	affine = *transform;
	if (cairo_matrix_invert (&affine) != CAIRO_STATUS_SUCCESS)
		return;

	width = cairo_image_surface_get_width (output->surface);
	height = cairo_image_surface_get_height (output->surface);

	if (height < 1 || width < 1)
		return;

	cairo = cairo_create (output->surface);

	output_pixels = cairo_image_surface_get_data (output->surface);
	rowstride = cairo_image_surface_get_stride (output->surface);

	x1 = CLAMP (output->subregion.x, 0, width);
	x2 = CLAMP (output->subregion.x + output->subregion.width, 0, width);
	y1 = CLAMP (output->subregion.y, 0, height);
	y2 = CLAMP (output->subregion.y + output->subregion.height, 0, height);

	turbulence.base_frequency_x = base_frequency_x;
	turbulence.base_frequency_y = base_frequency_y;
	turbulence.n_octaves = n_octaves;
	turbulence.seed = seed;
	turbulence.stitch_tiles = stitch_tiles;
	turbulence.type = type;

	feTurbulence_init (&turbulence);

	tileWidth = x2 - x1;
	tileHeight = y2 - y1;

	for (y = 0; y < tileHeight; y++) {
		for (x = 0; x < tileWidth; x++) {
			gint i;
			double point[2];
			guchar *pixel;
			point[0] = affine.xx * (x + x1) + affine.xy * (y + y1) + affine.x0;
			point[1] = affine.yx * (x + x1) + affine.yy * (y + y1) + affine.y0;

			pixel = output_pixels + 4 * (x + x1) + (y + y1) * rowstride;

			for (i = 0; i < 4; i++) {
				double cr;

				cr = feTurbulence_turbulence (&turbulence, i, point, (double) x, (double) y,
							      (double) tileWidth, (double) tileHeight);

				if (type == LSM_SVG_TURBULENCE_TYPE_FRACTAL_NOISE)
					cr = ((cr * 255.) + 255.) / 2.;
				else
					cr = (cr * 255.);

				cr = CLAMP (cr, 0., 255.);

				pixel[channelmap[i]] = (guchar) cr;
			}
			for (i = 0; i < 3; i++)
				pixel[channelmap[i]] =
					pixel[channelmap[i]] * pixel[channelmap[3]] / 255;

		}
	}

	cairo_surface_mark_dirty (output->surface);

	cairo_destroy (cairo);
}
