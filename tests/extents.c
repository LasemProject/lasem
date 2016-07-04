#include <lsm.h>
#include <lsmdom.h>

typedef struct {
	const char *name;
	const char *filename;
	double x;
	double y;
	double width;
	double height;
} ObjectsData;

ObjectsData objects_data[] = {
	{"/extents/rect",	"rect.svg",	10.0, 10.0, 10.0, 10.0}
}; 

static void
objects (ObjectsData *data)
{
	LsmDomDocument *document;
	LsmDomView *view;
	char *filename;
	double x, y, width, height;
	LsmBox viewport;

	filename = g_build_filename ("data", "svg", "extents", data->filename, NULL);
	document = lsm_dom_document_new_from_path (filename, NULL);
	g_free (filename);
	g_assert (LSM_IS_DOM_DOCUMENT (document));

	view = lsm_dom_document_create_view (document);
	g_assert (LSM_IS_DOM_VIEW (view));

	viewport.x = 0.0;
	viewport.y = 0.0;
	viewport.width = 480.0;
	viewport.height = 360.0;

	lsm_dom_view_set_resolution (view, 96);
	lsm_dom_view_set_viewport_pixels (view, &viewport);

	lsm_dom_view_get_extents (view, &x, &y, &width, &height);
	g_assert_cmpfloat (x, ==, data->x);
	g_assert_cmpfloat (y, ==, data->y);
	g_assert_cmpfloat (width, ==, data->width);
	g_assert_cmpfloat (height, ==, data->height);

	g_object_unref (view);
	g_object_unref (document);
}

int
main (int argc, char *argv[])
{
	int result;
	unsigned i;

	g_test_init (&argc, &argv, NULL);

#if !GLIB_CHECK_VERSION(2,36,0)
	g_type_init ();
#endif

	for (i = 0; i < G_N_ELEMENTS (objects_data); i++)
		g_test_add_data_func (objects_data[i].name,
				      &objects_data[i],
				      (void *) objects);

	result = g_test_run ();

	lsm_shutdown ();

	return result;
}

