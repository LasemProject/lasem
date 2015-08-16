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

#include <lsmsvgfilterdisplacementmap.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_displacement_map_get_node_name (LsmDomNode *node)
{
	return "feDisplacementMap";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_displacement_map_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
			     const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterDisplacementMap *displacement_map = LSM_SVG_FILTER_DISPLACEMENT_MAP (self);

	lsm_svg_view_apply_displacement_map (view, input, displacement_map->in2.value, output, subregion,
					     displacement_map->scale.value,
					     displacement_map->x_channel_selector.value,
					     displacement_map->y_channel_selector.value);
}

/* LsmSvgFilterDisplacementMap implementation */

LsmDomNode *
lsm_svg_filter_displacement_map_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_DISPLACEMENT_MAP, NULL);
}

static double scale_default = 0.0; 
static LsmSvgChannelSelector channel_selector_default = LSM_SVG_CHANNEL_SELECTOR_ALPHA;

static void
lsm_svg_filter_displacement_map_init (LsmSvgFilterDisplacementMap *self)
{
}

static void
lsm_svg_filter_displacement_map_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterDisplacementMap class */

static const LsmAttributeInfos lsm_svg_filter_displacement_map_attribute_infos[] = {
	{
		.name = "in2",
		.attribute_offset = offsetof (LsmSvgFilterDisplacementMap, in2),
		.trait_class = &lsm_null_trait_class
	},
	{
		.name = "scale",
		.attribute_offset = offsetof (LsmSvgFilterDisplacementMap, scale),
		.trait_class = &lsm_double_trait_class,
		.trait_default = &scale_default
	},
	{
		.name = "xChannelSelector",
		.attribute_offset = offsetof (LsmSvgFilterDisplacementMap, x_channel_selector),
		.trait_class = &lsm_svg_channel_selector_trait_class,
		.trait_default = &channel_selector_default
	},
	{
		.name = "yChannelSelector",
		.attribute_offset = offsetof (LsmSvgFilterDisplacementMap, y_channel_selector),
		.trait_class = &lsm_svg_channel_selector_trait_class,
		.trait_default = &channel_selector_default
	}
};

static void
lsm_svg_filter_displacement_map_class_init (LsmSvgFilterDisplacementMapClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_displacement_map_finalize;

	d_node_class->get_node_name = lsm_svg_filter_displacement_map_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_displacement_map_attribute_infos),
					      lsm_svg_filter_displacement_map_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_displacement_map_apply;
}

G_DEFINE_TYPE (LsmSvgFilterDisplacementMap, lsm_svg_filter_displacement_map, LSM_TYPE_SVG_FILTER_PRIMITIVE)
