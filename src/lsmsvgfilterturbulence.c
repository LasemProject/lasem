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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgfilterturbulence.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_turbulence_get_node_name (LsmDomNode *node)
{
	return "feTurbulence";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_turbulence_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
			     const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterTurbulence *turbulence = LSM_SVG_FILTER_TURBULENCE (self);

	lsm_svg_view_apply_turbulence (view, output, subregion,
				       turbulence->base_frequency.value.a,
				       turbulence->base_frequency.value.b,
				       turbulence->n_octaves.value,
				       turbulence->seed.value,
				       turbulence->stitch_tiles.value,
				       turbulence->type.value);
}

/* LsmSvgFilterTurbulence implementation */

LsmDomNode *
lsm_svg_filter_turbulence_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_TURBULENCE, NULL);
}

static const LsmSvgOneOrTwoDouble base_frequency_default = {0.0, 0.0};
static const int n_octaves_default = 1;
static const double seed_default = 0.0;
static const LsmSvgStitchTiles stitch_tiles_default = LSM_SVG_STITCH_TILES_NO_STITCH;
static const LsmSvgTurbulenceType type_default = LSM_SVG_TURBULENCE_TYPE_TURBULENCE;

static void
lsm_svg_filter_turbulence_init (LsmSvgFilterTurbulence *self)
{
	self->base_frequency.value = base_frequency_default;
	self->n_octaves.value = n_octaves_default;
	self->seed.value = seed_default;
	self->stitch_tiles.value = stitch_tiles_default;
	self->type.value = type_default;
}

static void
lsm_svg_filter_turbulence_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterTurbulence class */

static const LsmAttributeInfos lsm_svg_filter_turbulence_attribute_infos[] = {
	{
		.name = "baseFrequency",
		.attribute_offset = offsetof (LsmSvgFilterTurbulence, base_frequency),
		.trait_class = &lsm_svg_one_or_two_double_trait_class,
 		.trait_default = &base_frequency_default
	},
	{
		.name = "numOctaves",
		.attribute_offset = offsetof (LsmSvgFilterTurbulence, n_octaves),
		.trait_class = &lsm_integer_trait_class,
		.trait_default = &n_octaves_default
	},
	{
		.name = "seed",
		.attribute_offset = offsetof (LsmSvgFilterTurbulence, seed),
		.trait_class = &lsm_double_trait_class,
		.trait_default = &seed_default
	},
	{
		.name = "stitchTiles",
		.attribute_offset = offsetof (LsmSvgFilterTurbulence, stitch_tiles),
		.trait_class = &lsm_svg_stitch_tiles_trait_class,
		.trait_default = &stitch_tiles_default
	},
	{
		.name = "type",
		.attribute_offset = offsetof (LsmSvgFilterTurbulence, type),
		.trait_class = &lsm_svg_turbulence_type_trait_class,
		.trait_default = &type_default
	}
};

static void
lsm_svg_filter_turbulence_class_init (LsmSvgFilterTurbulenceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_turbulence_finalize;

	d_node_class->get_node_name = lsm_svg_filter_turbulence_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_turbulence_attribute_infos),
					      lsm_svg_filter_turbulence_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_turbulence_apply;
}

G_DEFINE_TYPE (LsmSvgFilterTurbulence, lsm_svg_filter_turbulence, LSM_TYPE_SVG_FILTER_PRIMITIVE)
