#include <glib.h>
#include <lsmdom.h>

static void
_weak_ref_cb (void *data, GObject *object)
{
	int *counter = data;

	(*counter)++;
}

static void
create_document_test (void)
{
	LsmDomDocument *document;

	document = lsm_dom_implementation_create_document (NULL, "math");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert_cmpstr (lsm_dom_node_get_node_name (LSM_DOM_NODE (document)), ==, "#document");
	g_assert (lsm_dom_node_get_node_value (LSM_DOM_NODE (document)) == NULL);

	g_object_unref (document);

	document = lsm_dom_implementation_create_document (NULL, "svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert_cmpstr (lsm_dom_node_get_node_name (LSM_DOM_NODE (document)), ==, "#document");
	g_assert (lsm_dom_node_get_node_value (LSM_DOM_NODE (document)) == NULL);

	g_object_unref (document);
}

static void
owner_document_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;
	LsmDomElement *text_element;
	LsmDomText *text;
	int counter = 0;

	document = lsm_dom_implementation_create_document (NULL, "svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (document)) == NULL);

	element = lsm_dom_document_create_element (document, "svg");
	g_assert (LSM_IS_DOM_ELEMENT (element));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (element)) == document);

	lsm_dom_node_append_child (LSM_DOM_NODE (document), LSM_DOM_NODE (element));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (element)) == document);

	text_element = lsm_dom_document_create_element (document, "text");
	g_assert (LSM_IS_DOM_ELEMENT (text_element));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (text_element)) == document);

	lsm_dom_node_append_child (LSM_DOM_NODE (element), LSM_DOM_NODE (text_element));

	text = lsm_dom_document_create_text_node (document, "");
	g_assert (LSM_IS_DOM_TEXT (text));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (text)) == document);

	lsm_dom_node_append_child (LSM_DOM_NODE (text_element), LSM_DOM_NODE (text));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (text)) == document);

	g_object_weak_ref (G_OBJECT (document), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (element), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (text_element), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (text), _weak_ref_cb, &counter);

	g_object_unref (document);

	g_assert_cmpint (counter, ==, 4);
}

static void
owner_mismatch_test (void)
{
	LsmDomDocument *document_a;
	LsmDomDocument *document_b;
	LsmDomElement *element_a;
	LsmDomElement *element_b;
	LsmDomElement *element_c;
	int counter = 0;

	document_a = lsm_dom_implementation_create_document (NULL, "svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document_a));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (document_a)) == NULL);

	element_a = lsm_dom_document_create_element (document_a, "svg");
	element_b = lsm_dom_document_create_element (document_a, "text");

	document_b = lsm_dom_implementation_create_document (NULL, "svg");
	g_assert (LSM_IS_DOM_DOCUMENT (document_b));
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (document_b)) == NULL);

	element_c = lsm_dom_document_create_element (document_b, "text");

	g_assert (lsm_dom_node_append_child (LSM_DOM_NODE (document_a), LSM_DOM_NODE (element_a)) == LSM_DOM_NODE (element_a));
	g_assert (lsm_dom_node_append_child (LSM_DOM_NODE (element_a), LSM_DOM_NODE (element_b)) == LSM_DOM_NODE (element_b));

	g_object_weak_ref (G_OBJECT (document_a), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (document_b), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (element_a), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (element_c), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (element_b), _weak_ref_cb, &counter);

	g_assert (lsm_dom_node_append_child (LSM_DOM_NODE (element_a), LSM_DOM_NODE (element_c)) == NULL);

	g_object_unref (document_a);
	g_object_unref (document_b);

	g_assert_cmpint (counter,  ==, 5);
}

static void
create_element_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;

	document = lsm_dom_implementation_create_document (NULL, "svg");
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

	document = lsm_dom_implementation_create_document (NULL, "svg");
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

	lsm_dom_node_remove_child (LSM_DOM_NODE (document), LSM_DOM_NODE (element));
	g_assert (lsm_dom_node_get_first_child (LSM_DOM_NODE (document)) == NULL);
	g_assert (lsm_dom_node_get_parent_node (LSM_DOM_NODE (element)) == NULL);
	g_assert (lsm_dom_node_get_owner_document (LSM_DOM_NODE (element)) != NULL);

	g_object_unref (element);
	g_object_unref (document);
}

