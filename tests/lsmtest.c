/* Lasem - SVG and Mathml library
 *
 * lsm-test - Regression test utility for Lasem
 *
 * Copyright © 2004 Richard D. Worth
 * Copyright © 2006 Red Hat, Inc.
 * Copyright © 2007-2012 Emmanuel Pacaud
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the authors
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * The authors make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: Emmanuel Pacaud <emmanuel@gnome.org>
 *	    Richard D. Worth <richard@theworths.org>
 *	    Carl Worth <cworth@cworth.org>
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
#include <glib/gprintf.h>
#include <gio/gio.h>

#include <libxml/parser.h>

#include <../itex2mml/itex2MML.h>

#define XML_FILENAME	"lasemtest.xml"

static char *option_debug_domains = NULL;
static char **option_input_filenames = NULL;
double option_ppi = 72.0;
static gboolean option_fatal_warning = FALSE;
static gboolean option_debug_filter = FALSE;
static gboolean option_debug_pattern = FALSE;
static gboolean option_debug_mask = FALSE;
static gboolean option_dry_run = FALSE;
static double option_compare_fuzz = 10.0;

static const GOptionEntry entries[] =
{
	{ G_OPTION_REMAINING,	' ', 0,	G_OPTION_ARG_FILENAME_ARRAY,
		&option_input_filenames, 	NULL, NULL},
	{ "ppi", 		'p', 0, G_OPTION_ARG_DOUBLE,
		&option_ppi, 			"Pixel per inch", NULL },
	{ "debug", 		'd', 0, G_OPTION_ARG_STRING,
		&option_debug_domains,		"Debug domains", NULL },
	{ "fatal-warning", 	'f', 0, G_OPTION_ARG_NONE,
		&option_fatal_warning,		"Make warning fatal", NULL },
	{ "debug-filter", 	' ' , 0, G_OPTION_ARG_NONE,
		&option_debug_filter,		"Debug filter surfaces", NULL },
	{ "debug-pattern", 	' ' , 0, G_OPTION_ARG_NONE,
		&option_debug_pattern,		"Debug pattern surfaces", NULL },
	{ "debug-mask", 	' ' , 0, G_OPTION_ARG_NONE,
		&option_debug_mask,		"Debug mask surfaces", NULL },
	{ "dry-run",		'n' , 0, G_OPTION_ARG_NONE,
		&option_dry_run,		"Don't write files", NULL },
	{ "compare-fuzz",	'z', 0, G_OPTION_ARG_DOUBLE,
		&option_compare_fuzz,		"Compare fuzz", NULL},
	{ NULL }
};

static const char *fail_face = "";
static const char *success_face = "";
static const char *normal_face = "";
FILE *lasem_test_html_file = NULL;

typedef struct {
	double elapsed_time;
	unsigned int rendered_count;
	unsigned int comparison_count;
	unsigned int failed_count;
	unsigned int success_count;
} Statistic;

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

typedef struct _buffer_diff_result {
    unsigned int pixels_changed;
    unsigned int max_diff;
} buffer_diff_result_t;

typedef guint32 pixman_bits_t;

static void
buffer_diff_core (unsigned char *_buf_a,
		  unsigned char *_buf_b,
		  unsigned char *_buf_diff,
		  int		width,
		  int		height,
		  int		stride,
		  pixman_bits_t mask,
		  buffer_diff_result_t *result_ret)
{
	int x, y;
	pixman_bits_t *row_a, *row_b, *row;
	buffer_diff_result_t result = {0, 0};
	pixman_bits_t *buf_a = (pixman_bits_t*)_buf_a;
	pixman_bits_t *buf_b = (pixman_bits_t*)_buf_b;
	pixman_bits_t *buf_diff = (pixman_bits_t*)_buf_diff;

	stride /= sizeof(pixman_bits_t);
	for (y = 0; y < height; y++)
	{
		row_a = buf_a + y * stride;
		row_b = buf_b + y * stride;
		row = buf_diff + y * stride;
		for (x = 0; x < width; x++)
		{
			/* check if the pixels are the same */
			if ((row_a[x] & mask) != (row_b[x] & mask)) {
				int channel;
				pixman_bits_t diff_pixel = 0;

				/* calculate a difference value for all 4 channels */
				for (channel = 0; channel < 4; channel++) {
					int value_a = (row_a[x] >> (channel*8)) & 0xff;
					int value_b = (row_b[x] >> (channel*8)) & 0xff;
					unsigned int diff;
					diff = abs (value_a - value_b);
					if (diff > result.max_diff)
						result.max_diff = diff;
					diff *= 4;  /* emphasize */
					if (diff)
						diff += 128; /* make sure it's visible */
					if (diff > 255)
						diff = 255;
					diff_pixel |= diff << (channel*8);
				}

				result.pixels_changed++;
				row[x] = diff_pixel;
			} else {
				row[x] = 0;
			}
			row[x] |= 0xff000000; /* Set ALPHA to 100% (opaque) */
		}
	}

	*result_ret = result;
}

