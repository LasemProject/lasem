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

#include <lsmsvguseelement.h>
#include <lsmsvgview.h>
#include <lsmdomdebug.h>
#include <lsmdomdocument.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_use_element_get_node_name (LsmDomNode *node)
{
	return "use";
}

static gboolean
lsm_svg_use_can_append_child (LsmDomNode *node, LsmDomNode *child)
{
	return FALSE;
}

/* LsmSvgElement implementation */

static void
lsm_svg_use_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgUseElement *use_element = LSM_SVG_USE_ELEMENT (self);
	LsmSvgLength length;

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&use_element->x, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&use_element->y, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&use_element->width, &length, 0.0);

	length.value = 0.0;
	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&use_element->height, &length, 0.0);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_use_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgUseElement *use_element;
	LsmDomDocument *document;
	LsmDomElement *element;
	LsmSvgMatrix matrix;
	const char *id;

	document = lsm_dom_node_get_owner_document (LSM_DOM_NODE (self));
	if (document == NULL) {
		lsm_dom_debug ("[LsmSvgUseElement::graphic_render] Owner document not found");
		return;
	}

	use_element = LSM_SVG_USE_ELEMENT (self);

	id = use_element->href.value;
	if (id == NULL)
		return;

	/* TODO Ugly hack */
	if (*id == '#')
		id++;

	element = lsm_dom_document_get_element_by_id (document, id);
	if (!LSM_SVG_IS_ELEMENT (element)) {
		lsm_dom_debug ("[LsmSvgUseElement::graphic_render] Target '%s' not found", id);
		return;
	}

/*        if (use_element->width.length.base.value <= 0.0 ||*/
/*            use_element->height.length.base.value <= 0.0)*/
/*                return;*/

	lsm_dom_debug ("[LsmSvgUseElement::graphic_render] Use '%s'", id);

	lsm_svg_matrix_init_translate (&matrix,
				    use_element->x.length.base.value,
				    use_element->y.length.base.value);

	lsm_svg_view_push_transform (view, &matrix);
	lsm_svg_element_render (LSM_SVG_ELEMENT (element), view);
	lsm_svg_view_pop_transform (view);
}

/* LsmSvgUseElement implementation */

LsmDomNode *
lsm_svg_use_element_new (void)
{
	return g_object_new (LSM_SVG_TYPE_USE_ELEMENT, NULL);
}

static void
lsm_svg_use_element_init (LsmSvgUseElement *self)
{
}

/* LsmSvgUseElement class */

static void
lsm_svg_use_element_class_init (LsmSvgUseElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_svg_use_element_get_node_name;
	d_node_class->can_append_child = lsm_svg_use_can_append_child;

	s_element_class->update = lsm_svg_use_element_update;

	s_graphic_class->graphic_render = lsm_svg_use_element_graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgUseElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgUseElement, y));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "width",
					  offsetof (LsmSvgUseElement, width));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "height",
					  offsetof (LsmSvgUseElement, height));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "xlink:href",
					  offsetof (LsmSvgUseElement, href));
}

G_DEFINE_TYPE (LsmSvgUseElement, lsm_svg_use_element, LSM_SVG_TYPE_GRAPHIC)
