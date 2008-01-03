/* gmathmlutils.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmlutils.h>

void
gmathml_bbox_add_to_right (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	self->width += bbox->width;
}

void
gmathml_bbox_add_over (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	self->height += bbox->height + bbox->depth;
	self->width = MAX (self->width, bbox->width);
}

void
gmathml_bbox_add_under (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	self->depth += bbox->height + bbox->depth;
	self->width = MAX (self->width, bbox->width);
}
