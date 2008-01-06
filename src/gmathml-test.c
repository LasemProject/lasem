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
#include <gmathmloperatordictionary.h>
#include <gmathmldocument.h>
#include <gmathmlmathelement.h>
#include <gmathmlfractionelement.h>
#include <gmathmlrowelement.h>
#include <gmathmlview.h>
#include <gmathmlparser.h>
#include <glib/gmessages.h>
#include <cairo-svg.h>

int main(int argc, char **argv)
{
	GDomNode *document;
	GMathmlView *view;
	cairo_t *cairo;
	cairo_surface_t *surface;

	g_type_init ();

	g_assert (gmathml_operator_get_attributes ("toto", GMATHML_FORM_PREFIX) != NULL);
	g_assert (gmathml_operator_get_attributes ("<>", GMATHML_FORM_INFIX) != NULL);

/*        g_message ("math class=%s", gdom_element_get_attribute (GDOM_ELEMENT (math_element), "class"));*/
/*        gdom_element_set_attribute (GDOM_ELEMENT (math_element), "class", "toto");*/
/*        g_message ("math class=%s", gdom_element_get_attribute (GDOM_ELEMENT (math_element), "class"));*/

	if (argc > 1) {
		document = gmathml_document_from_file (argv[1]);

		gdom_node_dump (document);

		view = gmathml_view_new (GMATHML_DOCUMENT (document));

		surface = cairo_svg_surface_create ("gmathml.svg", 200.0, 50.0);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		cairo_move_to (cairo, 0, 25);
		gmathml_view_render (view, cairo);

		cairo_destroy (cairo);

		g_object_unref (view);

		g_object_unref (document);
	}

	return (0);
}
