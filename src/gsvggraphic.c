/*
 * Copyright © 2009 Emmanuel Pacaud
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

#include <gsvggraphic.h>

static GObjectClass *parent_class;

/* GSvgGraphic implementation */

static void
gsvg_graphic_element_init (GSvgGraphic *self)
{
}

static void
gsvg_graphic_element_finalize (GObject *object)
{
}

/* GSvgGraphic class */

static void
gsvg_graphic_element_class_init (GSvgGraphicClass *s_graphic_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_graphic_class);

	parent_class = g_type_class_peek_parent (s_graphic_class);

	object_class->finalize = gsvg_graphic_element_finalize;
}

G_DEFINE_ABSTRACT_TYPE (GSvgGraphic, gsvg_graphic_element, GSVG_TYPE_ELEMENT)

