/*
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

#include <gmathmllayoututils.h>
#include <math.h>

void
gmathml_measure_sub_sup (GMathmlElement *parent,
			 GMathmlView *view,
			 GMathmlElement *base,
			 GMathmlElement *subscript,
			 GMathmlElement *superscript,
			 double subscript_offset_min,
			 double superscript_offset_min,
			 GMathmlDisplay display,
			 const GMathmlBbox *stretch_bbox,
			 GMathmlBbox *bbox,
			 double *subscript_offset,
			 double *superscript_offset)
{
	GMathmlBbox const *base_bbox = NULL;
	GMathmlBbox const *subscript_bbox = NULL;
	GMathmlBbox const *superscript_bbox = NULL;
	GMathmlBbox children_bbox = gmathml_bbox_null;
	double axis_offset, ascent, descent;
	double v_space, h_space;
	gboolean is_operator;

	*bbox = gmathml_bbox_null;

	if (base == NULL)
		return;

	is_operator = gmathml_element_get_embellished_core (base) != NULL;

	axis_offset = gmathml_view_measure_axis_offset (view, parent->style.math_size);
	h_space = parent->style.math_size * GMATHML_SPACE_EM_VERY_THIN;
	v_space = parent->style.math_size * GMATHML_SPACE_EM_THIN;
	gmathml_view_get_font_metrics (view, &parent->style, &ascent, &descent);

	if (display == GMATHML_DISPLAY_INLINE)
		descent /= 2.0;

	base_bbox = gmathml_element_measure (GMATHML_ELEMENT (base), view, stretch_bbox);

	*bbox = *base_bbox;
	bbox->width += gmathml_view_measure_length (view, h_space);

	subscript_bbox = subscript != NULL ? gmathml_element_measure (subscript, view, NULL) : NULL;
	superscript_bbox = superscript != NULL ? gmathml_element_measure (superscript, view, NULL) : NULL;

	if (subscript_bbox != NULL) {
		if (is_operator) {
			*subscript_offset = base_bbox->depth +
				0.5 * gmathml_view_measure_axis_offset (view, subscript->style.math_size);
		} else {
			*subscript_offset = descent;

			if (base_bbox->depth > descent)
				*subscript_offset += base_bbox->depth - descent;
			if (subscript_bbox->height - *subscript_offset > axis_offset)
				*subscript_offset = subscript_bbox->height - axis_offset;
		}
	} else
		*subscript_offset = 0.0;

	if (superscript_bbox != NULL) {
		if (is_operator) {
			*superscript_offset = base_bbox->height -
				1.5 * gmathml_view_measure_axis_offset (view, superscript->style.math_size);
		} else {
			if (display == GMATHML_DISPLAY_INLINE)
				*superscript_offset = axis_offset;
			else {
				double superscript_descent;

				gmathml_view_get_font_metrics (view, &superscript->style,
							       NULL, &superscript_descent);
				*superscript_offset = axis_offset + superscript_descent;
			}

			if (base_bbox->height > ascent)
				*superscript_offset += base_bbox->height - ascent;
			if (*superscript_offset - superscript_bbox->depth < axis_offset)
				*superscript_offset = axis_offset + superscript_bbox->depth;
		}
	} else
		*superscript_offset = 0.0;

	if (superscript_bbox != NULL && subscript_bbox != NULL) {
		double delta = (*superscript_offset + *subscript_offset) -
			(superscript_bbox->depth + subscript_bbox->height);
		if (delta < v_space) {
			*superscript_offset += fabs (delta - v_space) * 0.5;
			*subscript_offset   += fabs (delta - v_space) * 0.5;
		}
	}

	*superscript_offset = gmathml_view_measure_length (view, MAX (*superscript_offset,
								      superscript_offset_min));
	*subscript_offset = gmathml_view_measure_length (view, MAX (*subscript_offset,
								    subscript_offset_min));

	if (subscript_bbox != NULL)
		gmathml_bbox_merge_vertically (&children_bbox, subscript_bbox, -*subscript_offset);
	if (superscript_bbox != NULL)
		gmathml_bbox_merge_vertically (&children_bbox, superscript_bbox, *superscript_offset);

	gmathml_bbox_add_horizontally (bbox, &children_bbox);
}

void
gmathml_layout_sub_sup (GMathmlElement *parent,
			GMathmlView *view,
			double x,
			double y,
			GMathmlElement *base,
			GMathmlElement *subscript,
			GMathmlElement *superscript,
			double subscript_offset,
			double superscript_offset)
{
	const GMathmlBbox *base_bbox;
	double h_space;

	if (base == NULL)
		return;

	base_bbox = gmathml_element_get_bbox (base);

	gmathml_element_layout (base, view, x, y, base_bbox);

	h_space = parent->style.math_size * GMATHML_SPACE_EM_VERY_THIN;
	x += gmathml_view_measure_length (view, h_space);

	if (subscript)
		gmathml_element_layout (subscript, view,
					x + base_bbox->width,
					y + subscript_offset,
					gmathml_element_get_bbox (subscript));
	if (superscript)
		gmathml_element_layout (superscript, view,
					x + base_bbox->width,
					y - superscript_offset,
					gmathml_element_get_bbox (superscript));
}
