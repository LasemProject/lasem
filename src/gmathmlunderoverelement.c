/* gmathmlunderoverelement.c
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

#include <gmathmlunderoverelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_under_over_get_node_name (GDomNode *node)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (node);

	switch (under_over->type) {
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
			return "munder";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
			return "mover";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER:
		default:
			return "munderover";
	}
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_under_over_element_measure (GMathmlElement *self, GMathmlView *view)
{
	GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view);

	return &self->bbox;
}

static void
gmathml_under_over_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view, x, y, bbox);
}

/* GMathmlUnderOverElement implementation */

GDomNode *
gmathml_under_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER;

	return node;
}

GDomNode *
gmathml_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER;

	return node;
}

GDomNode *
gmathml_under_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER;

	return node;
}

static void
gmathml_under_over_element_init (GMathmlUnderOverElement *self)
{
}

/* GMathmlUnderOverElement class */

static void
gmathml_under_over_element_class_init (GMathmlUnderOverElementClass *under_over_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (under_over_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (under_over_class);

	parent_class = g_type_class_peek_parent (under_over_class);

	d_node_class->get_node_name = gmathml_under_over_get_node_name;

	m_element_class->measure = gmathml_under_over_element_measure;
	m_element_class->layout = gmathml_under_over_element_layout;
}

G_DEFINE_TYPE (GMathmlUnderOverElement, gmathml_under_over_element, GMATHML_TYPE_ELEMENT)
