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

#include <gsvgattributebags.h>

static const GDomAttributeBagClass gsvg_fill_attribute_bag_class = {};

void
gdom_attribute_map_add_fill_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
	gdom_attribute_map_add_bag_attribute (map, "fill", offsetof (GSvgFillAttributeBag, fill), NULL,
					      bag_offset, &gsvg_fill_attribute_bag_class);
}

void
gdom_attribute_map_add_stroke_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
}
