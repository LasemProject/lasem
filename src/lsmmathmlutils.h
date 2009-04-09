/* lsmmathmlutils.h
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

#ifndef LSM_MATHML_UTILS_H
#define LSM_MATHML_UTILS_H

#include <lsmmathml.h>

G_BEGIN_DECLS

typedef struct {
	double width;
	double height;
	double depth;
	gboolean is_defined;
} LsmMathmlBbox;

const LsmMathmlBbox lsm_mathml_bbox_null;

void 	lsm_mathml_bbox_add_horizontally 		(LsmMathmlBbox *bbox, const LsmMathmlBbox *new_bbox);
void 	lsm_mathml_bbox_add_over 			(LsmMathmlBbox *self, const LsmMathmlBbox *bbox);
void 	lsm_mathml_bbox_add_under 			(LsmMathmlBbox *self, const LsmMathmlBbox *bbox);
void 	lsm_mathml_bbox_merge_vertically 		(LsmMathmlBbox *self, const LsmMathmlBbox *bbox, double offset);
void 	lsm_mathml_bbox_stretch			(LsmMathmlBbox *self, const LsmMathmlBbox *bbox);
void 	lsm_mathml_bbox_stretch_vertically		(LsmMathmlBbox *self, const LsmMathmlBbox *bbox);
void 	lsm_mathml_bbox_stretch_horizontally	(LsmMathmlBbox *self, const LsmMathmlBbox *bbox);

G_END_DECLS

#endif
