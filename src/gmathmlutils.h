/* gmathmlutils.h
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

#ifndef GMATHML_UTILS_H
#define GMATHML_UTILS_H

#include <gmathml.h>

G_BEGIN_DECLS

typedef struct {
	double width;
	double height;
	double depth;
	gboolean is_defined;
} GMathmlBbox;

const GMathmlBbox gmathml_bbox_null;

void 		gmathml_bbox_add_horizontally 	(GMathmlBbox *bbox, const GMathmlBbox *new_bbox);
void 		gmathml_bbox_add_over 		(GMathmlBbox *self, const GMathmlBbox *bbox);
void 		gmathml_bbox_add_under 		(GMathmlBbox *self, const GMathmlBbox *bbox);
void 		gmathml_bbox_merge_vertically 	(GMathmlBbox *self, const GMathmlBbox *bbox, double offset);
void 		gmathml_bbox_stretch_vertically	(GMathmlBbox *self, const GMathmlBbox *bbox);

G_END_DECLS

#endif
