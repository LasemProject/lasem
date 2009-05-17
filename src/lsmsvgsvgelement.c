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
#include <lsmdomdocument.h>
#include <math.h>
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

	length.value_unit = 0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->x, &length);

	length.value_unit = 0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->y, &length);

	length.value_unit = parent_style->viewport.width;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->width, &length);

	length.value_unit = parent_style->viewport.height;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&svg->height, &length);

	lsm_svg_viewbox_attribute_parse (&svg->viewbox);
	lsm_svg_preserve_aspect_ratio_attribute_parse (&svg->preserve_aspect_ratio);

	lsm_debug ("[LsmSvgSvgElement::update] view_bbox = %g, %g, %g, %g\n",
		    svg->viewbox.value.x,
		    svg->viewbox.value.y,
		    svg->viewbox.value.width,
		    svg->viewbox.value.height);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);

	parent_style->viewport = svg->viewbox.value;
}

void
lsm_svg_svg_element_measure (LsmSvgSvgElement *self, double *width, double *height)
{
	LsmDomDocument *document;
	LsmSvgViewbox *svg_viewbox;
	LsmBox viewport;
	double resolution_ppi;
	double svg_x;
	double svg_y;
	double svg_width;
	double svg_height;
	double font_size;

	g_return_if_fail (LSM_IS_SVG_SVG_ELEMENT (self));

	document = LSM_DOM_DOCUMENT (LSM_DOM_NODE (self)->parent_node);
	g_return_if_fail (LSM_IS_DOM_DOCUMENT (document));

	resolution_ppi = lsm_dom_document_get_resolution (document);
	viewport = lsm_dom_document_get_viewport_px (document);

	svg_viewbox = lsm_svg_viewbox_new (resolution_ppi, &viewport);
	font_size = 10 * resolution_ppi / 72.0;

	svg_x      = lsm_svg_length_normalize (&self->x.length, svg_viewbox,
					       font_size, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	svg_y      = lsm_svg_length_normalize (&self->y.length, svg_viewbox,
					       font_size, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	svg_width  = lsm_svg_length_normalize (&self->width.length, svg_viewbox,
					       font_size, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	svg_height = lsm_svg_length_normalize (&self->height.length, svg_viewbox,
					       font_size, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	if (width != NULL)
		*width = svg_width * 72.0 / resolution_ppi;
	if (height != NULL)
		*height = svg_height * 72.0 / resolution_ppi;

	self->svg_box.x = svg_x;
	self->svg_box.y = svg_y;
	self->svg_box.width = svg_width;
	self->svg_box.height = svg_height;

	lsm_debug ("[LsmSvgSvgElement::measure] Size = %g, %g, %g, %g",
		   svg_x, svg_y, svg_width, svg_height);

	lsm_svg_viewbox_free (svg_viewbox);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_svg_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgSvgElement *svg = LSM_SVG_SVG_ELEMENT (self);
	gboolean is_viewbox_defined;
	LsmBox viewport;

	viewport.x      = lsm_svg_view_normalize_length (view, &svg->x.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.y      = lsm_svg_view_normalize_length (view, &svg->y.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	viewport.width  = lsm_svg_view_normalize_length (view, &svg->width.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.height = lsm_svg_view_normalize_length (view, &svg->height.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	is_viewbox_defined = lsm_dom_attribute_is_defined ((LsmDomAttribute *) &svg->viewbox);

	if (is_viewbox_defined && (svg->viewbox.value.width <= 0.0 ||
				   svg->viewbox.value.height <= 0.0))
		return;

	lsm_debug ("[LsmSvgSvgElement::graphic_render] viewport %g, %g, %g, %g",
		   viewport.x, viewport.y, viewport.width, viewport.height);

	lsm_svg_view_push_viewport (view, &viewport, is_viewbox_defined ? &svg->viewbox.value : NULL,
				    &svg->preserve_aspect_ratio.value);

	LSM_SVG_GRAPHIC_CLASS (parent_class)->graphic_render (self, view);

	lsm_svg_view_pop_viewport (view);
}

/* LsmSvgSvgElement implementation */

LsmSvgStyle *
lsm_svg_svg_element_get_default_style (LsmSvgSvgElement *svg_element)
{
	g_return_val_if_fail (LSM_IS_SVG_SVG_ELEMENT (svg_element), NULL);

	return svg_element->default_style;
}

void
lsm_svg_svg_element_update (LsmSvgSvgElement *svg_element)
{
	LsmDomDocument *document;
	LsmSvgStyle *style;

	g_return_if_fail (LSM_IS_SVG_SVG_ELEMENT (svg_element));

	document = LSM_DOM_DOCUMENT (LSM_DOM_NODE (svg_element)->parent_node);
	g_return_if_fail (LSM_IS_DOM_DOCUMENT (document));

	style = lsm_svg_svg_element_get_default_style (svg_element);

	style->viewport = lsm_dom_document_get_viewport_px (document);

	lsm_svg_element_update (LSM_SVG_ELEMENT (svg_element), style);
}

void
lsm_svg_svg_element_render (LsmSvgSvgElement *svg, LsmSvgView *view)
{
	lsm_svg_view_push_viewbox (view, &svg->svg_box);
	lsm_svg_element_render (LSM_SVG_ELEMENT (svg), view);
	lsm_svg_view_pop_viewbox (view);
}

LsmDomNode *
lsm_svg_svg_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_SVG_ELEMENT, NULL);
}

static void
lsm_svg_svg_element_init (LsmSvgSvgElement *self)
{
	LsmSvgStyle *style;

	style = lsm_svg_style_new ();
	self->default_style = style;
	g_return_if_fail (style != NULL);

	lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (self), "opacity", 		"1.0");
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
					     offsetof (LsmSvgSvgElement, viewbox));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "preserveAspectRatio",
					     offsetof (LsmSvgSvgElement, preserve_aspect_ratio));
}

G_DEFINE_TYPE (LsmSvgSvgElement, lsm_svg_svg_element, LSM_TYPE_SVG_GRAPHIC)
