/*
 * Copyright © 2007-2015 Emmanuel Pacaud
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lsm.h>
#include <lsmmathml.h>
#include <lsmsvg.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
#include <cairo-ps.h>
#include <../config.h>

#include <../itex2mml/itex2MML.h>

static char *option_debug_domains = NULL;
static char *option_output_file_format = NULL;
static char **option_input_filenames = NULL;
static char *option_output_filename = NULL;
static char *option_element_id = NULL;
static char *option_offset = NULL;
static char *option_size = NULL;
double option_ppi = 72.0;
double option_zoom = 1.0;

typedef enum {
	FORMAT_SVG,
	FORMAT_PDF,
	FORMAT_PS,
	FORMAT_PNG,
	FORMAT_UNKNOWN
} FileFormat;

static char const *file_formats[] = {
	"svg",
	"pdf",
	"ps",
	"png",
	"svg"
};

static const GOptionEntry entries[] =
{
	{ G_OPTION_REMAINING,	' ', 0,	G_OPTION_ARG_FILENAME_ARRAY,
		&option_input_filenames, 	NULL, NULL},
	{ "output",		'o', 0, G_OPTION_ARG_FILENAME,
		&option_output_filename,	N_("Output filename"), NULL},
	{ "format", 		'f', 0, G_OPTION_ARG_STRING,
		&option_output_file_format, 	N_("Output format"), NULL },
	{ "ppi", 		'p', 0, G_OPTION_ARG_DOUBLE,
		&option_ppi, 			N_("Pixel per inch"), NULL },
	{ "zoom", 		'z', 0, G_OPTION_ARG_DOUBLE,
		&option_zoom, 			N_("Zoom"), NULL },
	{ "id", 		'i', 0, G_OPTION_ARG_STRING,
		&option_element_id, 		N_("Element id"), NULL },
	{ "offset", 		't', 0, G_OPTION_ARG_STRING,
		&option_offset, 		N_("Offset"), NULL },
	{ "size", 		's', 0, G_OPTION_ARG_STRING,
		&option_size, 		N_("Size"), NULL },
	{ "debug", 		'd', 0, G_OPTION_ARG_STRING,
		&option_debug_domains,		N_("Debug domains"), NULL },
	{ NULL }
};

static gboolean
_hide_before_object (LsmDomDocument *document, const char *id)
{
	LsmDomNode *node;
	LsmDomNode *sibling;
	LsmDomNode *parent;

	/* TODO: Handle other document types */
	if (!LSM_IS_SVG_DOCUMENT (document)) {
		fprintf (stderr, "%s\n", _("Invalid document type (SVG expected)"));
		return  FALSE;
	}

	node = LSM_DOM_NODE (lsm_svg_document_get_element_by_id (LSM_SVG_DOCUMENT (document), id));
	if (!LSM_IS_DOM_NODE (node)) {
		fprintf (stderr, _("Element '%s' not found"), id);
		fprintf (stderr, "\n");
		return FALSE;
	}

	for (parent = lsm_dom_node_get_parent_node (node);
	     LSM_IS_DOM_NODE (parent);
	     node = parent, parent = lsm_dom_node_get_parent_node (parent)) {
		for (sibling = lsm_dom_node_get_first_child (parent);
		     LSM_IS_DOM_NODE (sibling);
		     sibling = lsm_dom_node_get_next_sibling (sibling)) {
			if (sibling != node && LSM_IS_SVG_ELEMENT (sibling))
				lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (sibling), "display", "none");
		}
	}

	return TRUE;
}

