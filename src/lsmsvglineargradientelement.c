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

#include <lsmsvglineargradientelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_linear_gradient_element_get_node_name (LsmDomNode *node)
{
	return "linearGradient";
}

/* GLinearGradientElement implementation */

static void
_linear_gradient_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgLinearGradientElement *linear = LSM_SVG_LINEAR_GRADIENT_ELEMENT (self);
	LsmSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&linear->x1, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&linear->y1, &length, 0.0);

	length.value = 1.0;
	length.value_unit = 1.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&linear->x2, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_animated_length_attribute_parse (&linear->y2, &length, 0.0);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

static void
_linear_gradient_element_render_paint (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgLinearGradientElement *linear = LSM_SVG_LINEAR_GRADIENT_ELEMENT (self);
	double x1, x2, y1, y2;

	x1 = linear->x1.length.base.value;
	y1 = linear->y1.length.base.value;
	x2 = linear->x2.length.base.value;
	y2 = linear->y2.length.base.value;

	lsm_debug ("[LsmSvgLinearGradientElement::render] Create linear %g, %g, %g, %g",
		    x1, y1, x2, y2);

	lsm_svg_view_create_linear_gradient (view, x1, y1, x2, y2);

	LSM_SVG_ELEMENT_CLASS (parent_class)->render_paint (self, view);
}

/* LsmSvgLinearGradientElement implementation */

LsmDomNode *
lsm_svg_linear_gradient_element_new (void)
{
	return g_object_new (LSM_SVG_TYPE_LINEAR_GRADIENT_ELEMENT, NULL);
}

static void
lsm_svg_linear_gradient_element_init (LsmSvgLinearGradientElement *self)
{

}

static void
lsm_svg_linear_gradient_element_finalize (GObject *object)
{
}

/* LsmSvgLinearGradientElement class */

static void
lsm_svg_linear_gradient_element_class_init (LsmSvgLinearGradientElementClass *s_svg_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_svg_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_svg_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_svg_class);

	parent_class = g_type_class_peek_parent (s_svg_class);

	object_class->finalize = lsm_svg_linear_gradient_element_finalize;

	d_node_class->get_node_name = lsm_svg_linear_gradient_element_get_node_name;

	s_element_class->update = _linear_gradient_element_update;
	s_element_class->render_paint = _linear_gradient_element_render_paint;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x1",
					  offsetof (LsmSvgLinearGradientElement, x1));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y1",
					  offsetof (LsmSvgLinearGradientElement, y1));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x2",
					  offsetof (LsmSvgLinearGradientElement, x2));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y2",
					  offsetof (LsmSvgLinearGradientElement, y2));
}

G_DEFINE_TYPE (LsmSvgLinearGradientElement, lsm_svg_linear_gradient_element, LSM_SVG_TYPE_GRADIENT_ELEMENT)
