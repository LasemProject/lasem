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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgfiltermorphology.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_morphology_get_node_name (LsmDomNode *node)
{
	return "feMorphology";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_morphology_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
			     const char *input, const char *output, const LsmBox *subregion)
{
	double radius;

	LsmSvgFilterMorphology *morphology = LSM_SVG_FILTER_MORPHOLOGY (self);

	radius = lsm_svg_view_normalize_length (view, &morphology->radius.length, LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	lsm_svg_view_apply_morphology (view, input, output, subregion, morphology->op.value, radius);
}

/* LsmSvgFilterMorphology implementation */

LsmDomNode *
lsm_svg_filter_morphology_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_MORPHOLOGY, NULL);
}

static const LsmSvgLength radius_default = { .value_unit =   0.0, .type = LSM_SVG_LENGTH_TYPE_PX};
static const LsmSvgMorphologyOperator operator_default = LSM_SVG_MORPHOLOGY_OPERATOR_ERODE;

static void
lsm_svg_filter_morphology_init (LsmSvgFilterMorphology *self)
{
	self->radius.length = radius_default;
	self->op.value = operator_default;
}

static void
lsm_svg_filter_morphology_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterMorphology class */

static const LsmAttributeInfos lsm_svg_filter_morphology_attribute_infos[] = {
	{
		.name = "radius",
		.attribute_offset = offsetof (LsmSvgFilterMorphology, radius),
		.trait_class = &lsm_svg_length_trait_class,
		.trait_default = &radius_default
	},
	{
		.name = "operator",
		.attribute_offset = offsetof (LsmSvgFilterMorphology, op),
		.trait_class = &lsm_svg_morphology_operator_trait_class,
		.trait_default = &operator_default
	}
};

static void
lsm_svg_filter_morphology_class_init (LsmSvgFilterMorphologyClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_morphology_finalize;

	d_node_class->get_node_name = lsm_svg_filter_morphology_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_morphology_attribute_infos),
					      lsm_svg_filter_morphology_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_morphology_apply;
}

G_DEFINE_TYPE (LsmSvgFilterMorphology, lsm_svg_filter_morphology, LSM_TYPE_SVG_FILTER_PRIMITIVE)
