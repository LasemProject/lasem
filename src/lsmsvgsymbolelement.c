/* Lasem
 *
 * Copyright © 2009 Emmanuel Pacaud
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

#include <lsmsvgsymbolelement.h>
#include <lsmsvguseelement.h>
#include <lsmsvgview.h>
#include <lsmsvgtraits.h>
#include <lsmdebug.h>
#include <lsmdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_symbol_element_get_node_name (LsmDomNode *node)
{
	return "symbol";
}

/* LsmSvgElement implementation */

/* LsmSvgGraphic implementation */

static void
_symbol_element_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgSymbolElement *symbol = LSM_SVG_SYMBOL_ELEMENT (self);
	LsmSvgUseElement *use_element;
	LsmSvgElement *element;
	LsmSvgStyle *style;
	LsmBox viewport;
	gboolean is_viewbox_defined;

	element = lsm_svg_view_get_referencing_element (view);
	if (!LSM_IS_SVG_USE_ELEMENT (element))
		return;

	use_element = LSM_SVG_USE_ELEMENT (element);

	style = lsm_svg_view_get_current_style (view);

	viewport.x = 0;
	viewport.y = 0;
	viewport.width = lsm_svg_view_normalize_length (view, &use_element->width.length, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	viewport.height = lsm_svg_view_normalize_length (view, &use_element->height.length, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	is_viewbox_defined = lsm_attribute_is_defined ((LsmAttribute *) &symbol->viewbox);

	if (is_viewbox_defined && (symbol->viewbox.value.width == 0.0 ||
				   symbol->viewbox.value.height == 0.0))
		return;

	if (viewport.width <= 0.0 || viewport.height <= 0.0)
		return;

	lsm_debug_render ("[LsmSvgSymbolElement::render] viewport %g, %g, %g, %g",
		   viewport.x, viewport.y, viewport.width, viewport.height);

	lsm_svg_view_push_viewport (view, &viewport, is_viewbox_defined ? &symbol->viewbox.value : NULL,
				    &symbol->preserve_aspect_ratio.value, style->overflow->value);

	LSM_SVG_ELEMENT_CLASS (parent_class)->render (self, view);

	lsm_svg_view_pop_viewport (view);
}

/* LsmSvgSymbolElement implementation */

LsmDomNode *
lsm_svg_symbol_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_SYMBOL_ELEMENT, NULL);
}

static const LsmBox viewbox_default =		 {0.0, 0.0, 0.0, 0.0};
static const LsmSvgPreserveAspectRatio preserve_aspect_ratio_default = {
	.defer = FALSE,
	.align = LSM_SVG_ALIGN_X_MID_Y_MID,
	.meet_or_slice = LSM_SVG_MEET_OR_SLICE_MEET
};

static void
lsm_svg_symbol_element_init (LsmSvgSymbolElement *self)
{
}

/* LsmSvgSymbolElement class */

static const LsmAttributeInfos lsm_svg_symbol_element_attribute_infos[] = {
	{
		.name = "viewBox",
		.attribute_offset = offsetof (LsmSvgSymbolElement, viewbox),
		.trait_class = &lsm_box_trait_class,
		.trait_default = &viewbox_default
	},
	{
		.name = "preserveAspectRatio",
		.attribute_offset = offsetof (LsmSvgSymbolElement, preserve_aspect_ratio),
		.trait_class = &lsm_svg_preserve_aspect_ratio_trait_class,
		.trait_default = &preserve_aspect_ratio_default
	}
};

static void
lsm_svg_symbol_element_class_init (LsmSvgSymbolElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_svg_symbol_element_get_node_name;

	s_element_class->category =
		LSM_SVG_ELEMENT_CATEGORY_CONTAINER |
		LSM_SVG_ELEMENT_CATEGORY_STRUCTURAL;

	s_element_class->render = _symbol_element_render;
	s_element_class->attribute_manager = lsm_attribute_manager_duplicate (s_element_class->attribute_manager);

	lsm_attribute_manager_add_attributes (s_element_class->attribute_manager,
					      G_N_ELEMENTS (lsm_svg_symbol_element_attribute_infos),
					      lsm_svg_symbol_element_attribute_infos);
}

G_DEFINE_TYPE (LsmSvgSymbolElement, lsm_svg_symbol_element, LSM_TYPE_SVG_ELEMENT)