static gboolean
compare_surfaces (const char *test_name, cairo_surface_t *surface_a, cairo_surface_t *surface_b)
{
	int width_a, width_b, height_a, height_b, stride_a, stride_b;

	if (surface_b == NULL)
		return FALSE;
	if (surface_a == NULL)
		return FALSE;

	width_a = cairo_image_surface_get_width (surface_a);
	height_a = cairo_image_surface_get_height (surface_a);
	stride_a = cairo_image_surface_get_stride (surface_a);
	width_b = cairo_image_surface_get_width (surface_b);
	height_b = cairo_image_surface_get_height (surface_b);
	stride_b = cairo_image_surface_get_stride (surface_b);

	if (width_a  == width_b && height_a == height_b && stride_a == stride_b) {
		buffer_diff_result_t result;
		cairo_surface_t *surface_diff;
		char *diff_png_filename;
		char *command;
		char *command_result = NULL;
		int command_status;

		diff_png_filename = g_strdup_printf ("%s-diff.png", test_name);

		surface_diff = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
							   width_a, height_a);

		buffer_diff_core (cairo_image_surface_get_data (surface_a),
				  cairo_image_surface_get_data (surface_b),
				  cairo_image_surface_get_data (surface_diff),
				  cairo_image_surface_get_width (surface_a),
				  cairo_image_surface_get_height (surface_a),
				  cairo_image_surface_get_stride (surface_a),
				  0xffffffff,
				  &result);

		cairo_surface_write_to_png (surface_diff, diff_png_filename);

		cairo_surface_destroy (surface_diff);
		g_free (diff_png_filename);

		if (result.pixels_changed == 0) {
			g_printf (" %sOK%s \n", success_face, normal_face);
			return TRUE;
		}

		command = g_strdup_printf ("compare -metric AE -fuzz %g%% %s-out.png %s-ref.png %s-compare-diff.png",
					   option_compare_fuzz, test_name, test_name, test_name);
		g_spawn_command_line_sync (command, NULL, &command_result, &command_status, NULL);
		g_free (command);

		if (command_result != NULL && atoi (command_result) == 0) {
			g_printf (" %sOK%s \n", success_face, normal_face);
			return TRUE;
		}
	}

	g_printf (" %sFAIL%s \n", fail_face, normal_face);
	return FALSE;
}

