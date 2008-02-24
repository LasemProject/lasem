/* gdomtext.c
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

#include <gdomtext.h>

/* GDomNode implementation */

static const char *
gdom_text_get_node_name (GDomNode *node)
{
	return "#text";
}

static const char *
gdom_text_get_node_value (GDomNode *node)
{
	return GDOM_CHARACTER_DATA (node)->data;
}

static GDomNodeType
gdom_text_get_node_type (GDomNode *node)
{
	return GDOM_NODE_TYPE_TEXT_NODE;
}

/* GDomText implementation */

GDomNode *
gdom_text_new (const char *data)
{
	GDomNode *node;

	node = g_object_new (GDOM_TYPE_TEXT, NULL);

	gdom_character_data_set_data (GDOM_CHARACTER_DATA (node), data);

	return node;
}

static void
gdom_text_init (GDomText *text_node)
{
}

/* GDomText class */

static void
gdom_text_class_init (GDomTextClass *klass)
{
	GDomNodeClass *node_class = GDOM_NODE_CLASS (klass);

	node_class->get_node_name = gdom_text_get_node_name;
	node_class->get_node_value = gdom_text_get_node_value;
	node_class->get_node_type = gdom_text_get_node_type;
}

G_DEFINE_TYPE (GDomText, gdom_text, GDOM_TYPE_CHARACTER_DATA)
