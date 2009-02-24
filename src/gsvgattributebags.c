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

static void *
gsvg_fill_attribute_bag_init (void)
{
	GSvgFillAttributeBag *fill;

	fill = g_new0 (GSvgFillAttributeBag, 1);
	return fill;
}

static void
gsvg_fill_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const GDomAttributeBagClass gsvg_fill_attribute_bag_class =
{
	.init = gsvg_fill_attribute_bag_init,
	.finalize = gsvg_fill_attribute_bag_finalize
};

static const GDomAttributeClass gsvg_paint_attribute_class = {
	.finalize = gsvg_paint_attribute_finalize
};

void
gdom_attribute_map_add_fill_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
	gdom_attribute_map_add_bag_attribute (map, "fill",
					      offsetof (GSvgFillAttributeBag, paint),
					      &gsvg_paint_attribute_class,
					      bag_offset, &gsvg_fill_attribute_bag_class);
	gdom_attribute_map_add_bag_attribute (map, "fill-opacity",
					      offsetof (GSvgFillAttributeBag, opacity),
					      NULL,
					      bag_offset, &gsvg_fill_attribute_bag_class);
}

static void *
gsvg_stroke_attribute_bag_init (void)
{
	GSvgStrokeAttributeBag *stroke;

	stroke = g_new0 (GSvgStrokeAttributeBag, 1);
	return stroke;
}

static void
gsvg_stroke_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const GDomAttributeBagClass gsvg_stroke_attribute_bag_class =
{
	.init = gsvg_stroke_attribute_bag_init,
	.finalize = gsvg_stroke_attribute_bag_finalize
};

void
gdom_attribute_map_add_stroke_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
	gdom_attribute_map_add_bag_attribute (map, "stroke",
					      offsetof (GSvgStrokeAttributeBag, paint),
					      &gsvg_paint_attribute_class,
					      bag_offset, &gsvg_stroke_attribute_bag_class);
	gdom_attribute_map_add_bag_attribute (map, "stroke-width",
					      offsetof (GSvgStrokeAttributeBag, width),
					      NULL,
					      bag_offset, &gsvg_stroke_attribute_bag_class);
	gdom_attribute_map_add_bag_attribute (map, "stroke-opacity",
					      offsetof (GSvgStrokeAttributeBag, opacity),
					      NULL,
					      bag_offset, &gsvg_stroke_attribute_bag_class);
}

static void *
gsvg_transform_attribute_bag_init (void)
{
	GSvgTransformAttributeBag *transform;

	transform = g_new0 (GSvgTransformAttributeBag, 1);
	return transform;
}

static void
gsvg_transform_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const GDomAttributeBagClass gsvg_transform_attribute_bag_class =
{
	.init = gsvg_transform_attribute_bag_init,
	.finalize = gsvg_transform_attribute_bag_finalize
};

void
gdom_attribute_map_add_transform_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
	gdom_attribute_map_add_bag_attribute (map, "transform",
					      offsetof (GSvgTransformAttributeBag, transform),
					      NULL,
					      bag_offset, &gsvg_transform_attribute_bag_class);
}

static void *
gsvg_text_attribute_bag_init (void)
{
	GSvgTextAttributeBag *text;

	text = g_new0 (GSvgTextAttributeBag, 1);
	return text;
}

static void
gsvg_text_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const GDomAttributeBagClass gsvg_text_attribute_bag_class =
{
	.init = gsvg_text_attribute_bag_init,
	.finalize = gsvg_text_attribute_bag_finalize
};

static const GDomAttributeClass gdom_string_attribute_class = {
	.finalize = gdom_string_attribute_finalize
};

void
gdom_attribute_map_add_text_attribute_bag (GDomAttributeMap *map, ptrdiff_t bag_offset)
{
	gdom_attribute_map_add_bag_attribute (map, "font-family",
					      offsetof (GSvgTextAttributeBag, font_family),
					      &gdom_string_attribute_class,
					      bag_offset, &gsvg_text_attribute_bag_class);

	gdom_attribute_map_add_bag_attribute (map, "font-size",
					      offsetof (GSvgTextAttributeBag, font_size),
					      NULL,
					      bag_offset, &gsvg_text_attribute_bag_class);
}
