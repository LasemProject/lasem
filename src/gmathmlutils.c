/* gmathmlutils.c
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

#include <gmathmlutils.h>

const GMathmlBbox gmathml_bbox_null = {0.0, 0.0, 0.0, FALSE};

void
gmathml_bbox_add_horizontally (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		return;
	}

	self->width += bbox->width;
	if (bbox->height > self->height)
		self->height = bbox->height;
	if (bbox->depth > self->depth)
		self->depth = bbox->depth;
}

void
gmathml_bbox_add_over (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		return;
	}

	self->height += bbox->height + bbox->depth;
	self->width = MAX (self->width, bbox->width);
}

void
gmathml_bbox_add_under (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		return;
	}

	self->depth += bbox->height + bbox->depth;
	self->width = MAX (self->width, bbox->width);
}

void
gmathml_bbox_merge_vertically (GMathmlBbox *self, const GMathmlBbox *bbox, double offset)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		self->height += offset;
		self->depth -= offset;
		return;
	}

	if (bbox->height + offset > self->height)
		self->height = bbox->height + offset;
	if (bbox->depth - offset > self->depth)
		self->depth = bbox->depth - offset;
	self->width = MAX (self->width, bbox->width);
}

void
gmathml_bbox_stretch (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		return;
	}

	if (bbox->height > self->height)
		self->height = bbox->height;
	if (bbox->depth > self->depth)
		self->depth = bbox->depth;
	if (bbox->width > self->width)
		self->width = bbox->width;
}

void
gmathml_bbox_stretch_vertically (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		*self = *bbox;
		self->width = 0;
		return;
	}

	if (bbox->height > self->height)
		self->height = bbox->height;
	if (bbox->depth > self->depth)
		self->depth = bbox->depth;
}

void
gmathml_bbox_stretch_horizontally (GMathmlBbox *self, const GMathmlBbox *bbox)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (bbox != NULL);

	if (!bbox->is_defined)
		return;

	if (!self->is_defined) {
		self->width = bbox->width;
		self->height = -G_MAXDOUBLE;
		self->depth = -G_MAXDOUBLE;
		self->is_defined = TRUE;
		return;
	}

	if (bbox->width > self->width)
		self->width = bbox->width;
}
