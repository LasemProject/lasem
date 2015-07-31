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

#include <lsmsvgfiltercolormatrix.h>
#include <lsmsvgview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_filter_color_matrix_get_node_name (LsmDomNode *node)
{
	return "feColorMatrix";
}

/* LsmSvgElement implementation */

static void
lsm_svg_filter_color_matrix_apply  (LsmSvgFilterPrimitive *self, LsmSvgView *view,
				    const char *input, const char *output, const LsmBox *subregion)
{
	LsmSvgFilterColorMatrix *color_matrix = LSM_SVG_FILTER_COLOR_MATRIX (self);

	lsm_svg_view_apply_color_matrix (view, input, output, subregion,
					 color_matrix->type.value,
					 color_matrix->values.value.n_values,
					 color_matrix->values.value.values);
}

/* LsmSvgFilterColorMatrix implementation */

static const LsmSvgColorFilterType color_filter_type_default = LSM_SVG_COLOR_FILTER_TYPE_MATRIX;
static const LsmSvgVector values_default = { .n_values = 0, .values = NULL};

LsmDomNode *
lsm_svg_filter_color_matrix_new (void)
{
	return g_object_new (LSM_TYPE_SVG_FILTER_COLOR_MATRIX, NULL);
}

static void
lsm_svg_filter_color_matrix_init (LsmSvgFilterColorMatrix *self)
{
	self->type.value = color_filter_type_default;
	self->values.value = values_default;
}

static void
lsm_svg_filter_color_matrix_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmSvgFilterColorMatrix class */

static const LsmAttributeInfos lsm_svg_filter_color_matrix_attribute_infos[] = {
	{
		.name = "type",
		.attribute_offset = offsetof (LsmSvgFilterColorMatrix, type),
		.trait_class = &lsm_svg_color_filter_type_trait_class,
		.trait_default = &color_filter_type_default
	},
	{
		.name = "values",
		.attribute_offset = offsetof (LsmSvgFilterColorMatrix, values),
		.trait_class = &lsm_svg_vector_trait_class,
		.trait_default = &values_default
	}
};

static void
lsm_svg_filter_color_matrix_class_init (LsmSvgFilterColorMatrixClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgFilterPrimitiveClass *f_primitive_class = LSM_SVG_FILTER_PRIMITIVE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_svg_filter_color_matrix_finalize;

	d_node_class->get_node_name = lsm_svg_filter_color_matrix_get_node_name;

	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_filter_color_matrix_attribute_infos),
					      lsm_svg_filter_color_matrix_attribute_infos);

	f_primitive_class->apply = lsm_svg_filter_color_matrix_apply;
}

G_DEFINE_TYPE (LsmSvgFilterColorMatrix, lsm_svg_filter_color_matrix, LSM_TYPE_SVG_FILTER_PRIMITIVE)
