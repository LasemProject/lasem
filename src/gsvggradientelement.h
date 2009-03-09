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

#ifndef GSVG_GRADIENT_ELEMENT_H
#define GSVG_GRADIENT_ELEMENT_H

#include <gsvg.h>
#include <gsvggraphic.h>

G_BEGIN_DECLS

#define GSVG_TYPE_GRADIENT_ELEMENT             (gsvg_gradient_element_get_type ())
#define GSVG_GRADIENT_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSVG_TYPE_GRADIENT_ELEMENT, GSvgGradientElement))
#define GSVG_GRADIENT_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSVG_TYPE_GRADIENT_ELEMENT, GSvgGradientElementClass))
#define GSVG_IS_GRADIENT_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSVG_TYPE_GRADIENT_ELEMENT))
#define GSVG_IS_GRADIENT_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSVG_TYPE_GRADIENT_ELEMENT))
#define GSVG_GRADIENT_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GSVG_TYPE_GRADIENT_ELEMENT, GSvgGradientElementClass))

typedef struct _GSvgGradientElementClass GSvgGradientElementClass;

struct _GSvgGradientElement {
	GSvgGraphic graphic;

	GSvgTransformAttribute transform;
	GDomEnumAttribute units;
	GDomEnumAttribute spread_method;
	GDomAttribute href;
};

struct _GSvgGradientElementClass {
	GSvgGraphicClass  parent_class;
};

GType gsvg_gradient_element_get_type (void);

G_END_DECLS

#endif
