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

#ifndef GSVG_VIEW_H
#define GSVG_VIEW_H

#include <gdomview.h>
#include <gsvgelement.h>
#include <gsvg.h>

G_BEGIN_DECLS

#define GSVG_TYPE_VIEW             (gsvg_view_get_type ())
#define GSVG_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_VIEW, GSvgView))
#define GSVG_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_VIEW, GSvgViewClass))
#define GSVG_IS_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_VIEW))
#define GSVG_IS_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_VIEW))
#define GSVG_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_VIEW, GSvgViewClass))

typedef struct _GSvgViewClass GSvgViewClass;
typedef struct _GSvgViewPrivate GSvgViewPrivate;

struct _GSvgView {
	GDomView dom_view;

	GSList *fill_stack;
	GSList *stroke_stack;
};

struct _GSvgViewClass {
	GDomViewClass parent_class;
};

GType gsvg_view_get_type (void);

GSvgView *	gsvg_view_new 			(GSvgDocument *document);

void		gsvg_view_push_transform 		(GSvgView *view, const GSvgMatrix *matrix);
void		gsvg_view_pop_transform			(GSvgView *view);
void 		gsvg_view_push_fill_attributes 		(GSvgView *view, GSvgFillAttributeBag *fill);
void 		gsvg_view_pop_fill_attributes 		(GSvgView *view);
void 		gsvg_view_push_stroke_attributes 	(GSvgView *view, GSvgStrokeAttributeBag *stroke);
void 		gsvg_view_pop_stroke_attributes 	(GSvgView *view);

void 		gsvg_view_show_rectangle 	(GSvgView *view, double x, double y,
						                 double width, double height,
								 double rx, double ry);
void 		gsvg_view_show_circle		(GSvgView *view, double cx, double cy, double r);
void 		gsvg_view_show_ellipse		(GSvgView *view, double cx, double cy, double rx, double ry);
void		gsvg_view_show_path		(GSvgView *view, const char *d);

G_END_DECLS

#endif
