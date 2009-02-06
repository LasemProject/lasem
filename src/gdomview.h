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

#ifndef GDOM_VIEW_H
#define GDOM_VIEW_H

#include <gdom.h>
#include <cairo.h>
#include <pango/pangocairo.h>

G_BEGIN_DECLS

#define GDOM_TYPE_VIEW             (gdom_view_get_type ())
#define GDOM_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_VIEW, GDomView))
#define GDOM_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_VIEW, GDomViewClass))
#define GDOM_IS_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_VIEW))
#define GDOM_IS_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_VIEW))
#define GDOM_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_VIEW, GDomViewClass))

typedef struct _GDomViewClass GDomViewClass;

struct _GDomView {
	GObject	object;

	GDomDocument *		document;

	PangoFontDescription *	font_description;
	PangoLayout *		pango_layout;
	PangoLayout *		measure_pango_layout;
	cairo_t *		cairo;
	gboolean		is_vector;

	gboolean debug;
};

struct _GDomViewClass {
	GObjectClass parent_class;

	GType document_type;

	void (*measure)		(GDomView *view, double *width, double *height);
	void (*render)		(GDomView *view, double x, double y);
};

GType gdom_view_get_type (void);

void 			gdom_view_render 		(GDomView *view, double x, double y);
void			gdom_view_measure 		(GDomView *view, double *width, double *height);

void 			gdom_view_set_document 		(GDomView *view, GDomDocument *document);
void			gdom_view_set_cairo 		(GDomView *view, cairo_t *cr);

void			gdom_view_set_debug 		(GDomView *view, gboolean debug);

G_END_DECLS

#endif
