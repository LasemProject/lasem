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

#if 0
static const GMathmlBbox *
gmathml_radical_element_measure (GMathmlElement *element, GMathmlView *view)
{
}

static void
gmathml_radical_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
}
#endif

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

	d_node_class->get_node_name = gmathml_radical_get_node_name;
}

G_DEFINE_TYPE (GMathmlRadicalElement, gmathml_radical_element, GMATHML_TYPE_ELEMENT)
