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

void
gmathml_element_layout (GMathmlElement *element, GMathmlView *view)
{
}

void
gmathml_element_render (GMathmlElement *element, GMathmlView *view)
{
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
gmathml_element_class_init (GMathmlElementClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GDomElementClass *element_class = GDOM_ELEMENT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = gmathml_element_finalize;

	element_class->get_attribute = gmathml_element_get_attribute;
	element_class->set_attribute = gmathml_element_set_attribute;
}

G_DEFINE_ABSTRACT_TYPE (GMathmlElement, gmathml_element, GDOM_TYPE_ELEMENT)