static void
node_list_test (void)
{
	LsmDomDocument *document;
	LsmDomElement *element;
	LsmDomNodeList *childs;
	int counter = 0;

	document = lsm_dom_implementation_create_document (NULL, "svg");
	element = lsm_dom_document_create_element (document, "svg");
	lsm_dom_node_append_child (LSM_DOM_NODE (document), LSM_DOM_NODE (element));

	childs = lsm_dom_node_get_child_nodes (LSM_DOM_NODE (document));
	g_assert (LSM_IS_DOM_NODE_LIST (childs));
	g_assert (lsm_dom_node_list_get_length (childs) == 1);
	g_assert (lsm_dom_node_list_get_item (childs, 0) == LSM_DOM_NODE (element));
	g_assert (lsm_dom_node_list_get_item (childs, 1) == NULL);

	g_object_weak_ref (G_OBJECT (document), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (element), _weak_ref_cb, &counter);
	g_object_weak_ref (G_OBJECT (childs), _weak_ref_cb, &counter);

	g_object_ref (childs);
	g_object_unref (document);

	g_assert (lsm_dom_node_list_get_length (childs) == 0);

	g_object_unref (childs);

	g_assert_cmpint (counter, ==, 3);
}

static void
insert_before_test (void)
{
	LsmDomDocument *document;
	LsmDomNode *node;
	LsmDomNodeList *childs;
	LsmDomElement *math;
	LsmDomElement *mn1;
	LsmDomElement *mn2;
	LsmDomElement *mo1;
	LsmDomElement *mo2;

	document = lsm_dom_implementation_create_document (NULL, "math");
	math = lsm_dom_document_create_element (document, "math");
	lsm_dom_node_append_child (LSM_DOM_NODE (document), LSM_DOM_NODE (math));
	mn1 = lsm_dom_document_create_element (document, "mn");
	mn2 = lsm_dom_document_create_element (document, "mn");
	mo1 = lsm_dom_document_create_element (document, "mo");

	node = lsm_dom_node_append_child (LSM_DOM_NODE (math), LSM_DOM_NODE (mn2));
	g_assert (node == LSM_DOM_NODE (mn2));
	node = lsm_dom_node_insert_before (LSM_DOM_NODE (math), LSM_DOM_NODE (mn1), LSM_DOM_NODE (mn2));
	g_assert (node == LSM_DOM_NODE (mn1));
	node = lsm_dom_node_insert_before (LSM_DOM_NODE (math), LSM_DOM_NODE (mo1), LSM_DOM_NODE (mn2));
	g_assert (node == LSM_DOM_NODE (mo1));

	childs = lsm_dom_node_get_child_nodes (LSM_DOM_NODE (math));
	g_assert (LSM_IS_DOM_NODE_LIST (childs));

	g_assert (lsm_dom_node_list_get_item (childs, 0) == LSM_DOM_NODE (mn1));
	g_assert (lsm_dom_node_list_get_item (childs, 1) == LSM_DOM_NODE (mo1));
	g_assert (lsm_dom_node_list_get_item (childs, 2) == LSM_DOM_NODE (mn2));
	g_assert (lsm_dom_node_list_get_length (childs) == 3);

	mo2 = lsm_dom_document_create_element (document, "mo");
	node = lsm_dom_node_replace_child (LSM_DOM_NODE (math), LSM_DOM_NODE (mo2), LSM_DOM_NODE (mo1));
	g_assert (node == LSM_DOM_NODE (mo1));

	g_assert (lsm_dom_node_list_get_item (childs, 1) == LSM_DOM_NODE (mo2));
	g_assert (lsm_dom_node_list_get_length (childs) == 3);

	g_object_unref (mo1);
	g_object_unref (document);
}

int
main (int argc, char *argv[])
{
	int result;

	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/dom/create-document", create_document_test);
	g_test_add_func ("/dom/owner-document", owner_document_test);
	g_test_add_func ("/dom/owner-mismatch", owner_mismatch_test);
	g_test_add_func ("/dom/create-element", create_element_test);
	g_test_add_func ("/dom/add-remove-element", add_remove_element_test);
	g_test_add_func ("/dom/node-list", node_list_test);
	g_test_add_func ("/dom/insert-before", insert_before_test);

	result = g_test_run();

	lsm_shutdown ();

	return result;
}
