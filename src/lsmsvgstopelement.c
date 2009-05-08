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

#include <lsmsvgstopelement.h>
#include <lsmsvgview.h>
#include <lsmdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_stop_element_get_node_name (LsmDomNode *node)
{
	return "stop";
}


/* LsmSvgElement implementation */

static void
_stop_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgStopElement *stop = LSM_SVG_STOP_ELEMENT (self);
	LsmSvgLength length;

	length.value = 1.0;
	length.type = LSM_SVG_LENGTH_TYPE_NUMBER;
	lsm_svg_length_attribute_parse (&stop->offset, &length, parent_style,
					LSM_SVG_LENGTH_DIRECTION_DIAGONAL);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgStopElement implementation */

double
lsm_svg_stop_element_get_offset (LsmSvgStopElement *self)
{
	g_return_val_if_fail (LSM_IS_SVG_STOP_ELEMENT (self), 0.0);

	return LSM_SVG_STOP_ELEMENT (self)->offset.length.value;
}

const LsmSvgColor *
lsm_svg_stop_element_get_color (LsmSvgStopElement *self)
{
	g_return_val_if_fail (LSM_IS_SVG_STOP_ELEMENT (self), &lsm_svg_color_null);

	if (LSM_SVG_GRAPHIC(self)->stop != NULL)
		return &(LSM_SVG_GRAPHIC (self)->stop->color.value);
	else
		return &lsm_svg_color_null;
}

double
lsm_svg_stop_element_get_opacity (LsmSvgStopElement *self)
{
	g_return_val_if_fail (LSM_IS_SVG_STOP_ELEMENT (self), 1.0);

	if (LSM_SVG_GRAPHIC(self)->stop != NULL)
		return LSM_SVG_GRAPHIC (self)->stop->opacity.value;
	else
		return 1.0;
}

LsmDomNode *
lsm_svg_stop_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_STOP_ELEMENT, NULL);
}

static void
lsm_svg_stop_element_init (LsmSvgStopElement *self)
{
}

/* LsmSvgStopElement class */

static void
lsm_svg_stop_element_class_init (LsmSvgStopElementClass *klass)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (klass);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = lsm_svg_stop_element_get_node_name;

	s_element_class->update = _stop_element_update;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "offset",
					  offsetof (LsmSvgStopElement, offset));
}

G_DEFINE_TYPE (LsmSvgStopElement, lsm_svg_stop_element, LSM_TYPE_SVG_GRAPHIC)
