/* Lasem
 * 
 * Copyright © 2012 Emmanuel Pacaud
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

#include <lsmsvgfilterblend.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_blend_get_node_name (LsmDomNode *node)
{
	return "feBlend";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_blend_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
			     const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterBlend *blend = LSM_SVG_FILTER_BLEND (self);

	lsm_svg_view_apply_blend (view, input, blend->in2.value, output, subregion, blend->mode.value);
}

/* LsmSvgFilterBlend implementation */

LsmDomNode *
lsm_svg_filter_blend_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_BLEND, NULL);
}

static void
lsm_svg_filter_blend_init (LsmSvgFilterBlend *self)
{
}

static void
lsm_svg_filter_blend_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterBlend class */

static const LsmAttributeInfos lsm_svg_filter_blend_attribute_infos[] = {
	{
		.name = "in2",
		.attribute_offset = offsetof (LsmSvgFilterBlend, in2),
		.trait_class = &lsm_null_trait_class
	},
	{
		.name = "mode",
		.attribute_offset = offsetof (LsmSvgFilterBlend, mode),
		.trait_class = &lsm_svg_blending_mode_trait_class
	}
};

static void
lsm_svg_filter_blend_class_init (LsmSvgFilterBlendClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_blend_finalize;

	d_node_class->get_node_name = lsm_svg_filter_blend_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_blend_attribute_infos),
					      lsm_svg_filter_blend_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_blend_apply;
}

G_DEFINE_TYPE (LsmSvgFilterBlend, lsm_svg_filter_blend, LSM_TYPE_SVG_FILTER_PRIMITIVE)
