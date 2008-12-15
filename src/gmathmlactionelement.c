/*
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#include <gmathmlactionelement.h>

/* GDomNode implementation */

static const char *
gmathml_action_element_get_node_name (GDomNode *node)
{
	return "maction";
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_action_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	if (GDOM_NODE (self)->first_child != NULL)
		self->bbox = *gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE (self)->first_child),
						       view, bbox);
	else
		self->bbox = gmathml_bbox_null;

	return &self->bbox;
}

static void
gmathml_action_element_layout (GMathmlElement *self, GMathmlView *view,
			       double x, double y, const GMathmlBbox *bbox)
{
	if (GDOM_NODE (self)->first_child != NULL)
		gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE (self)->first_child),
					view, x, y, bbox);
}

static void
gmathml_action_element_render (GMathmlElement *self, GMathmlView *view)
{
	if (GDOM_NODE (self)->first_child != NULL)
		gmathml_element_render (GMATHML_ELEMENT (GDOM_NODE (self)->first_child), view);
}

/* GMathmlActionElement implementation */

GDomNode *
gmathml_action_element_new (void)
{
	return g_object_new (GMATHML_TYPE_ACTION_ELEMENT, NULL);
}

static void
gmathml_action_element_init (GMathmlActionElement *element)
{
}

/* GMathmlActionElement class */

static void
gmathml_action_element_class_init (GMathmlActionElementClass *m_action_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_action_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (m_action_class);

	d_node_class->get_node_name = gmathml_action_element_get_node_name;

	m_element_class->measure = gmathml_action_element_measure;
	m_element_class->layout = gmathml_action_element_layout;
	m_element_class->render = gmathml_action_element_render;
}

G_DEFINE_TYPE (GMathmlActionElement, gmathml_action_element, GMATHML_TYPE_PRESENTATION_CONTAINER)

