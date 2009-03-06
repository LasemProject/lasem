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

#ifndef GSVG_DEFS_ELEMENT_H
#define GSVG_DEFS_ELEMENT_H

#include <gsvg.h>
#include <gsvggraphic.h>

G_BEGIN_DECLS

#define GSVG_TYPE_DEFS_ELEMENT             (gsvg_defs_element_get_type ())
#define GSVG_DEFS_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_DEFS_ELEMENT, GSvgDefsElement))
#define GSVG_DEFS_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_DEFS_ELEMENT, GSvgDefsElementClass))
#define GSVG_IS_DEFS_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_DEFS_ELEMENT))
#define GSVG_IS_DEFS_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_DEFS_ELEMENT))
#define GSVG_DEFS_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_DEFS_ELEMENT, GSvgDefsElementClass))

typedef struct _GSvgDefsElementClass GSvgDefsElementClass;

struct _GSvgDefsElement {
	GSvgGraphic graphic;
};

struct _GSvgDefsElementClass {
	GSvgGraphicClass  parent_class;
};

GType gsvg_defs_element_get_type (void);

GDomNode *		gsvg_defs_element_new 		(void);

G_END_DECLS

#endif