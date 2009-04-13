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

#include <lsmsvgsvgelement.h>
#include <lsmsvgstyle.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_svg_element_get_node_name (LsmDomNode *node)
{
	return "svg";
}

/* LsmSvgElement implementation */

static void
_svg_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgSvgElement *svg = LSM_SVG_SVG_ELEMENT (self);
	LsmSvgLength length;
	LsmSvgViewBox view_box = {0,0,0,0};

	lsm_svg_view_box_attribute_parse (&svg->view_box, &view_box);

	length.value = svg->view_box.value.x;
	length.value_unit = svg->view_box.value.x;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->x, &length, 0.0);

	length.value = svg->view_box.value.y;
	length.value_unit = svg->view_box.value.y;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->y, &length, 0.0);

	length.value = svg->view_box.value.width;
	length.value_unit = svg->view_box.value.width;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->width, &length, 0.0);

	length.value = svg->view_box.value.height;
	length.value_unit = svg->view_box.value.height;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->height, &length, 0.0);

	lsm_debug ("[LsmSvgSvgElement::update] height = %g, width = %g",
		    svg->height.length.value,
		    svg->width.length.value);

	lsm_debug ("[LsmSvgSvgElement::update] view_bbox = %g, %g, %g, %g\n",
		    svg->view_box.value.x,
		    svg->view_box.value.y,
		    svg->view_box.value.width,
		    svg->view_box.value.height);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

void
lsm_svg_svg_element_measure (LsmSvgSvgElement *self, double *width, double *height)
{
	g_return_if_fail (LSM_SVG_IS_SVG_ELEMENT (self));

	if (width != NULL)
		*width = self->width.length.value;
	if (height != NULL)
		*height = self->height.length.value;
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_svg_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgSvgElement *svg = LSM_SVG_SVG_ELEMENT (self);
	LsmSvgMatrix matrix;

	if (svg->view_box.value.width <= 0.0 ||
	    svg->view_box.value.height <= 0.0)
		return;

	lsm_svg_matrix_init (&matrix,
			  svg->width.length.value / svg->view_box.value.width,
			  0, 0,
			  svg->height.length.value / svg->view_box.value.height,
			  0, 0);
	lsm_svg_view_push_transform (view, &matrix);

	LSM_SVG_GRAPHIC_CLASS (parent_class)->graphic_render (self, view);

	lsm_svg_view_pop_transform (view);
}

/* LsmSvgSvgElement implementation */

LsmSvgStyle *
lsm_svg_svg_element_get_default_style (LsmSvgSvgElement *svg_element)
{
	g_return_val_if_fail (LSM_SVG_IS_SVG_ELEMENT (svg_element), NULL);

	return svg_element->default_style;
}

void
lsm_svg_svg_element_update (LsmSvgSvgElement *svg_element)
{
	lsm_svg_element_update (LSM_SVG_ELEMENT (svg_element),
			     lsm_svg_svg_element_get_default_style (svg_element));
}

LsmDomNode *
lsm_svg_svg_element_new (void)
{
	return g_object_new (LSM_SVG_TYPE_SVG_ELEMENT, NULL);
}

static void
lsm_svg_svg_element_init (LsmSvgSvgElement *self)
{
	LsmSvgStyle *style;

	style = lsm_svg_style_new ();
	self->default_style = style;
	g_return_if_fail (style != NULL);

	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "fill", 			"black");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "fill-opacity", 		"1");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "fill-rule",		"nonzero");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke", 		"none");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke-width", 		"1px");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke-opacity", 	"1");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke-linejoin", 	"miter");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke-linecap", 	"butt");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stroke-miterlimit", 	"4");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "font-family", 		"sans");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "font-size", 		"10pt");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stop-color", 		"black");
	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "stop-opacity", 		"1");

}

static void
lsm_svg_svg_element_finalize (GObject *object)
{
	LsmSvgSvgElement *svg_element = LSM_SVG_SVG_ELEMENT (object);

	lsm_svg_style_free (svg_element->default_style);
}

/* LsmSvgSvgElement class */

static void
lsm_svg_svg_element_class_init (LsmSvgSvgElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_svg_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_svg_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = lsm_svg_svg_element_finalize;

	d_node_class->get_node_name = lsm_svg_svg_element_get_node_name;

	s_element_class->update = _svg_element_update;

	s_graphic_class->graphic_render = lsm_svg_svg_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgSvgElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgSvgElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (LsmSvgSvgElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (LsmSvgSvgElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "viewBox",
					  offsetof (LsmSvgSvgElement, view_box));
}

G_DEFINE_TYPE (LsmSvgSvgElement, lsm_svg_svg_element, LSM_SVG_TYPE_GRAPHIC)