int
main(int argc, char **argv)
{
	LsmDomDocument *document;
	LsmDomView *view;
	cairo_t *cairo;
	cairo_surface_t *surface;
	GError *error = NULL;
	GOptionContext *context;
	FileFormat format;
	char *output_filename;
	char *input_filename;
	double offset_x_pt, offset_y_pt;
	double height_pt, width_pt;
	unsigned int height, width;
	double size[2];
	double offset[2];

	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

#if !GLIB_CHECK_VERSION(2,36,0)
	g_type_init ();
#endif

	context = g_option_context_new (NULL);
	g_option_context_add_main_entries (context, entries, NULL);

	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_option_context_free (context);
		fprintf (stderr, "%s %s\n", _("Option parsing failed:"), error->message);
		return EXIT_FAILURE;
	}

	g_option_context_free (context);

	if (option_zoom < 0.0) {
		fprintf (stderr, "%s\n", _("Invalid zoom value"));
		return EXIT_FAILURE;
	}

	if (option_offset != NULL) {
		unsigned n_values;

		n_values = lsm_str_parse_double_list (&option_offset, 2, offset);
		if (n_values != 2) {
			fprintf (stderr, "%s\n", _("Invalid number of arguments for offset option"));
			return EXIT_FAILURE;
		}
	}

	if (option_size != NULL) {
		unsigned n_values;

		n_values = lsm_str_parse_double_list (&option_size, 2, size);
		if (n_values != 2) {
			fprintf (stderr, "%s\n", _("Invalid number of arguments for size option"));
			return EXIT_FAILURE;
		}
		if (size[0] <= 0.0 ||
		    size[1] <= 0.0) {
			fprintf (stderr, "%s\n", _("Invalid export size"));
			return EXIT_FAILURE;
		}
	}

	lsm_debug_enable (option_debug_domains);

	if (option_input_filenames == NULL || g_strv_length (option_input_filenames) > 1) {
		fprintf (stderr, "%s\n", _("Missing input filename"));
		return EXIT_FAILURE;
	}

	input_filename = option_input_filenames[0];

	if (option_output_file_format != NULL) {
		for (format = 0; format < G_N_ELEMENTS(file_formats) - 1; format++)
			if (g_ascii_strcasecmp (option_output_file_format, file_formats[format]) == 0)
				break;
		if (FORMAT_UNKNOWN == format) {
			fprintf (stderr, "%s %s\n", _("Unknown format:"), option_output_file_format);
			return EXIT_FAILURE;
		}
	} else
		format = FORMAT_UNKNOWN;

	if (option_output_filename != NULL) {
		char *extension;

		output_filename = option_output_filename;

		extension = g_strrstr (output_filename, ".");
		if (extension != NULL) {
			extension++;
			for (format = 0; format < G_N_ELEMENTS(file_formats) - 1; format++)
				if (g_ascii_strcasecmp (extension, file_formats[format]) == 0)
					break;
		}
	} else {
		char *filename;
		char *directory;
		char *extension;
		char *new_filename;

		filename = g_path_get_basename (input_filename);
		directory = g_path_get_dirname (input_filename);

		extension = g_strrstr (filename, ".");
		if (extension != NULL)
			*extension = '\0';

		new_filename = g_strconcat (filename, ".", file_formats[format], NULL);
		output_filename = g_build_filename (directory, new_filename, NULL);

		g_free (filename);
		g_free (new_filename);
		g_free (directory);
	}

	if (format == FORMAT_UNKNOWN) {
		fprintf (stderr, "%s\n", _("Don't know which format to use, please either give a format (-f) or an output filename (-o)"));
		return EXIT_FAILURE;
	}

	document = lsm_dom_document_new_from_path (input_filename,
						   NULL);
	if (document == NULL) {
		document = lsm_dom_document_new_from_url (input_filename,
							  NULL);
		if (document == NULL) {
			document = LSM_DOM_DOCUMENT (lsm_mathml_document_new_from_itex_path (input_filename,
											     NULL));
			if (document == NULL) {
				document = LSM_DOM_DOCUMENT (lsm_mathml_document_new_from_itex_url (input_filename,
												    NULL));
			}
		}
	}

	if (document != NULL && option_element_id != NULL) {
		if (!_hide_before_object (document, option_element_id)) {
			g_object_unref (document);
			return EXIT_FAILURE;
		}
	}

	if (document != NULL) {
		if (lsm_debug_check (&lsm_debug_category_dom, LSM_DEBUG_LEVEL_LOG)) {
			void *buffer;
			gsize size;

			lsm_dom_document_save_to_memory (document, &buffer, &size, NULL);

			if (buffer != NULL) {
				printf ("%*s\n", (int) size, (char *) buffer);
				g_free (buffer);
			}
		}

		view = lsm_dom_document_create_view (document);
		lsm_dom_view_set_resolution (view, option_ppi);

		width_pt = 2.0;
		height_pt = 2.0;

		lsm_dom_view_get_size (view, &width_pt, &height_pt, NULL);
		lsm_dom_view_get_size_pixels (view, &width, &height, NULL);
		
		if (option_offset == NULL) {
			offset_x_pt = offset_y_pt = 0.0;
		} else {
			offset_x_pt = option_zoom * offset[0];
			offset_y_pt = option_zoom * offset[1];
		}

		if (option_size == NULL) {
			width_pt *= option_zoom;
			height_pt *= option_zoom;
			width *= option_zoom;
			height *= option_zoom;
		}  else {
			width_pt = option_zoom * size[0];
			height_pt = option_zoom * size[1];
			width = width_pt;
			height = height_pt;
		}

		switch (format) {
			case FORMAT_PDF:
				surface = cairo_pdf_surface_create (output_filename,
								    width_pt, height_pt);
				break;
			case FORMAT_PS:
				surface = cairo_ps_surface_create (output_filename,
								   width_pt, height_pt);
				break;
			case FORMAT_PNG:
				surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
								      width,
								      height);
				break;
			case FORMAT_SVG:
			default:
				surface = cairo_svg_surface_create (output_filename,
								    width_pt, height_pt);
				break;
		}

		cairo = cairo_create (surface);
		cairo_surface_destroy (surface);
		cairo_scale (cairo, option_zoom, option_zoom);

		lsm_dom_view_render (view, cairo, -offset_x_pt, -offset_y_pt);

		switch (format) {
			case FORMAT_PNG:
				cairo_surface_write_to_png (cairo_get_target (cairo),
							    output_filename);
				break;
			default:
				break;
		}

		cairo_destroy (cairo);

		g_object_unref (view);

		g_object_unref (document);

		lsm_debug_render ("width = %g pt, height = %g pt",  width_pt, height_pt);
	} else {
		fprintf (stderr, _("Can't load '%s'"), input_filename);
		fprintf (stderr, "\n");

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
