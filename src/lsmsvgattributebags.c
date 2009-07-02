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

#include <lsmsvgattributebags.h>

static void *
lsm_svg_mask_attribute_bag_init (void)
{
	LsmSvgMaskAttributeBag *mask;

	mask = g_new0 (LsmSvgMaskAttributeBag, 1);
	return mask;
}

static void
lsm_svg_mask_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_mask_attribute_bag_class =
{
	.init = lsm_svg_mask_attribute_bag_init,
	.finalize = lsm_svg_mask_attribute_bag_finalize
};

void
lsm_dom_attribute_map_add_mask_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "opacity",
						 offsetof (LsmSvgMaskAttributeBag, opacity),
						 NULL,
						 bag_offset, &lsm_svg_mask_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "clip-path",
						 offsetof (LsmSvgMaskAttributeBag, clip_path),
						 NULL,
						 bag_offset, &lsm_svg_mask_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "clip-rule",
						 offsetof (LsmSvgMaskAttributeBag, clip_rule),
						 NULL,
						 bag_offset, &lsm_svg_mask_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "mask",
						 offsetof (LsmSvgMaskAttributeBag, mask),
						 NULL,
						 bag_offset, &lsm_svg_mask_attribute_bag_class);
}

static void *
lsm_svg_fill_attribute_bag_init (void)
{
	LsmSvgFillAttributeBag *fill;

	fill = g_new0 (LsmSvgFillAttributeBag, 1);
	return fill;
}

static void
lsm_svg_fill_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_fill_attribute_bag_class =
{
	.init = lsm_svg_fill_attribute_bag_init,
	.finalize = lsm_svg_fill_attribute_bag_finalize
};

static const LsmDomAttributeClass lsm_svg_paint_attribute_class = {
	.finalize = lsm_svg_paint_attribute_finalize
};

void
lsm_dom_attribute_map_add_fill_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "fill",
						 offsetof (LsmSvgFillAttributeBag, paint),
						 &lsm_svg_paint_attribute_class,
						 bag_offset, &lsm_svg_fill_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "fill-rule",
						 offsetof (LsmSvgFillAttributeBag, rule),
						 NULL,
						 bag_offset, &lsm_svg_fill_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "fill-opacity",
						 offsetof (LsmSvgFillAttributeBag, opacity),
						 NULL,
						 bag_offset, &lsm_svg_fill_attribute_bag_class);
}

static void *
lsm_svg_stroke_attribute_bag_init (void)
{
	LsmSvgStrokeAttributeBag *stroke;

	stroke = g_new0 (LsmSvgStrokeAttributeBag, 1);
	return stroke;
}

static void
lsm_svg_stroke_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_stroke_attribute_bag_class =
{
	.init = lsm_svg_stroke_attribute_bag_init,
	.finalize = lsm_svg_stroke_attribute_bag_finalize
};

static const LsmDomAttributeClass lsm_svg_dash_array_attribute_class = {
	.finalize = lsm_svg_dash_array_attribute_finalize
};

void
lsm_dom_attribute_map_add_stroke_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke",
						 offsetof (LsmSvgStrokeAttributeBag, paint),
						 &lsm_svg_paint_attribute_class,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-width",
						 offsetof (LsmSvgStrokeAttributeBag, width),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-opacity",
						 offsetof (LsmSvgStrokeAttributeBag, opacity),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-linejoin",
						 offsetof (LsmSvgStrokeAttributeBag, line_join),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-linecap",
						 offsetof (LsmSvgStrokeAttributeBag, line_cap),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-miterlimit",
						 offsetof (LsmSvgStrokeAttributeBag, miter_limit),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-dasharray",
						 offsetof (LsmSvgStrokeAttributeBag, dash_array),
						 &lsm_svg_dash_array_attribute_class,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
	lsm_dom_attribute_map_add_bag_attribute (map, "stroke-dashoffset",
						 offsetof (LsmSvgStrokeAttributeBag, dash_offset),
						 NULL,
						 bag_offset, &lsm_svg_stroke_attribute_bag_class);
}

static void *
lsm_svg_transform_attribute_bag_init (void)
{
	LsmSvgTransformAttributeBag *transform;

	transform = g_new0 (LsmSvgTransformAttributeBag, 1);
	return transform;
}

static void
lsm_svg_transform_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_transform_attribute_bag_class =
{
	.init = lsm_svg_transform_attribute_bag_init,
	.finalize = lsm_svg_transform_attribute_bag_finalize
};

void
lsm_dom_attribute_map_add_transform_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "transform",
						 offsetof (LsmSvgTransformAttributeBag, transform),
						 NULL,
						 bag_offset, &lsm_svg_transform_attribute_bag_class);
}

static void *
lsm_svg_text_attribute_bag_init (void)
{
	LsmSvgTextAttributeBag *text;

	text = g_new0 (LsmSvgTextAttributeBag, 1);
	return text;
}

static void
lsm_svg_text_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_text_attribute_bag_class =
{
	.init = lsm_svg_text_attribute_bag_init,
	.finalize = lsm_svg_text_attribute_bag_finalize
};

static const LsmDomAttributeClass lsm_dom_string_attribute_class = {
	.finalize = lsm_dom_string_attribute_finalize
};

void
lsm_dom_attribute_map_add_text_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "font-family",
						 offsetof (LsmSvgTextAttributeBag, font_family),
						 &lsm_dom_string_attribute_class,
						 bag_offset, &lsm_svg_text_attribute_bag_class);

	lsm_dom_attribute_map_add_bag_attribute (map, "font-size",
						 offsetof (LsmSvgTextAttributeBag, font_size),
						 NULL,
						 bag_offset, &lsm_svg_text_attribute_bag_class);
}

static void *
lsm_svg_stop_attribute_bag_init (void)
{
	LsmSvgStopAttributeBag *stop;

	stop = g_new0 (LsmSvgStopAttributeBag, 1);
	return stop;
}

static void
lsm_svg_stop_attribute_bag_finalize (void *bag)
{
	g_free (bag);
}

static const LsmDomAttributeBagClass lsm_svg_stop_attribute_bag_class =
{
	.init = lsm_svg_stop_attribute_bag_init,
	.finalize = lsm_svg_stop_attribute_bag_finalize
};

void
lsm_dom_attribute_map_add_stop_attribute_bag (LsmDomAttributeMap *map, ptrdiff_t bag_offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, "stop-color",
					      offsetof (LsmSvgStopAttributeBag, color),
					      NULL,
					      bag_offset, &lsm_svg_stop_attribute_bag_class);

	lsm_dom_attribute_map_add_bag_attribute (map, "stop-opacity",
					      offsetof (LsmSvgStopAttributeBag, opacity),
					      NULL,
					      bag_offset, &lsm_svg_stop_attribute_bag_class);
}
