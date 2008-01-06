/* gmathmlradicalelement.c
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

#include <gmathmlradicalelement.h>

/* GdomNode implementation */

static const char *
gmathml_radical_get_node_name (GDomNode *node)
{
	GMathmlRadicalElement *radical = GMATHML_RADICAL_ELEMENT (node);

	switch (radical->type) {
		case GMATHML_RADICAL_ELEMENT_TYPE_SQRT:
			return "msqrt";
		case GMATHML_RADICAL_ELEMENT_TYPE_ROOT:
		default:
			return "mroot";
	}
}

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_radical_element_measure (GMathmlElement *element, GMathmlView *view)
{
	return gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE(element)->first_child), view);
}

static void
gmathml_radical_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
	gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view, x, y, bbox);
}

/* GMathmlRadicalElement implementation */

GDomNode *
gmathml_sqrt_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_RADICAL_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_RADICAL_ELEMENT (node)->type = GMATHML_RADICAL_ELEMENT_TYPE_SQRT;

	return node;
}

GDomNode *
gmathml_root_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_RADICAL_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_RADICAL_ELEMENT (node)->type = GMATHML_RADICAL_ELEMENT_TYPE_ROOT;

	return node;
}

static void
gmathml_radical_element_init (GMathmlRadicalElement *self)
{
}

/* GMathmlRadicalElement class */

static void
gmathml_radical_element_class_init (GMathmlRadicalElementClass *radical_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (radical_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (radical_class);

	d_node_class->get_node_name = gmathml_radical_get_node_name;

	m_element_class->measure = gmathml_radical_element_measure;
	m_element_class->layout = gmathml_radical_element_layout;
}

G_DEFINE_ABSTRACT_TYPE (GMathmlRadicalElement, gmathml_radical_element, GMATHML_TYPE_ELEMENT)
