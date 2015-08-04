#include <glib.h>
#include <string.h>
#include <lsmdom.h>

GKeyFile *suite_options;

static GSList *
build_file_list (const char *path, GRegex *filename_regex)
{
	GSList *files = NULL;
	GDir *directory;
	GError *error = NULL;
	const char *entry;
	char *filename;

	directory = g_dir_open (path, 0, &error);
	if (error != NULL) {
		if (directory != NULL)
			g_dir_close (directory);
		g_error_free (error);
		return NULL;
	}

	do {
		entry = g_dir_read_name (directory);
		if (entry != NULL &&
		    strstr (entry, "ignore-") != entry &&
		    strcmp (entry, "images") != 0)
		{
			filename = g_build_filename (path, entry, NULL);

			if (g_file_test (filename, G_FILE_TEST_IS_DIR))
				files = g_slist_concat (files, build_file_list (filename, filename_regex));
			else if (g_file_test (filename, G_FILE_TEST_IS_REGULAR) &&
				 g_regex_match (filename_regex, filename, 0, NULL) &&
				 !g_key_file_get_boolean (suite_options, entry, "ignore", NULL)) {
				files = g_slist_prepend (files, g_strdup (filename));
			}

			g_free (filename);
		}
	} while (entry != NULL);

	g_dir_close (directory);

	return files;
}

static void
render_test (gconstpointer user_data)
{
	const char *filename = user_data;
	char *basename;
	LsmDomDocument *document;
	LsmDomView *view;
	LsmBox viewport;
	unsigned int width, height;
	cairo_t *cairo;
	cairo_surface_t *surface;

	basename = g_path_get_basename (filename);

	document = lsm_dom_document_new_from_path (filename, NULL);
	g_assert (LSM_IS_DOM_DOCUMENT (document));

	view = lsm_dom_document_create_view (document);
	g_assert (LSM_IS_DOM_VIEW (view));

	viewport.x = 0.0;
	viewport.y = 0.0;
	viewport.width = 480.0;
	viewport.height = 360.0;

	lsm_dom_view_set_resolution (view, 96);
	lsm_dom_view_set_viewport_pixels (view, &viewport);

	lsm_dom_view_get_size_pixels (LSM_DOM_VIEW (view), &width, &height, NULL);

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
	cairo = cairo_create (surface);
	cairo_surface_destroy (surface);

	if (cairo_status (cairo) == CAIRO_STATUS_SUCCESS) {
		lsm_dom_view_render (LSM_DOM_VIEW (view), cairo, 1, 1);

		if (cairo_status (cairo) != CAIRO_STATUS_SUCCESS) {
			g_test_message ("cairo_status = %s", cairo_status_to_string (cairo_status (cairo)));
			if (!g_key_file_get_boolean (suite_options, basename, "ignore-cairo-status", NULL))
				g_test_fail ();
		}
	}

	cairo_destroy (cairo);

	g_object_unref (view);
	g_object_unref (document);

	g_free (basename);
}

static void
add_render_tests (gpointer element, gpointer user_data)
{
	const char *filename = element;
	char *test_path = g_strdup_printf ("/render/%s", (char *) element + sizeof (SUITE_DATA_DIRECTORY));

	g_test_add_data_func (test_path, filename, render_test);

	g_free (test_path);
}

int
main (int argc, char *argv[])
{
	GRegex *filename_regex;
	GSList *files;
	int result;

#if !GLIB_CHECK_VERSION(2,36,0)
	g_type_init ();
#endif

	suite_options = g_key_file_new ();
	g_assert (g_key_file_load_from_file (suite_options, SUITE_OPTION_FILE, G_KEY_FILE_NONE, NULL));

	g_test_init (&argc, &argv, NULL);

	filename_regex = g_regex_new ("\\.(svg|mml)$", 0, 0, NULL);
	files = build_file_list (SUITE_DATA_DIRECTORY, filename_regex);
	g_slist_foreach (files, add_render_tests, NULL);

	result = g_test_run();

	g_slist_foreach (files, (GFunc) g_free, NULL);
	g_slist_free (files);
	g_regex_unref (filename_regex);

	g_key_file_free (suite_options);

	lsm_shutdown ();

	return result;
}
