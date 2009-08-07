/* lsmsvgelement.h
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

#ifndef LSM_SVG_ELEMENT_H
#define LSM_SVG_ELEMENT_H

#include <lsmsvg.h>
#include <lsmattributes.h>
#include <lsmsvgattributes.h>
#include <lsmsvgstyle.h>
#include <lsmdomelement.h>
#include <cairo.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_ELEMENT             (lsm_svg_element_get_type ())
#define LSM_SVG_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_ELEMENT, LsmSvgElement))
#define LSM_SVG_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_ELEMENT, LsmSvgElementClass))
#define LSM_IS_SVG_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_ELEMENT))
#define LSM_IS_SVG_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_ELEMENT))
#define LSM_SVG_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_ELEMENT, LsmSvgElementClass))

typedef struct _LsmSvgElementClass LsmSvgElementClass;

struct _LsmSvgElement {
	LsmDomElement	element;

	LsmPropertyBag property_bag;

	LsmAttribute			id;
	LsmAttribute			class_name;
	LsmSvgTransformAttribute 	transform;
};

struct _LsmSvgElementClass {
	LsmDomElementClass  parent_class;

	LsmAttributeManager *attribute_manager;

	void		(*enable_rendering)	(LsmSvgElement *element);
	void 		(*render)		(LsmSvgElement *element, LsmSvgView *view);
	void 		(*get_extents)		(LsmSvgElement *element, LsmSvgView *view, LsmExtents *extents);
};

GType lsm_svg_element_get_type (void);

void 		lsm_svg_element_render 		(LsmSvgElement *element, LsmSvgView *view);
void 		lsm_svg_element_force_render 	(LsmSvgElement *element, LsmSvgView *view);
void		lsm_svg_element_get_extents	(LsmSvgElement *element, LsmSvgView *view, LsmExtents *extents);

G_END_DECLS

#endif
