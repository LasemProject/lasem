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
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmmathmlencloseelement.h>
#include <lsmmathmlview.h>

static GObjectClass *parent_class;

/* LsmDomNode implementation */

static const char *
_get_node_name (LsmDomNode *node)
{
	return "menclose";
}

/* LsmMathmlElement implementation */

static void
_update (LsmMathmlElement *self, LsmMathmlStyle *style)
{
	LsmMathmlEncloseElement *enclose = LSM_MATHML_ENCLOSE_ELEMENT (self);

	style->math_color = lsm_mathml_color_attribute_inherit (&enclose->math_color, style->math_color);
	style->math_background = lsm_mathml_color_attribute_inherit (&enclose->math_background, style->math_background);
}

static const LsmMathmlBbox *
_measure (LsmMathmlElement *self, LsmMathmlView *view, const LsmMathmlBbox *bbox)
{
	LsmMathmlEncloseElement *enclose = LSM_MATHML_ENCLOSE_ELEMENT (self);
	LsmMathmlBbox stretch_bbox;

	LSM_MATHML_ELEMENT_CLASS (parent_class)->measure (self, view, bbox);

	stretch_bbox = self->bbox;

	lsm_mathml_view_measure_notation (view, &self->style, enclose->notation.value, &stretch_bbox,
					  &self->bbox, &enclose->x_child_offset);

	return &self->bbox;
}

static void
_layout (LsmMathmlElement *self, LsmMathmlView *view,
	 double x, double y, const LsmMathmlBbox *bbox)
{
	LsmMathmlEncloseElement *enclose = LSM_MATHML_ENCLOSE_ELEMENT (self);

	LSM_MATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x + enclose->x_child_offset, y, bbox);
}

static void
_render (LsmMathmlElement *self, LsmMathmlView *view)
{
	LsmMathmlEncloseElement *enclose = LSM_MATHML_ENCLOSE_ELEMENT (self);

	LSM_MATHML_ELEMENT_CLASS (parent_class)->render (self, view);

	lsm_mathml_view_show_notation (view, &self->style, enclose->notation.value, self->x, self->y, &self->bbox,
				       enclose->x_child_offset);
}

/* LsmMathmlEncloseElement implementation */

LsmDomNode *
lsm_mathml_enclose_element_new (void)
{
	return g_object_new (LSM_TYPE_MATHML_ENCLOSE_ELEMENT, NULL);
}

static const LsmMathmlNotation notation_default = LSM_MATHML_NOTATION_LONGDIV;

static void
lsm_mathml_enclose_element_init (LsmMathmlEncloseElement *element)
{
}

/* LsmMathmlEncloseElement class */

static const LsmAttributeInfos _attribute_infos[] = {
	{
		.name = "notation",
		.attribute_offset = offsetof (LsmMathmlEncloseElement, notation),
		.trait_class = &lsm_mathml_notation_trait_class,
		.trait_default = &notation_default
	},
	{
		.name = "mathcolor",
		.attribute_offset = offsetof (LsmMathmlEncloseElement, math_color),
		.trait_class = &lsm_mathml_color_trait_class,
	},
	{
		.name = "mathbackground",
		.attribute_offset = offsetof (LsmMathmlEncloseElement, math_background),
		.trait_class = &lsm_mathml_color_trait_class,
	}
};

static void
lsm_mathml_enclose_element_class_init (LsmMathmlEncloseElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = _get_node_name;

	m_element_class->update = _update;
	m_element_class->measure = _measure;
	m_element_class->layout = _layout;
	m_element_class->render = _render;
	m_element_class->attribute_manager = lsm_attribute_manager_duplicate (m_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (m_element_class->attribute_manager,
					      G_N_ELEMENTS (_attribute_infos),
					      _attribute_infos);
}

G_DEFINE_TYPE (LsmMathmlEncloseElement, lsm_mathml_enclose_element, LSM_TYPE_MATHML_PRESENTATION_CONTAINER)

