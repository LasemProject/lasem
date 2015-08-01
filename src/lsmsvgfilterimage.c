/* Lasem
 * 
 * Copyright © 2015 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgfilterimage.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_image_get_node_name (LsmDomNode *node)
{
	return "feImage";
}

static void
lsm_svg_filter_image_set_attribute (LsmDomElement *self, const char *name, const char *value)
{
	LsmSvgFilterImage *filter = LSM_SVG_FILTER_IMAGE (self);

	LSM_DOM_ELEMENT_CLASS (parent_class)->set_attribute (self, name, value);

	if (g_strcmp0 (name, "xlink:href") == 0)
		g_clear_object (&filter->pixbuf);
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_image_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
			     const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterImage *filter = LSM_SVG_FILTER_IMAGE (self);

	/* TODO href can also be a reference to an element */ 
	/* TODO this code can be factorized with the one of <image> */

	if (filter->pixbuf == NULL) {
		LsmDomDocument *document;
		char *data;
		gsize size;

		document = lsm_dom_node_get_owner_document (LSM_DOM_NODE (self));

		if (filter->href.value != NULL) {
			data = lsm_dom_document_get_href_data (document, filter->href.value, &size);
			if (data != NULL) {
				GdkPixbufLoader *loader;

				loader = gdk_pixbuf_loader_new ();

				gdk_pixbuf_loader_write (loader, (guchar *) data, size, NULL);

				g_free (data);

				gdk_pixbuf_loader_close (loader, NULL);

				filter->pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
				g_object_ref (filter->pixbuf);

				g_object_unref (loader);
			} else
				lsm_debug_render ("[SvgFilterImage::apply] Failed to load image '%s'",
						  filter->href.value);
		} else
			lsm_debug_render ("[SvgFilterImage::apply] Missing xlink:href attribute");
	}

	if (GDK_IS_PIXBUF (filter->pixbuf))
		lsm_svg_view_apply_image (view, output, subregion, filter->pixbuf, filter->preserve_aspect_ratio.value);
}

/* LsmSvgFilterImage implementation */

LsmDomNode *
lsm_svg_filter_image_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_IMAGE, NULL);
}

static const LsmSvgPreserveAspectRatio preserve_aspect_ratio_default = {
	.defer = FALSE,
	.align = LSM_SVG_ALIGN_X_MID_Y_MID,
	.meet_or_slice = LSM_SVG_MEET_OR_SLICE_MEET
};

static void
lsm_svg_filter_image_init (LsmSvgFilterImage *self)
{
	self->pixbuf = NULL;
	self->preserve_aspect_ratio.value = preserve_aspect_ratio_default;
}

static void
lsm_svg_filter_image_finalize (GObject *object)
{
	LsmSvgFilterImage *filter = LSM_SVG_FILTER_IMAGE (object);

	g_clear_object (&filter->pixbuf);

	parent_class->finalize (object);
}

/* LsmSvgFilterImage class */

static const LsmAttributeInfos lsm_svg_filter_image_attribute_infos[] = {
	{
		.name = "xlink:href",
		.attribute_offset = offsetof (LsmSvgFilterImage, href),
		.trait_class = &lsm_null_trait_class
	},
	{
		.name = "preserveAspectRatio",
		.attribute_offset = offsetof (LsmSvgFilterImage, preserve_aspect_ratio),
		.trait_class = &lsm_svg_preserve_aspect_ratio_trait_class,
		.trait_default = &preserve_aspect_ratio_default
	}
};

static void
lsm_svg_filter_image_class_init (LsmSvgFilterImageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmDomElementClass *d_element_class = LSM_DOM_ELEMENT_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_image_finalize;

	d_node_class->get_node_name = lsm_svg_filter_image_get_node_name;

	d_element_class->set_attribute = lsm_svg_filter_image_set_attribute;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_image_attribute_infos),
					      lsm_svg_filter_image_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_image_apply;
}

G_DEFINE_TYPE (LsmSvgFilterImage, lsm_svg_filter_image, LSM_TYPE_SVG_FILTER_PRIMITIVE)
