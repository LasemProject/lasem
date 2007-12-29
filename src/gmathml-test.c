/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Emmanuel Pacaud 2007 <emmanuel@gnome.org>
 * 
 * main.c is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with main.c.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <gdomtext.h>
#include <gmathmlnumberelement.h>
#include <gmathmlrowelement.h>
#include <glib/gmessages.h>

int main()
{
	GDomNode *node1;
	GDomNode *node2;
	GDomNode *node3;

	g_type_init ();

	node1 = gmathml_number_element_new ();
	g_assert (GDOM_IS_NODE (node1));
	node2 = gdom_text_new ("toto");
	g_assert (GDOM_IS_TEXT (node2));
	node3 = gdom_text_new ("laripette");
	g_assert (GDOM_IS_TEXT (node3));

	gdom_node_append_child (node1, node2);
	gdom_node_append_child (node1, node3);

	g_message ("node1 name  = %s", gdom_node_get_node_name (node1));
	g_message ("node2 data  = %s", gdom_character_data_get_data (GDOM_CHARACTER_DATA (node2)));
	g_message ("node2 value = %s", gdom_node_get_node_value (node2));
	g_message ("node3 data  = %s", gdom_character_data_get_data (GDOM_CHARACTER_DATA (node3)));
	g_message ("node3 value = %s", gdom_node_get_node_value (node3));

	gdom_node_dump (node1);
	g_print ("\n");

	g_object_unref (node1);

	return (0);
}
