/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#ifndef LSM_SVG_ATTRIBUTE_BAGS_H
#define LSM_SVG_ATTRIBUTE_BAGS_H

#include <lsmsvg.h>
#include <lsmsvgattributes.h>

G_BEGIN_DECLS

typedef struct {
	LsmSvgPaintAttribute paint;
	LsmDomEnumAttribute rule;
	LsmDomDoubleAttribute opacity;
	LsmDomAttribute clip_path;
	LsmDomEnumAttribute clip_rule;
} LsmSvgFillAttributeBag;

typedef struct {
	LsmSvgPaintAttribute paint;
	LsmSvgLengthAttribute width;
	LsmDomEnumAttribute line_join;
	LsmDomEnumAttribute line_cap;
	LsmSvgDoubleAttribute miter_limit;
	LsmDomDoubleAttribute opacity;
	LsmSvgLengthAttribute dash_offset;
	LsmSvgDashArrayAttribute dash_array;
} LsmSvgStrokeAttributeBag;

typedef struct {
	LsmSvgTransformAttribute transform;
} LsmSvgTransformAttributeBag;

typedef struct {
	LsmDomStringAttribute font_family;
	LsmSvgLengthAttribute font_size;
} LsmSvgTextAttributeBag;

typedef struct {
	LsmSvgColorAttribute color;
	LsmSvgDoubleAttribute opacity;
} LsmSvgStopAttributeBag;

void 	lsm_dom_attribute_map_add_fill_attribute_bag 		(LsmDomAttributeMap *map, ptrdiff_t bag_offset);
void 	lsm_dom_attribute_map_add_stroke_attribute_bag		(LsmDomAttributeMap *map, ptrdiff_t bag_offset);
void 	lsm_dom_attribute_map_add_transform_attribute_bag	(LsmDomAttributeMap *map, ptrdiff_t bag_offset);
void 	lsm_dom_attribute_map_add_text_attribute_bag		(LsmDomAttributeMap *map, ptrdiff_t bag_offset);
void 	lsm_dom_attribute_map_add_stop_attribute_bag		(LsmDomAttributeMap *map, ptrdiff_t bag_offset);

G_END_DECLS

#endif
