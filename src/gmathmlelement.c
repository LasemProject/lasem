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
static const GMathmlBbox null_bbox = {0.0, 0.0, 0.0};

const GMathmlBbox *
gmathml_element_measure (GMathmlElement *element, GMathmlView *view)
{
	GMathmlElementClass *element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_val_if_fail (element_class != NULL, &null_bbox);

	if (element_class->measure) {
		const GMathmlBbox *bbox;

		bbox = element_class->measure (element, view);

		g_message ("BBox (%s) %g, %g, %g",
			   gdom_node_get_node_name (GDOM_NODE (element)),
			   bbox->width, bbox->ascent, bbox->descent);
		return bbox;
	}

	return &null_bbox;
}

void
gmathml_element_layout (GMathmlElement *element, GMathmlView *view)
{
	GMathmlElementClass *element_class = GMATHML_ELEMENT_GET_CLASS (element);

	g_return_if_fail (element_class != NULL);

	if (element_class->layout)
		element_class->layout (element, view);
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
gmathml_element_set_attribute (GDomElement *self, const char* name, const char *value)
{
	g_hash_table_replace (GMATHML_ELEMENT (self)->attributes, g_strdup (name), g_strdup (value));
}

char *
gmathml_element_get_attribute (GDomElement *self, const char *name)
{
	return g_hash_table_lookup (GMATHML_ELEMENT (self)->attributes, name);
}

static void
gmathml_element_init (GMathmlElement *element)
{
	element->attributes = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
}

static void
gmathml_element_finalize (GObject *object)
{
	GMathmlElement *element = GMATHML_ELEMENT (object);

	g_hash_table_unref (element->attributes);

	parent_class->finalize (object);
}

static void
gmathml_element_class_init (GMathmlElementClass *m_element_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (m_element_class);
	GDomElementClass *d_element_class = GDOM_ELEMENT_CLASS (m_element_class);

	parent_class = g_type_class_peek_parent (m_element_class);

	object_class->finalize = gmathml_element_finalize;

	d_element_class->get_attribute = gmathml_element_get_attribute;
	d_element_class->set_attribute = gmathml_element_set_attribute;
	m_element_class->render = gmathml_element_render_default;
}

G_DEFINE_ABSTRACT_TYPE (GMathmlElement, gmathml_element, GDOM_TYPE_ELEMENT)
