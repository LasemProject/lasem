/* Lasem
 *
 * Copyright © 2009 Emmanuel Pacaud
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

#ifndef LSM_SVG_G_ELEMENT_H
#define LSM_SVG_G_ELEMENT_H

#include <lsmsvgtypes.h>
#include <lsmsvgtransformable.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_G_ELEMENT             (lsm_svg_g_element_get_type ())
#define LSM_SVG_G_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_G_ELEMENT, LsmSvgGElement))
#define LSM_SVG_G_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_G_ELEMENT, LsmSvgGElementClass))
#define LSM_IS_SVG_G_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_G_ELEMENT))
#define LSM_IS_SVG_G_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_G_ELEMENT))
#define LSM_SVG_G_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_G_ELEMENT, LsmSvgGElementClass))

typedef struct _LsmSvgGElementClass LsmSvgGElementClass;

struct _LsmSvgGElement {
	LsmSvgTransformable base;
};

struct _LsmSvgGElementClass {
	LsmSvgTransformableClass  base_class;
};

GType lsm_svg_g_element_get_type (void);

LsmDomNode *		lsm_svg_g_element_new 		(void);

G_END_DECLS

#endif
