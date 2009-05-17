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
#include <string.h>

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
	const char *href;

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

	lsm_svg_preserve_aspect_ratio_attribute_parse (&image_element->preserve_aspect_ratio);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);

	if (image_element->pixbuf != NULL) {
		g_object_unref (image_element->pixbuf);
		image_element->pixbuf = NULL;
	}

	href = image_element->href.value;

	if (href != NULL) {
		GdkPixbufLoader *loader;
		void *image_data;
		gsize data_size;
		int result;

		if (strncmp (href, "data:", 5) == 0) {

			lsm_debug ("[LsmSvgImageElement::update] Found inlined image");

			while (*href != '\0' && *href != ',')
				href++;

			image_data = g_base64_decode (href, &data_size);
		} else {
			image_data = NULL;
			data_size = 0;
		}

		loader = gdk_pixbuf_loader_new ();

		result = gdk_pixbuf_loader_write (loader, image_data, data_size, NULL);

		g_free (image_data);

		gdk_pixbuf_loader_close (loader, NULL);

		image_element->pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
		g_object_ref (image_element->pixbuf);

		g_object_unref (loader);
	}
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_image_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgImageElement *image;
	LsmBox viewport;
	LsmBox viewbox;

	image = LSM_SVG_IMAGE_ELEMENT (self);
	if (image->pixbuf == NULL)
		return;

	viewport.x      = lsm_svg_view_normalize_length (view, &image->x.length.base,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.y      = lsm_svg_view_normalize_length (view, &image->y.length.base,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);
	viewport.width  = lsm_svg_view_normalize_length (view, &image->width.length.base,
							 LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.height = lsm_svg_view_normalize_length (view, &image->height.length.base,
							 LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	lsm_debug ("[LsmSvgImageElement::graphic_render] viewport %g, %g, %g, %g",
		   viewport.x, viewport.y, viewport.width, viewport.height);

	viewbox.x = 0;
	viewbox.y = 0;
	viewbox.width = gdk_pixbuf_get_width (image->pixbuf);
	viewbox.height = gdk_pixbuf_get_height (image->pixbuf);

	lsm_svg_view_push_viewport (view, &viewport, &viewbox, &image->preserve_aspect_ratio.value);

	lsm_svg_view_show_pixbuf (view, image->pixbuf);

	lsm_svg_view_pop_viewport (view);
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
	self->pixbuf = NULL;
}

static void
lsm_svg_image_element_finalize (GObject *gobject)
{
	LsmSvgImageElement *image = LSM_SVG_IMAGE_ELEMENT (gobject);

	if (image->pixbuf != NULL)
		g_object_unref (image->pixbuf);
}

/* LsmSvgImageElement class */

static void
lsm_svg_image_element_class_init (LsmSvgImageElementClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	gobject_class->finalize = lsm_svg_image_element_finalize;

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
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "preserveAspectRatio",
					     offsetof (LsmSvgImageElement, preserve_aspect_ratio));
}

G_DEFINE_TYPE (LsmSvgImageElement, lsm_svg_image_element, LSM_TYPE_SVG_GRAPHIC)
