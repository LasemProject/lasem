/* gmathmlview.h
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

double 		gmathml_view_measure_length	(GMathmlView *view, double length);
double 		gmathml_view_measure_hairline	(GMathmlView *view);
void 		gmathml_view_measure_text 	(GMathmlView *view, char const *text, GMathmlBbox *bbox);
double 		gmathml_view_measure_axis_offset(GMathmlView *view);

void 		gmathml_view_show_text 		(GMathmlView *view, double x, double y, const char *text);

void 		gmathml_view_show_rectangle 	(GMathmlView *view, double x, double y, double width, double height,
						 GMathmlLine line);
void 		gmathml_view_show_line 		(GMathmlView *view, double x0, double y0, double x1, double y1,
						 GMathmlLine line);
void 		gmathml_view_show_background 	(GMathmlView *view, double x, double y, const GMathmlBbox *bbox);

void 		gmathml_view_show_bbox 		(GMathmlView *view, double x, double y, const GMathmlBbox *bbox);

void		gmathml_view_push_element	(GMathmlView *view, const GMathmlElement *element);
void		gmathml_view_pop_element	(GMathmlView *view);

void 		gmathml_view_draw_fraction_line 	(GMathmlView *view,
							 double x, double y,
							 double width, double thickness,
							 GMathmlColor *color);

void		gmathml_view_draw_root		(GMathmlView *view,
						 double x, double y,
						 double width, double height,
						 double top_width,
						 double thickness,
						 GMathmlColor *color);

void		gmathml_view_set_debug 		(GMathmlView *view, gboolean debug);

G_END_DECLS

#endif
