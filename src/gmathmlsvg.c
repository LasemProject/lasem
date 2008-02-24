/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright © 2007-2008 Emmanuel Pacaud <emmanuel@gnome.org>
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
#include <glib/goption.h>
#include <cairo-svg.h>

static gboolean debug = FALSE;

static const GOptionEntry entries[] =
{
  { "debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Debug mode", NULL },
  { NULL }
};

int main(int argc, char **argv)
{
	GDomNode *document;
	GMathmlView *view;
	GError *error = NULL;
	GOptionContext *context;
	cairo_t *cairo;
	cairo_surface_t *surface;
	double height, width;

	g_type_init ();

	context = g_option_context_new (NULL);
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_parse (context, &argc, &argv, &error);

	if (argc > 1) {
		document = gmathml_document_from_file (argv[1]);

		gdom_node_dump (document);

		surface = cairo_svg_surface_create ("gmathml.svg", 100, 100);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		view = gmathml_view_new (GMATHML_DOCUMENT (document), cairo);

		cairo_destroy (cairo);

		gmathml_view_set_debug (view, debug);

		gmathml_view_measure (view, &width, &height);

		surface = cairo_svg_surface_create ("gmathml.svg", width, height);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		gmathml_view_set_cairo (view, cairo);

		cairo_destroy (cairo);

		gmathml_view_render (view);

		g_object_unref (view);

		gdom_node_dump (document);

		g_object_unref (document);
	}

	return (0);
}
