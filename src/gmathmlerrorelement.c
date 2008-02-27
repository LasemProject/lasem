/* gmathmlerrorelement.c
 *
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

#include <gmathmlerrorelement.h>

/* GDomNode implementation */

static const char *
gmathml_error_element_get_node_name (GDomNode *node)
{
	return "merror";
}

/* GMathmlErrorElement implementation */

GDomNode *
gmathml_error_element_new (void)
{
	return g_object_new (GMATHML_TYPE_ERROR_ELEMENT, NULL);
}

static void
gmathml_error_element_init (GMathmlErrorElement *element)
{
}

/* GMathmlErrorElement class */

static void
gmathml_error_element_class_init (GMathmlErrorElementClass *klass)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (klass);

	d_node_class->get_node_name = gmathml_error_element_get_node_name;
}

G_DEFINE_TYPE (GMathmlErrorElement, gmathml_error_element, GMATHML_TYPE_PRESENTATION_CONTAINER)

