#include <glib.h>
#include <lsmdom.h>

static void
create_document_test (void)
{
	LsmDomDocument *document;

	document = lsm_dom_implementation_create_document ("math");
	g_assert (LSM_IS_DOM_DOCUMENT (document));

	g_object_unref (document);

	document = lsm_dom_implementation_create_document ("svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));

	g_object_unref (document);
}

int
main (int argc, char *argv[])
{
	int i;

	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/dom/create-document", create_document_test);

	g_type_init ();

	return g_test_run();
}
