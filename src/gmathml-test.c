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
#include <gmathmldocument.h>
#include <gmathmlmathelement.h>
#include <gmathmlnumberelement.h>
#include <gmathmlrowelement.h>
#include <gmathmlview.h>
#include <glib/gmessages.h>

int main()
{
	GDomNode *document;
	GDomNode *math_element;
	GDomNode *number_element;
	GDomNode *text1;
	GDomNode *text2;
	GMathmlView *view;

	g_type_init ();

	document = gmathml_document_new ();
	g_assert (GDOM_IS_NODE (document));
	math_element = gmathml_math_element_new ();
	g_assert (GDOM_IS_NODE (math_element));
	number_element = gmathml_number_element_new ();
	g_assert (GDOM_IS_NODE (number_element));
	text1 = gdom_text_new ("toto");
	g_assert (GDOM_IS_NODE (text1));
	text2 = gdom_text_new ("laripette");
	g_assert (GDOM_IS_NODE (text2));

	g_assert (gdom_node_append_child (number_element, text1) != NULL);
	g_assert (gdom_node_append_child (number_element, text2) != NULL);
	g_assert (gdom_node_append_child (math_element, number_element) != NULL);
	g_assert (gdom_node_append_child (document, math_element) != NULL);

	gdom_node_dump (document);

	view = gmathml_view_new (GMATHML_DOCUMENT (document));

	g_object_unref (view);

	g_object_unref (document);

	return (0);
}
