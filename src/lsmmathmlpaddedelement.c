/* Lasem
 *
 * Copyright © 2007-2008 Emmanuel Pacaud
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

#include <lsmmathmlpaddedelement.h>

static GObjectClass *parent_class;

/* LsmDomNode implementation */

static const char *
lsm_mathml_padded_element_get_node_name (LsmDomNode *node)
{
	return "mpadded";
}

/* LsmMathmlElement implementation */

static const LsmMathmlLength length_default = {0.0, LSM_MATHML_UNIT_PT};

static void
_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlPaddedElement *padded = LSM_MATHML_PADDED_ELEMENT (self);

	lsm_mathml_length_attribute_normalize (&padded->height, 0.0, &length_default, style);
	lsm_mathml_length_attribute_normalize (&padded->depth, 0.0, &length_default, style);
	lsm_mathml_length_attribute_normalize (&padded->width, 0.0, &length_default, style);
	lsm_mathml_length_attribute_normalize (&padded->left_space, 0.0, &length_default, style);
}

static const LsmMathmlBbox *
_measure (LsmMathmlElement *self, LsmMathmlView *view, const LsmMathmlBbox *bbox)
{
	LsmMathmlPaddedElement *padded = LSM_MATHML_PADDED_ELEMENT (self);

	LSM_MATHML_ELEMENT_CLASS (parent_class)->measure (self, view, bbox);

	self->bbox.height += padded->height.value;
	self->bbox.depth += padded->depth.value;
	self->bbox.width += padded->width.value + padded->left_space.value;

	return &self->bbox;
}

static void
_layout (LsmMathmlElement *self, LsmMathmlView *view,
	 double x, double y, const LsmMathmlBbox *bbox)
{
	LsmMathmlPaddedElement *padded = LSM_MATHML_PADDED_ELEMENT (self);

	LSM_MATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x + padded->left_space.value, y, bbox);
}

/* LsmMathmlPaddedElement implementation */

LsmDomNode *
lsm_mathml_padded_element_new (void)
{
	return g_object_new (LSM_TYPE_MATHML_PADDED_ELEMENT, NULL);
}

static void
lsm_mathml_padded_element_init (LsmMathmlPaddedElement *element)
{
}

/* LsmMathmlPaddedElement class */

static const LsmAttributeInfos _attribute_infos[] = {
	{
		.name = "height",
		.attribute_offset = offsetof (LsmMathmlPaddedElement, height),
		.trait_class = &lsm_mathml_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "depth",
		.attribute_offset = offsetof (LsmMathmlPaddedElement, depth),
		.trait_class = &lsm_mathml_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "width",
		.attribute_offset = offsetof (LsmMathmlPaddedElement, width),
		.trait_class = &lsm_mathml_length_trait_class,
		.trait_default = &length_default
	},
	{
		.name = "lspace",
		.attribute_offset = offsetof (LsmMathmlPaddedElement, left_space),
		.trait_class = &lsm_mathml_length_trait_class,
		.trait_default = &length_default
	}
};

static void
lsm_mathml_padded_element_class_init (LsmMathmlPaddedElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_mathml_padded_element_get_node_name;

	m_element_class->attribute_manager = lsm_attribute_manager_duplicate (m_element_class->attribute_manager);
	m_element_class->update = _update;
	m_element_class->measure = _measure;
	m_element_class->layout = _layout;

	lsm_attribute_manager_add_attributes (m_element_class->attribute_manager,
					      G_N_ELEMENTS (_attribute_infos),
					      _attribute_infos);
}

G_DEFINE_TYPE (LsmMathmlPaddedElement, lsm_mathml_padded_element, LSM_TYPE_MATHML_PRESENTATION_CONTAINER)

