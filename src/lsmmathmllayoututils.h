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

#ifndef LSM_MATHML_LAYOUT_UTILS_H
#define LSM_MATHML_LAYOUT_UTILS_H

#include <lsmmathml.h>
#include <lsmmathmlview.h>

G_BEGIN_DECLS

void 		lsm_mathml_measure_sub_sup 	(LsmMathmlElement *parent,
						 LsmMathmlView *view,
						 LsmMathmlElement *base,
						 LsmMathmlElement *subscript,
						 LsmMathmlElement *superscript,
						 double subscript_offset_min,
						 double superscript_offset_min,
						 LsmMathmlDisplay display,
						 const LsmMathmlBbox *stretch_bbox,
						 LsmMathmlBbox *bbox,
						 double *subscript_offset,
						 double *superscript_offset);

void		lsm_mathml_layout_sub_sup 		(LsmMathmlElement *parent,
						 LsmMathmlView *view,
						 double x,
						 double y,
						 LsmMathmlElement *base,
						 LsmMathmlElement *subscript,
						 LsmMathmlElement *superscript,
						 double subscript_offset,
						 double superscript_offset);

G_END_DECLS

#endif

