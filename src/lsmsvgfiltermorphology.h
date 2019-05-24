/* Lasem
 *
 * Copyright © 2015 Emmanuel Pacaud
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

#ifndef LSM_SVG_FILTER_MORPHOLOGY_H
#define LSM_SVG_FILTER_MORPHOLOGY_H

#include <lsmsvgtypes.h>
#include <lsmsvgfilterprimitive.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_FILTER_MORPHOLOGY             (lsm_svg_filter_morphology_get_type ())
#define LSM_SVG_FILTER_MORPHOLOGY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_FILTER_MORPHOLOGY, LsmSvgFilterMorphology))
#define LSM_SVG_FILTER_MORPHOLOGY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_FILTER_MORPHOLOGY, LsmSvgFilterMorphologyClass))
#define LSM_IS_SVG_FILTER_MORPHOLOGY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_FILTER_MORPHOLOGY))
#define LSM_IS_SVG_FILTER_MORPHOLOGY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_FILTER_MORPHOLOGY))
#define LSM_SVG_FILTER_MORPHOLOGY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_FILTER_MORPHOLOGY, LsmSvgFilterMorphologyClass))

typedef struct _LsmSvgFilterMorphologyClass LsmSvgFilterMorphologyClass;

struct _LsmSvgFilterMorphology {
	LsmSvgFilterPrimitive base;

	LsmSvgMorphologyOperatorAttribute op;
	LsmSvgLengthAttribute radius;
};

struct _LsmSvgFilterMorphologyClass {
	LsmSvgFilterPrimitiveClass  element_class;
};

GType lsm_svg_filter_morphology_get_type (void);

LsmDomNode * 	lsm_svg_filter_morphology_new 		(void);

G_END_DECLS

#endif
