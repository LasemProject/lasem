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

#include <lsmsvgfilterconvolvematrix.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_convolve_matrix_get_node_name (LsmDomNode *node)
{
	return "feConvolveMatrix";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_convolve_matrix_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
				       const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterConvolveMatrix *convolve_matrix = LSM_SVG_FILTER_CONVOLVE_MATRIX (self);

	lsm_svg_view_apply_convolve_matrix (view, input, output, subregion,
					    convolve_matrix->order.value.a,
					    convolve_matrix->order.value.b,
					    convolve_matrix->kernel.value.n_values,
					    convolve_matrix->kernel.value.values,
					    convolve_matrix->edge_mode.value);
}

/* LsmSvgFilterConvolveMatrix implementation */

LsmDomNode *
lsm_svg_filter_convolve_matrix_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_CONVOLVE_MATRIX, NULL);
}

static const LsmSvgOneOrTwoInteger order_default = {0, 0};
static const LsmSvgVector kernel_default = { .n_values = 0, .values = NULL};
static const LsmSvgEdgeMode edge_mode_default = LSM_SVG_EDGE_MODE_NONE;

static void
lsm_svg_filter_convolve_matrix_init (LsmSvgFilterConvolveMatrix *self)
{
	self->order.value = order_default;
	self->kernel.value = kernel_default;
	self->edge_mode.value = edge_mode_default;
}

static void
lsm_svg_filter_convolve_matrix_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterConvolveMatrix class */

static const LsmAttributeInfos lsm_svg_filter_convolve_matrix_attribute_infos[] = {
	{
		.name = "order",
		.attribute_offset = offsetof (LsmSvgFilterConvolveMatrix, order),
		.trait_class = &lsm_svg_one_or_two_integer_trait_class,
		.trait_default = &order_default
	},
	{
		.name = "kernelMatrix",
		.attribute_offset = offsetof (LsmSvgFilterConvolveMatrix, kernel),
		.trait_class = &lsm_svg_vector_trait_class,
		.trait_default = &kernel_default
	},
	{
		.name = "edgeMode",
		.attribute_offset = offsetof (LsmSvgFilterConvolveMatrix, edge_mode),
		.trait_class = &lsm_svg_edge_mode_trait_class,
		.trait_default = &edge_mode_default
	}
};

static void
lsm_svg_filter_convolve_matrix_class_init (LsmSvgFilterConvolveMatrixClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_convolve_matrix_finalize;

	d_node_class->get_node_name = lsm_svg_filter_convolve_matrix_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_convolve_matrix_attribute_infos),
					      lsm_svg_filter_convolve_matrix_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_convolve_matrix_apply;
}

G_DEFINE_TYPE (LsmSvgFilterConvolveMatrix, lsm_svg_filter_convolve_matrix, LSM_TYPE_SVG_FILTER_PRIMITIVE)
