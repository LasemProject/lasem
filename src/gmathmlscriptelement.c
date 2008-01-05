/* gmathmlscriptelement.c
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

#include <gmathmlscriptelement.h>

/* GdomNode implementation */

/* GMathmlElement implementation */

static const GMathmlBbox *
gmathml_script_element_measure (GMathmlElement *element, GMathmlView *view)
{
	return gmathml_element_measure (GMATHML_ELEMENT (GDOM_NODE(element)->first_child), view);
}

static void
gmathml_script_element_layout (GMathmlElement *self, GMathmlView *view,
			     double x, double y, const GMathmlBbox *bbox)
{
	gmathml_element_layout (GMATHML_ELEMENT (GDOM_NODE(self)->first_child), view, x, y, bbox);
}

/* GMathmlScriptElement implementation */

static void
gmathml_script_element_init (GMathmlScriptElement *self)
{
}

/* GMathmlScriptElement class */

static void
gmathml_script_element_class_init (GMathmlScriptElementClass *klass)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (klass);

	m_element_class->measure = gmathml_script_element_measure;
	m_element_class->layout = gmathml_script_element_layout;
}

G_DEFINE_ABSTRACT_TYPE (GMathmlScriptElement, gmathml_script_element, GMATHML_TYPE_ELEMENT)
