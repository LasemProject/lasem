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

#include <lsmsvgimageelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <lsmdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_image_element_get_node_name (LsmDomNode *node)
{
	return "image";
}

static gboolean
lsm_svg_use_can_append_child (LsmDomNode *node, LsmDomNode *child)
{
	return FALSE;
}

/* LsmSvgElement implementation */

static void
lsm_svg_image_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgImageElement *image_element = LSM_SVG_IMAGE_ELEMENT (self);
	LsmSvgLength length;

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&image_element->x, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&image_element->y, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&image_element->width, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&image_element->height, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_image_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgImageElement *image_element;
	LsmDomDocument *document;
	const char *id;

	document = lsm_dom_node_get_owner_document (LSM_DOM_NODE (self));
	if (document == NULL) {
		lsm_debug ("[LsmSvgImageElement::graphic_render] Owner document not found");
		return;
	}

	image_element = LSM_SVG_IMAGE_ELEMENT (self);

	id = image_element->href.value;
	if (id == NULL)
		return;
}

/* LsmSvgImageElement implementation */

LsmDomNode *
lsm_svg_image_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_IMAGE_ELEMENT, NULL);
}

static void
lsm_svg_image_element_init (LsmSvgImageElement *self)
{
}

/* LsmSvgImageElement class */

static void
lsm_svg_image_element_class_init (LsmSvgImageElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_svg_image_element_get_node_name;
	d_node_class->can_append_child = lsm_svg_use_can_append_child;

	s_element_class->update = lsm_svg_image_element_update;

	s_graphic_class->graphic_render = lsm_svg_image_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgImageElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgImageElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (LsmSvgImageElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (LsmSvgImageElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (LsmSvgImageElement, href));
}

G_DEFINE_TYPE (LsmSvgImageElement, lsm_svg_image_element, LSM_TYPE_SVG_GRAPHIC)
