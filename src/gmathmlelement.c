/* gmathmlelement.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmlelement.h>

static GObjectClass *parent_class;

static const GMathmlBbox null_bbox = {0.0,0.0,0.0};

/* GDomNode implementation */

static void
gmathml_element_post_new_child (GDomNode *parent, GDomNode *child)
{
	GMathmlStyleAttributes *parent_style;
	GMathmlStyleAttributes *child_style;

	if (!GMATHML_IS_ELEMENT (child))
		return;

	parent_style = &(GMATHML_ELEMENT (parent)->style_attrs);
	child_style = &(GMATHML_ELEMENT (child)->style_attrs);

	if (!child_style->script_level.is_defined)
		child_style->script_level.value = parent_style->script_level.value;
}

static void
gmathml_element_pre_remove_child (GDomNode *parent, GDomNode *child)
{
}

/* GDomElement implementation */

static void
gmathml_element_set_attribute (GDomElement *self, const char* name, const char *value)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS(self);

	gmathml_attributes_set_attribute (m_element_class->attributes, self,
					  name, value);
}

const char *
gmathml_element_get_attribute (GDomElement *self, const char *name)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS(self);

	return gmathml_attributes_get_attribute (m_element_class->attributes, self, name);
}

/* GMathmlElement implementation */

const GMathmlBbox *
gmathml_element_measure (GMathmlElement *element, GMathmlView *view)
{
	GMathmlElementClass *element_class;

	g_return_val_if_fail (GMATHML_IS_ELEMENT (element), &null_bbox);

	element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_val_if_fail (element_class != NULL, &null_bbox);

	if (!element->measure_done) {
		if (element_class->measure) {
			element->bbox = *(element_class->measure (element, view));

			g_message ("BBox (%s) %g, %g, %g",
				   gdom_node_get_node_name (GDOM_NODE (element)),
				   element->bbox.width, element->bbox.height, element->bbox.depth);
		} else {
			element->bbox.width = 0.0;
			element->bbox.height = 0.0;
			element->bbox.depth = 0.0;
		}

		element->measure_done = TRUE;
	}

	return &element->bbox;
}

void
gmathml_element_layout (GMathmlElement *self, GMathmlView *view,
			double x, double y, const GMathmlBbox *bbox)
{
	GMathmlElementClass *element_class;

	g_return_if_fail (GMATHML_IS_ELEMENT (self));

	element_class = GMATHML_ELEMENT_GET_CLASS (self);

	g_return_if_fail (element_class != NULL);

	g_message ("Assigned bbox for %s = %g, %g, %g",
		   gdom_node_get_node_name (GDOM_NODE (self)), bbox->width, bbox->height, bbox->depth);

	self->x = x;
	self->y = y;

	if (element_class->layout)
		element_class->layout (self, view, x, y, bbox);
}

static void
gmathml_element_render_default (GMathmlElement *element, GMathmlView *view)
{
	GDomNode *node;

	for (node = GDOM_NODE (element)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node))
		    gmathml_element_render (GMATHML_ELEMENT (node), view);
}

void
gmathml_element_render (GMathmlElement *element, GMathmlView *view)
{
	GMathmlElementClass *element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_if_fail (element_class != NULL);

	if (element_class->render)
		element_class->render (element, view);
}

static void
gmathml_element_init (GMathmlElement *element)
{
}

static void
gmathml_element_finalize (GObject *object)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_GET_CLASS (object);

	gmathml_attributes_finalize_attributes (m_element_class->attributes, object);

	parent_class->finalize (object);
}

/* GMathmlElement class */

void
gmathml_element_class_add_element_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attributes_add_attribute (m_element_class->attributes, "class", GMATHML_ATTRIBUTE_STRING,
					  offsetof (GMathmlElement, element_attrs.class_name));
	gmathml_attributes_add_attribute (m_element_class->attributes, "id", GMATHML_ATTRIBUTE_STRING,
					  offsetof (GMathmlElement, element_attrs.id));
}

void
gmathml_element_class_add_style_attributes (GMathmlElementClass *m_element_class)
{
	gmathml_attributes_add_attribute (m_element_class->attributes, "scriptlevel",
					  GMATHML_ATTRIBUTE_INT,
					  offsetof (GMathmlElement, style_attrs.script_level));
	gmathml_attributes_add_attribute (m_element_class->attributes, "display",
					  GMATHML_ATTRIBUTE_BOOLEAN,
					  offsetof (GMathmlElement, style_attrs.display_style));
	gmathml_attributes_add_attribute (m_element_class->attributes, "scriptsizemultiplier",
					  GMATHML_ATTRIBUTE_DOUBLE,
					  offsetof (GMathmlElement, style_attrs.script_size_multiplier));
	gmathml_attributes_add_attribute (m_element_class->attributes, "scriptminsize",
					  GMATHML_ATTRIBUTE_VUNIT,
					  offsetof (GMathmlElement, style_attrs.script_min_size));
	gmathml_attributes_add_attribute (m_element_class->attributes, "background",
					  GMATHML_ATTRIBUTE_COLOR,
					  offsetof (GMathmlElement, style_attrs.background));
	gmathml_attributes_add_attribute (m_element_class->attributes, "veryverythinmathspace",
					  GMATHML_ATTRIBUTE_HUNIT,
					  offsetof (GMathmlElement, style_attrs.very_very_thin_math_space));
}

static void
gmathml_element_class_init (GMathmlElementClass *m_element_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (m_element_class);
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_element_class);
	GDomElementClass *d_element_class = GDOM_ELEMENT_CLASS (m_element_class);

	parent_class = g_type_class_peek_parent (m_element_class);

	object_class->finalize = gmathml_element_finalize;

	d_node_class->post_new_child = gmathml_element_post_new_child;
	d_node_class->pre_remove_child = gmathml_element_pre_remove_child;

	d_element_class->get_attribute = gmathml_element_get_attribute;
	d_element_class->set_attribute = gmathml_element_set_attribute;

	m_element_class->render = gmathml_element_render_default;

	m_element_class->attributes = gmathml_attributes_new ();

	gmathml_element_class_add_element_attributes (m_element_class);
	gmathml_element_class_add_style_attributes (m_element_class);
}

G_DEFINE_ABSTRACT_TYPE (GMathmlElement, gmathml_element, GDOM_TYPE_ELEMENT)
