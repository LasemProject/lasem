/*
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <glib/gregex.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gmathmlview.h>
#include <gmathmldocument.h>
#include <gmathmlparser.h>

#include <libxml/parser.h>

#include <../itex2mml/itex2MML.h>

#define TEST_WIDTH 	480
#define XML_FILENAME	"gmathmltest.xml"

static const char *fail_face = "", *normal_face = "";
FILE *gmathml_test_html_file = NULL;

static void
gmathml_test_html (const char *fmt, ...)
{
	va_list va;
	FILE *file = gmathml_test_html_file ? gmathml_test_html_file : stdout;

	va_start (va, fmt);
	vfprintf (file, fmt, va);
	va_end (va);
}

static GRegex *regex_mml = NULL;

void
gmathml_test_render (char const *filename)
{
	GDomNode *document;
	GMathmlView *view;
	cairo_t *cairo;
	cairo_surface_t *surface;
	char *buffer = NULL;
	size_t size;
	char *png_filename;
	char *reference_png_filename;
	char *test_name;
	char *mime;
	double width, height;
	gboolean is_mml, success;
	GRegex *regex;
	GError *error = NULL;
	char *filtered_buffer;

	test_name = g_regex_replace (regex_mml, filename, -1, 0, "", 0, NULL);

	png_filename = g_strdup_printf ("%s-out.png", test_name);
	reference_png_filename = g_strdup_printf ("%s.png", test_name);

	mime = g_content_type_guess (filename, NULL, 0, NULL);
	is_mml = strcmp (mime, "text/mathml") == 0;
	g_free (mime);

	g_printf ("\trender %s (%s)\n", filename, is_mml ? "MathML" : "Tex");

	success = g_file_get_contents (filename, &buffer, &size, NULL);
	if (success) {
		char *mathml = NULL;

		if (is_mml)
			mathml = buffer;
		else
			mathml = itex2MML_parse (buffer, size);

		document = gmathml_document_from_memory (mathml);

		view = gmathml_view_new (GMATHML_DOCUMENT (document), NULL);

		gmathml_view_measure (view, &width, &height);

		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width + .5, height + .5);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		gmathml_view_set_cairo (view, cairo);

		cairo_destroy (cairo);

		gmathml_view_render (view);

		cairo_surface_write_to_png (surface, png_filename);

		g_object_unref (view);
		g_object_unref (document);

		gmathml_test_html ("<table border=\"1\" cellpadding=\"8\">\n");
		gmathml_test_html ("<tr>");
		gmathml_test_html ("<td>");

		regex = g_regex_new ("<math>", 0, 0, &error);
		assert (error == NULL);

		filtered_buffer = g_regex_replace (regex, mathml,
						   -1, 0,
						   "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">",
						   0, NULL);
		g_regex_unref (regex);

		gmathml_test_html (filtered_buffer);

		g_free (filtered_buffer);

		gmathml_test_html ("</td>");
		gmathml_test_html ("<td><a href=\"%s\"><img border=\"0\" src=\"%s\"/></a></td>",
				   filename, png_filename);
		gmathml_test_html ("<td><img src=\"%s\"/></td>", reference_png_filename);
		gmathml_test_html ("</tr>\n");
		gmathml_test_html ("</table>\n");

		if (!is_mml && !g_file_test (reference_png_filename, G_FILE_TEST_IS_REGULAR)) {
			FILE *file;
			int result;
			char *cmd;

			file = fopen ("gmathmltest.tmp", "w");
			fprintf (file, "\\documentclass[10pt]{article}\n");
			fprintf (file, "\\usepackage{amsmath}\n");
			fprintf (file, "\\usepackage{amsfonts}\n");
			fprintf (file, "\\usepackage{amssymb}\n");
			fprintf (file, "\\usepackage{pst-plot}\n");
			fprintf (file, "\\usepackage{color}\n");
			fprintf (file, "\\pagestyle{empty}\n");
			fprintf (file, "\\begin{document}\n");
			fprintf (file, "%s\n", buffer);
			fprintf (file, "\\end{document}\n");
			fclose (file);

			result = system ("latex --interaction=nonstopmode gmathmltest.tmp");
			result = system ("dvips -E gmathmltest.dvi -o gmathmltest.ps");

			cmd = g_strdup_printf ("convert -density 120 gmathmltest.ps %s", reference_png_filename);
			result = system (cmd);
			g_free (cmd);

			result = system ("rm gmathmltest.tmp");
			result = system ("rm gmathmltest.dvi");
			result = system ("rm gmathmltest.log");
			result = system ("rm gmathmltest.aux");
			result = system ("rm gmathmltest.ps");
		}

		if (mathml != buffer) {
			char *xml_filename;

			xml_filename = g_strdup_printf ("%s.xml", test_name);

			g_file_set_contents (xml_filename, mathml, -1, NULL);

			g_free (xml_filename);
			g_free (buffer);
		}

		g_free (mathml);
	}

	g_free (png_filename);
	g_free (reference_png_filename);

	g_free (test_name);
}

unsigned int
gmathml_test_process_dir (const char *name)
{
	GDir *directory;
	GError *error = NULL;
	const char *entry;
	char *filename;
	unsigned int n_files = 0;

	directory = g_dir_open (name, 0, &error);
	assert (error == NULL);

	g_printf ("In directory %s\n", name);

	gmathml_test_html ("<h1>%s</h1>", name);

	do {
		entry = g_dir_read_name (directory);
		if (entry != NULL &&
		    strstr (entry, "ignore-") != entry)
		{
			filename = g_build_filename (name, entry, NULL);

			if (g_file_test (filename, G_FILE_TEST_IS_DIR))
				n_files += gmathml_test_process_dir (filename);
			else if (g_file_test (filename, G_FILE_TEST_IS_REGULAR) &&
				 g_regex_match (regex_mml, filename, 0, NULL)) {
				gmathml_test_render (filename);
				n_files++;
			}

			g_free (filename);
		}
	} while (entry != NULL);

	g_dir_close (directory);

	return n_files;
}

int
main (int argc, char **argv)
{
	GTimer *timer;
	GError *error = NULL;
	unsigned int i;
	unsigned int n_files = 0;

#ifdef HAVE_UNISTD_H
	if (isatty (2)) {
		fail_face = "\033[41m\033[37m\033[1m";
		normal_face = "\033[m";
	}
#endif

	gmathml_test_html_file = fopen (XML_FILENAME, "w");

	gmathml_test_html ("<?xml version=\"1.0\"?>");
	gmathml_test_html ("<!DOCTYPE html PUBLIC "
			   "\"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN\" "
			   "\"http://www.w3.org/Math/DTD/mathml2/xhtml-math11-f.dtd\">");
	gmathml_test_html ("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	gmathml_test_html ("<body>\n");

	g_type_init ();

	timer = g_timer_new ();

	regex_mml = g_regex_new ("\\.(mml|tex)$", 0, 0, &error);
	assert (error == NULL);

	if (argc >= 2)
		for (i = 0; i < argc - 1; i++)
			gmathml_test_render (argv[i + 1]);
	else
		n_files = gmathml_test_process_dir (".");

	gmathml_test_html ("</body>\n");
	gmathml_test_html ("</html>\n");

	if (gmathml_test_html_file != NULL)
		fclose (gmathml_test_html_file);

	g_regex_unref (regex_mml);

	g_printf ("%d files processed in %g seconds.\n", n_files, g_timer_elapsed (timer, NULL));

	g_timer_destroy (timer);

	return 0;
}

/* vim: set sw=8 sts=8: -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 8 -*- */
