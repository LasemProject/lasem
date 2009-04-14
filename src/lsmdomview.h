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

#ifndef LSM_DOM_VIEW_H
#define LSM_DOM_VIEW_H

#include <lsmdom.h>
#include <cairo.h>
#include <pango/pangocairo.h>

G_BEGIN_DECLS

#define LSM_TYPE_DOM_VIEW             (lsm_dom_view_get_type ())
#define LSM_DOM_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_VIEW, LsmDomView))
#define LSM_DOM_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_VIEW, LsmDomViewClass))
#define LSM_IS_DOM_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_VIEW))
#define LSM_IS_DOM_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_VIEW))
#define LSM_DOM_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_VIEW, LsmDomViewClass))

typedef struct _GDomViewClass LsmDomViewClass;

struct _GDomView {
	GObject	object;

	LsmDomDocument *		document;

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

	void (*measure)		(LsmDomView *view, double *width, double *height);
	void (*render)		(LsmDomView *view, double x, double y);
};

GType lsm_dom_view_get_type (void);

void 			lsm_dom_view_render 		(LsmDomView *view, double x, double y);
void			lsm_dom_view_measure 		(LsmDomView *view, double *width, double *height);

void 			lsm_dom_view_set_document 		(LsmDomView *view, LsmDomDocument *document);
void			lsm_dom_view_set_cairo 		(LsmDomView *view, cairo_t *cr);

void			lsm_dom_view_set_debug 		(LsmDomView *view, gboolean debug);

G_END_DECLS

#endif
