/* Lasem - SVG and Mathml library
 *
 * Copyright © 2007-2010  Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
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

#include <lsmmathml.h>
#include <glib.h>
#include <gio/gio.h>

#include <libxml/parser.h>

#include <../itex2mml/itex2MML.h>

#define XML_FILENAME	"lasemtest.xml"

static char *option_debug_domains = NULL;
static char **option_input_filenames = NULL;
double option_ppi = 72.0;
static gboolean fatal_warning = FALSE;

static const GOptionEntry entries[] =
{
	{ G_OPTION_REMAINING,	' ', 0,	G_OPTION_ARG_FILENAME_ARRAY,
		&option_input_filenames, 	NULL, NULL},
	{ "ppi", 		'p', 0, G_OPTION_ARG_DOUBLE,
		&option_ppi, 			"Pixel per inch", NULL },
	{ "debug", 		'd', 0, G_OPTION_ARG_STRING,
		&option_debug_domains,		"Debug domains", NULL },
	{ "fatal-warning", 	'f', 0, G_OPTION_ARG_NONE,
		&fatal_warning,			"Make warning fatal", NULL },
	{ NULL }
};

static const char *fail_face = "", *normal_face = "";
FILE *lasem_test_html_file = NULL;

static void
lasem_test_html (const char *fmt, ...)
{
	va_list va;
	FILE *file = lasem_test_html_file ? lasem_test_html_file : stdout;

	va_start (va, fmt);
	vfprintf (file, fmt, va);
	va_end (va);
}

static GRegex *regex_mml = NULL;

void
lasem_test_render (char const *filename)
{
	LsmDomDocument *document;
	LsmDomView *view;
	cairo_t *cairo;
	cairo_surface_t *surface;
	char *buffer = NULL;
	size_t size;
	char *png_filename;
	char *reference_png_filename;
	char *test_name;
	char *mime;
	unsigned int width, height;
	gboolean is_xml, success;
	gboolean is_svg;
	gboolean is_mathml;
	GRegex *regex;
	GError *error = NULL;
	char *filtered_buffer;

	test_name = g_regex_replace (regex_mml, filename, -1, 0, "", 0, NULL);

	png_filename = g_strdup_printf ("%s-out.png", test_name);
	reference_png_filename = g_strdup_printf ("%s.png", test_name);

	mime = g_content_type_guess (filename, NULL, 0, NULL);

	is_svg = strcmp (mime, "image/svg+xml") == 0;
	is_mathml = (strcmp (mime, "text/mathml") == 0) || (strcmp (mime, "application/mathml+xml") == 0);
	is_xml = is_svg || is_mathml;

	g_printf ("\trender %s (%s)\n", filename, mime);
	g_free (mime);

	success = g_file_get_contents (filename, &buffer, &size, NULL);
	if (success) {
		LsmBox viewport;
		char *xml;

		if (is_xml)
			xml = buffer;
		else {
			xml = itex2MML_parse (buffer, size);
			size = -1;
		}

		document = lsm_dom_document_new_from_memory (xml, size, NULL);

		lsm_dom_document_set_path (document, filename);

		view = lsm_dom_document_create_view (document);

		viewport.x = 0.0;
		viewport.y = 0.0;
		viewport.width = 480.0;
		viewport.height = 360.0;

		lsm_dom_document_set_resolution (document, option_ppi);
		lsm_dom_document_set_viewport_px (document, &viewport);
		lsm_dom_view_get_size_pixels (LSM_DOM_VIEW (view), &width, &height);

		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width + 2, height + 2);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		lsm_dom_view_render (LSM_DOM_VIEW (view), cairo, 1, 1);

		cairo_surface_write_to_png (surface, png_filename);

		cairo_destroy (cairo);

		g_object_unref (view);
		g_object_unref (document);

		lasem_test_html ("<table border=\"1\" cellpadding=\"8\">\n");
		lasem_test_html ("<tr>");

		lasem_test_html ("<td><a href=\"%s\"><img border=\"0\" src=\"%s\"/></a></td>",
				   filename, png_filename);
		lasem_test_html ("<td><img src=\"%s\"/></td>", reference_png_filename);

		lasem_test_html ("<td>");

		if (is_mathml) {
			regex = g_regex_new ("<math>", 0, 0, &error);
			assert (error == NULL);

			filtered_buffer = g_regex_replace (regex, xml,
							   -1, 0,
							   "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">",
							   0, NULL);
			g_regex_unref (regex);

			lasem_test_html (filtered_buffer);

			g_free (filtered_buffer);
		}

		if (is_svg) {
			lasem_test_html ("<object type=\"image/svg+xml\" data=\"");
			lasem_test_html (filename);
			lasem_test_html ("\" width=\"480\"/>");
		}

		lasem_test_html ("</td>");
		lasem_test_html ("</tr>\n");
		lasem_test_html ("</table>\n");

		if (!is_xml && !g_file_test (reference_png_filename, G_FILE_TEST_IS_REGULAR)) {
			FILE *file;
			int result;
			char *cmd;

			file = fopen ("lsmmathmltest.tmp", "w");
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

			result = system ("latex --interaction=nonstopmode lsmmathmltest.tmp");
			result = system ("dvips -E lsmmathmltest.dvi -o lsmmathmltest.ps");

			cmd = g_strdup_printf ("convert -density 120 lsmmathmltest.ps %s", reference_png_filename);
			result = system (cmd);
			g_free (cmd);

			result = system ("rm lsmmathmltest.tmp");
			result = system ("rm lsmmathmltest.dvi");
			result = system ("rm lsmmathmltest.log");
			result = system ("rm lsmmathmltest.aux");
			result = system ("rm lsmmathmltest.ps");
		}

		if (xml != buffer) {
			char *xml_filename;

			xml_filename = g_strdup_printf ("%s.xml", test_name);

			g_file_set_contents (xml_filename, xml, -1, NULL);

			g_free (xml_filename);
			g_free (buffer);
			itex2MML_free_string (xml);
		} else
			g_free (xml);
	}

	g_free (png_filename);
	g_free (reference_png_filename);

	g_free (test_name);
}

unsigned int
lasem_test_process_dir (const char *name)
{
	GDir *directory;
	GError *error = NULL;
	const char *entry;
	char *filename;
	unsigned int n_files = 0;

	directory = g_dir_open (name, 0, &error);
	assert (error == NULL);

	g_printf ("In directory %s\n", name);

	lasem_test_html ("<h1>%s</h1>", name);

	do {
		entry = g_dir_read_name (directory);
		if (entry != NULL &&
		    strstr (entry, "ignore-") != entry &&
		    strcmp (entry, "images") != 0)
		{
			filename = g_build_filename (name, entry, NULL);

			if (g_file_test (filename, G_FILE_TEST_IS_DIR))
				n_files += lasem_test_process_dir (filename);
			else if (g_file_test (filename, G_FILE_TEST_IS_REGULAR) &&
				 g_regex_match (regex_mml, filename, 0, NULL)) {
				lasem_test_render (filename);
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
	GOptionContext *context;
	GTimer *timer;
	GError *error = NULL;
	unsigned int i;
	unsigned int n_input_files = 0;

#ifdef HAVE_UNISTD_H
	if (isatty (2)) {
		fail_face = "\033[41m\033[37m\033[1m";
		normal_face = "\033[m";
	}
#endif

	lasem_test_html_file = fopen (XML_FILENAME, "w");

	lasem_test_html ("<?xml version=\"1.0\"?>");
	lasem_test_html ("<!DOCTYPE html PUBLIC "
			   "\"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN\" "
			   "\"http://www.w3.org/Math/DTD/mathml2/xhtml-math11-f.dtd\">");
	lasem_test_html ("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	lasem_test_html ("<body>\n");

	g_type_init ();
	context = g_option_context_new (NULL);
	g_option_context_add_main_entries (context, entries, NULL);

	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_option_context_free (context);
		g_print ("Option parsing failed: %s\n", error->message);
		return 1;
	}

	g_option_context_free (context);

	lsm_debug_enable (option_debug_domains);

	if (fatal_warning)
		g_log_set_fatal_mask ("Lasem", G_LOG_FATAL_MASK | G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_WARNING);

	timer = g_timer_new ();

	regex_mml = g_regex_new ("\\.(mml|tex|svg)$", 0, 0, &error);
	assert (error == NULL);

	n_input_files = option_input_filenames != NULL ? g_strv_length (option_input_filenames) : 0;
	if (n_input_files == 1 && g_file_test (option_input_filenames[0], G_FILE_TEST_IS_DIR))
		n_input_files = lasem_test_process_dir (option_input_filenames[0]);
	else {
		if (n_input_files > 0)
			for (i = 0; i < n_input_files; i++)
				lasem_test_render (option_input_filenames[i]);
		else
			n_input_files = lasem_test_process_dir (".");
	}

	lasem_test_html ("</body>\n");
	lasem_test_html ("</html>\n");

	if (lasem_test_html_file != NULL)
		fclose (lasem_test_html_file);

	g_regex_unref (regex_mml);

	g_printf ("%d files processed in %g seconds.\n", n_input_files, g_timer_elapsed (timer, NULL));

	g_timer_destroy (timer);

	return 0;
}

/* vim: set sw=8 sts=8: -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 8 -*- */