void
lasem_test_render (char const *filename, gboolean compare, gboolean dry_run, Statistic *statistic)
{
	LsmDomDocument *document;
	LsmDomView *view;
	GTimer *timer;
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
	gboolean check;
	GRegex *regex;
	GError *error = NULL;
	char *filtered_buffer;

	g_return_if_fail (statistic != NULL);

	test_name = g_regex_replace (regex_mml, filename, -1, 0, "", 0, NULL);

	png_filename = g_strdup_printf ("%s-out.png", test_name);
	reference_png_filename = g_strdup_printf ("%s-ref.png", test_name);
	if (g_file_test (reference_png_filename, G_FILE_TEST_IS_REGULAR)) {
		check = compare;
	} else {
		g_free (reference_png_filename);
		reference_png_filename = g_strdup_printf ("%s.png", test_name);
		check = FALSE;
	}

	mime = g_content_type_guess (filename, NULL, 0, NULL);

	is_svg = strcmp (mime, "image/svg+xml") == 0;
	is_mathml = (strcmp (mime, "text/mathml") == 0) || (strcmp (mime, "application/mathml+xml") == 0);
	is_xml = is_svg || is_mathml;

	g_printf ("\trender %s (%s)", filename, mime);
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

		timer = g_timer_new ();

		document = lsm_dom_document_new_from_memory (xml, size, NULL);

		lsm_dom_document_set_path (document, filename);

		view = lsm_dom_document_create_view (document);

		viewport.x = 0.0;
		viewport.y = 0.0;
		viewport.width = 480.0;
		viewport.height = 360.0;

		lsm_dom_view_set_resolution (view, option_ppi);
		lsm_dom_view_set_viewport_pixels (view, &viewport);
		lsm_dom_view_get_size_pixels (LSM_DOM_VIEW (view), &width, &height, NULL);

		if (option_debug_mask)
			lsm_dom_view_set_debug (view, "mask", TRUE);
		if (option_debug_pattern)
			lsm_dom_view_set_debug (view, "pattern", TRUE);
		if (option_debug_filter)
			lsm_dom_view_set_debug (view, "filter", TRUE);

		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width + 2, height + 2);
		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);

		lsm_dom_view_render (LSM_DOM_VIEW (view), cairo, 1, 1);

		statistic->elapsed_time +=  g_timer_elapsed (timer, NULL);
		statistic->rendered_count++;
		g_timer_destroy (timer);

		if (!dry_run)
			cairo_surface_write_to_png (surface, png_filename);

		if (check) {
			cairo_surface_t *reference_surface;

			reference_surface = cairo_image_surface_create_from_png (reference_png_filename);
			if (reference_surface != NULL) {
				gboolean same;

				same = compare_surfaces (test_name, surface, reference_surface); 
				cairo_surface_destroy (reference_surface);

				if (same)
					statistic->success_count++;
				else
					statistic->failed_count++;
				statistic->comparison_count++;
			}
		} else
			g_printf ("\n");

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
			lasem_test_html ("\" width=\"%dpx\"/>", width + 2);
		}

		lasem_test_html ("</td>");
		lasem_test_html ("</tr>\n");
		lasem_test_html ("</table>\n");

		if (!is_xml && !g_file_test (reference_png_filename, G_FILE_TEST_IS_REGULAR) && !dry_run) {
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

		if (xml != buffer && !dry_run) {
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

void
lasem_test_process_dir (const char *name, gboolean compare, gboolean dry_run, Statistic *statistic)
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
				lasem_test_process_dir (filename, compare, dry_run, statistic);
			else if (g_file_test (filename, G_FILE_TEST_IS_REGULAR) &&
				 g_regex_match (regex_mml, filename, 0, NULL)) {
				lasem_test_render (filename, compare, dry_run, statistic);
				n_files++;
			}

			g_free (filename);
		}
	} while (entry != NULL);

	g_dir_close (directory);
}

static gboolean
check_for_compare (void)
{
	char *result, *output;

	g_spawn_command_line_sync ("compare --version", &output, &result, NULL, NULL);

	g_free (output);

	if (result != NULL) {
		g_free (result);
		return TRUE;
	}

	g_printf ("Compare utility not found.\nPlease install ImageMagick.\n");
	return FALSE;
}

int
main (int argc, char **argv)
{
	GOptionContext *context;
	GError *error = NULL;
	unsigned int i;
	unsigned int n_input_files = 0;
	Statistic statistic = {0, 0, 0, 0, 0};

	if (!check_for_compare())
		return EXIT_FAILURE;

#ifdef HAVE_UNISTD_H
	if (isatty (2)) {
		fail_face = "\033[41m\033[37m\033[1m";
		success_face = "\033[42m\033[37m\033[1m";
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

	if (option_fatal_warning)
		g_log_set_fatal_mask ("Lasem", G_LOG_FATAL_MASK | G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_WARNING);

	regex_mml = g_regex_new ("\\.(mml|tex|svg)$", 0, 0, &error);
	assert (error == NULL);

	n_input_files = option_input_filenames != NULL ? g_strv_length (option_input_filenames) : 0;
	if (n_input_files == 1 && g_file_test (option_input_filenames[0], G_FILE_TEST_IS_DIR))
		lasem_test_process_dir (option_input_filenames[0], TRUE, option_dry_run, &statistic);
	else {
		if (n_input_files > 0)
			for (i = 0; i < n_input_files; i++)
				lasem_test_render (option_input_filenames[i], TRUE, option_dry_run, &statistic);
		else
			lasem_test_process_dir (".", TRUE, option_dry_run, &statistic);
	}

	lasem_test_html ("</body>\n");
	lasem_test_html ("</html>\n");

	if (lasem_test_html_file != NULL)
		fclose (lasem_test_html_file);

	g_regex_unref (regex_mml);

	g_printf ("%d files processed in %g seconds.\n", statistic.rendered_count, statistic.elapsed_time);
	if (statistic.comparison_count > 0)
		g_printf ("%s%d/%d%s comparison failures.\n",
			  statistic.failed_count > 0 ? fail_face : success_face,
			  statistic.failed_count,
			  statistic.comparison_count,
			  normal_face);

	return 0;
}

/* vim: set sw=8 sts=8: -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 8 -*- */
