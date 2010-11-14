#include <glib.h>
#include <lsmdom.h>

static void
create_document_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;

	document = lsm_dom_implementation_create_document ("math");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert_cmpstr (lsm_dom_node_get_node_name (LSM_DOM_NODE (document)), ==, "#document");
	g_assert (lsm_dom_node_get_node_value (LSM_DOM_NODE (document)) == NULL);

	g_object_unref (document);

	document = lsm_dom_implementation_create_document ("svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert_cmpstr (lsm_dom_node_get_node_name (LSM_DOM_NODE (document)), ==, "#document");
	g_assert (lsm_dom_node_get_node_value (LSM_DOM_NODE (document)) == NULL);

	g_object_unref (document);
}

static void
create_element_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;

	document = lsm_dom_implementation_create_document ("svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert_cmpstr (lsm_dom_node_get_node_name (LSM_DOM_NODE (document)), ==, "#document");

	element = lsm_dom_document_create_element (document, "unknown");
	g_assert (element == NULL);

	element = lsm_dom_document_create_element (document, "svg");
	g_assert (LSM_IS_DOM_ELEMENT (element));

	g_object_unref (element);
	g_object_unref (document);
}

static void
add_remove_element_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;

	document = lsm_dom_implementation_create_document ("svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));

	element = lsm_dom_document_create_element (document, "unknown");
	g_assert (element == NULL);

	element = lsm_dom_document_create_element (document, "svg");
	g_assert (LSM_IS_DOM_ELEMENT (element));
	g_assert_cmpstr (lsm_dom_element_get_tag_name (LSM_DOM_ELEMENT (element)), ==, "svg");
	g_assert (lsm_dom_node_get_next_sibling (LSM_DOM_NODE (element)) == NULL);
	g_assert (lsm_dom_node_get_previous_sibling (LSM_DOM_NODE (element)) == NULL);
	g_assert (lsm_dom_node_get_parent_node (LSM_DOM_NODE (element)) == NULL);

	g_assert (lsm_dom_node_append_child (LSM_DOM_NODE (document), LSM_DOM_NODE (element)) ==
		  LSM_DOM_NODE (element));
	g_assert (lsm_dom_node_get_parent_node (LSM_DOM_NODE (element)) == LSM_DOM_NODE (document));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (element)) == document);
	g_assert (lsm_dom_node_get_first_child (LSM_DOM_NODE (document)) == LSM_DOM_NODE (element));

	g_object_ref (element);

	lsm_dom_node_remove_child (LSM_DOM_NODE (document), LSM_DOM_NODE (element));
	g_assert (lsm_dom_node_get_first_child (LSM_DOM_NODE (document)) == NULL);
	g_assert (lsm_dom_node_get_parent_node (LSM_DOM_NODE (element)) == NULL);
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (element)) == NULL);

	g_object_unref (element);
	g_object_unref (document);
}

int
main (int argc, char *argv[])
{
	int i;

	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/dom/create-document", create_document_test);
	g_test_add_func ("/dom/create-element", create_document_test);
	g_test_add_func ("/dom/add-remove-element", create_document_test);

	g_type_init ();

	return g_test_run();
}
