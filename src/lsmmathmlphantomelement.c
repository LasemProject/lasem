/* lsmmathmlphantomelement.c
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

#include <lsmmathmlphantomelement.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_mathml_phantom_get_node_name (LsmDomNode *node)
{
	return "mphantom";
}

/* LsmMathmlElement implementation */

static void
lsm_mathml_phantom_element_render (LsmMathmlElement *self, LsmMathmlView *view)
{
}

/* LsmMathmlPhantomElement implementation */

LsmDomNode *
lsm_mathml_phantom_element_new (void)
{
	return g_object_new (LSM_MATHML_TYPE_PHANTOM_ELEMENT, NULL);
}

static void
lsm_mathml_phantom_element_init (LsmMathmlPhantomElement *self)
{
}

/* LsmMathmlPhantomElement class */

static void
lsm_mathml_phantom_element_class_init (LsmMathmlPhantomElementClass *phantom_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (phantom_class);
	LsmMathmlElementClass *m_element_class = LSM_MATHML_ELEMENT_CLASS (phantom_class);

	parent_class = g_type_class_peek_parent (phantom_class);

	d_node_class->get_node_name = lsm_mathml_phantom_get_node_name;

	m_element_class->render = lsm_mathml_phantom_element_render;
}

G_DEFINE_TYPE (LsmMathmlPhantomElement, lsm_mathml_phantom_element, LSM_MATHML_TYPE_ELEMENT)
