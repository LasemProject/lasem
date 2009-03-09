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

#include <gsvgstopelement.h>
#include <gsvgview.h>
#include <gdomdebug.h>
#include <stdio.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gsvg_stop_element_get_node_name (GDomNode *node)
{
	return "stop";
}


/* GSvgElement implementation */

static void
_stop_element_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgStopElement *stop = GSVG_STOP_ELEMENT (self);
	GSvgLength length;

	length.value = 1.0;
	length.type = GSVG_LENGTH_TYPE_NUMBER;
	gsvg_length_attribute_parse (&stop->offset, &length, 0.0);

	GSVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* GSvgStopElement implementation */

double
gsvg_stop_element_get_offset (GSvgStopElement *self)
{
	g_return_val_if_fail (GSVG_IS_STOP_ELEMENT (self), 0.0);

	return GSVG_STOP_ELEMENT (self)->offset.length.value;
}

const GSvgColor *
gsvg_stop_element_get_color (GSvgStopElement *self)
{
	g_return_val_if_fail (GSVG_IS_STOP_ELEMENT (self), &gsvg_color_null);

	if (GSVG_GRAPHIC(self)->stop != NULL)
		return &(GSVG_GRAPHIC (self)->stop->color.value);
	else
		return &gsvg_color_null;
}

double
gsvg_stop_element_get_opacity (GSvgStopElement *self)
{
	g_return_val_if_fail (GSVG_IS_STOP_ELEMENT (self), 1.0);

	if (GSVG_GRAPHIC(self)->stop != NULL)
		return GSVG_GRAPHIC (self)->stop->opacity.value;
	else
		return 1.0;
}

GDomNode *
gsvg_stop_element_new (void)
{
	return g_object_new (GSVG_TYPE_STOP_ELEMENT, NULL);
}

static void
gsvg_stop_element_init (GSvgStopElement *self)
{
}

/* GSvgStopElement class */

static void
gsvg_stop_element_class_init (GSvgStopElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	d_node_class->get_node_name = gsvg_stop_element_get_node_name;

	s_element_class->update = _stop_element_update;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "offset",
					  offsetof (GSvgStopElement, offset));
}

G_DEFINE_TYPE (GSvgStopElement, gsvg_stop_element, GSVG_TYPE_GRAPHIC)
