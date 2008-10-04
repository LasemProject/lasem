/* gmathmlview.h
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef GMATHML_VIEW_H
#define GMATHML_VIEW_H

#include <gmathml.h>
#include <gmathmlutils.h>
#include <gmathmlelement.h>
#include <gmathmlpresentationtoken.h>
#include <cairo.h>
#include <pango/pangocairo.h>

G_BEGIN_DECLS

typedef enum {
	GMATHML_GLYPH_FLAG_STRETCH_VERTICAL =	1 << 0,
	GMATHML_GLYPH_FLAG_STRETCH_HORIZONTAL =	1 << 1,
	GMATHML_GLYPH_FLAG_TYPE_SIZED =		1 << 2,
	GMATHML_GLYPH_FLAG_ALIGN_AXIS =		1 << 3,
	GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION =	1 << 4
} GMathmlGlyphFlags;

typedef enum {
	GMATHML_FONT_ERROR,
	GMATHML_FONT_DEFAULT,
	GMATHML_FONT_CMR10,
	GMATHML_FONT_CMI10,
	GMATHML_FONT_CMEX10,
	GMATHML_FONT_CMSY10,
	GMATHML_FONT_SYMBOL
} GMathmlFont;

typedef struct {
	GMathmlFont 	font;
	const char 	utf8[4];
} GMathmlGlyph;

typedef struct {
	const char		*utf8;
	GMathmlGlyphFlags	flags;
	GMathmlGlyph		start_glyph;
	GMathmlGlyph		middle_glyph;
	GMathmlGlyph		stop_glyph;
	GMathmlGlyph		glue_glyph;
	unsigned int		n_sized_glyphs;
	GMathmlGlyph		sized_glyphs[5];
} GMathmlOperatorGlyph;

#define GMATHML_TYPE_VIEW             (gmathml_view_get_type ())
#define GMATHML_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_VIEW, GMathmlView))
#define GMATHML_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_VIEW, GMathmlViewClass))
#define GMATHML_IS_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_VIEW))
#define GMATHML_IS_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_VIEW))
#define GMATHML_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_VIEW, GMathmlViewClass))

typedef struct _GMathmlViewClass GMathmlViewClass;
typedef struct _GMathmlViewPrivate GMathmlViewPrivate;

struct _GMathmlView {
	GObject	object;

	GMathmlViewPrivate *priv;
};

struct _GMathmlViewClass {
	GObjectClass parent_class;
};

GType gmathml_view_get_type (void);

GMathmlView *	gmathml_view_new 		(GMathmlDocument *document, cairo_t *cr);

void 		gmathml_view_render 		(GMathmlView *view);
void 		gmathml_view_measure 		(GMathmlView *view, double *width, double *height);

void		gmathml_view_set_cairo 		(GMathmlView *view, cairo_t *cr);

void		gmathml_view_set_debug 		(GMathmlView *view, gboolean debug);

void		gmathml_view_set_ppi 		(GMathmlView *view, double ppi);

/* Internal API */

double 		gmathml_view_measure_length	(GMathmlView *view, double length);

void 		gmathml_view_measure_text 	(GMathmlView *view, char const *text,
						 double math_size, GMathmlVariant math_variant,
						 GMathmlBbox *bbox);
void 		gmathml_view_show_text 		(GMathmlView *view, double x, double y, const char *text,
						 double math_size, GMathmlVariant math_variant,
						 const GMathmlColor *math_color);

void 		gmathml_view_measure_operator 	(GMathmlView *view, char const *text,
						 double math_size, GMathmlVariant math_variant,
						 gboolean large, gboolean symmetric,
						 double axis_offset,
						 GMathmlBbox const *stretch_bbox, GMathmlBbox *bbox);
void 		gmathml_view_show_operator 	(GMathmlView *view, double x, double y, char const *text,
						 double math_size, GMathmlVariant math_variant,
						 const GMathmlColor *math_color,
						 gboolean large,
						 GMathmlBbox const *stretch_bbox);

void 		gmathml_view_measure_radical 	(GMathmlView *view, double math_size,
						 GMathmlBbox const *stretch_bbox, GMathmlBbox *bbox,
						 double *x_offset, double *y_offset);
void 		gmathml_view_show_radical	 (GMathmlView *view, double x, double y, double width,
						 double math_size, const GMathmlColor *math_color,
						 GMathmlBbox const *stretch_bbox);

double 		gmathml_view_measure_axis_offset(GMathmlView *view, double math_size);

void 		gmathml_view_show_background 	(GMathmlView *view, double x, double y,
						 const GMathmlColor *math_background,
						 const GMathmlBbox *bbox);
void 		gmathml_view_show_rectangle 	(GMathmlView *view, double x, double y, double width, double height,
						 GMathmlLine line, double line_width,
						 const GMathmlColor *color);
void 		gmathml_view_show_line 		(GMathmlView *view, double x0, double y0, double x1, double y1,
						 GMathmlLine line, double line_width,
						 const GMathmlColor *color);
void 		gmathml_view_show_fraction_line	(GMathmlView *view,
						 double x, double y,
						 double width, double thickness,
						 const GMathmlColor *color);


void 		gmathml_view_show_bbox 		(GMathmlView *view, double x, double y, const GMathmlBbox *bbox);

void		gmathml_view_draw_root		(GMathmlView *view,
						 double x, double y,
						 double width, double height,
						 double top_width,
						 double thickness,
						 GMathmlColor *color);

G_END_DECLS

#endif
