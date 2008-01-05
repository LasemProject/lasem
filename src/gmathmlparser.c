#include <gdom.h>
#include <gdomdocument.h>
#include <gmathml.h>
#include <gmathmldocument.h>
#include <gmathmlpresentationtoken.h>
#include <libxml/parser.h>
#include <string.h>

typedef enum {
	STATE
} GMathmlSaxParserStateEnum;

typedef struct {
	GMathmlSaxParserStateEnum state;

	GDomNode *document;
	GDomNode *current_node;

	gboolean is_error;
} GMathmlSaxParserState;

static void
gmathml_parser_start_document (void *user_data)
{
	GMathmlSaxParserState *state = user_data;

	state->state = STATE;
	state->document = NULL;
	state->is_error = FALSE;
}

static void
gmathml_parser_end_document (void *user_data)
{
}

static void
gmathml_parser_start_element(void *user_data,
			     const xmlChar *name,
			     const xmlChar **attrs)
{
	GMathmlSaxParserState *state = user_data;
	GDomNode *node;
	int i;

	if (strcmp ((char *) name, "math") == 0) {
		if (state->document != NULL)
			g_object_unref (state->document);

		state->document = gmathml_document_new ();
		state->current_node = state->document;
	}

	g_return_if_fail (GDOM_IS_DOCUMENT (state->document));

	node = GDOM_NODE (gdom_document_create_element (GDOM_DOCUMENT (state->document), (char *) name));

	if (GDOM_IS_NODE (node)) {
		gdom_node_append_child (state->current_node, node);

		if (attrs != NULL)
			for (i = 0; attrs[i] != NULL && attrs[i+1] != NULL; i += 2)
				gdom_element_set_attribute (GDOM_ELEMENT (node),
							    (char *) attrs[i],
							    (char *) attrs[i+1]);

		state->current_node = node;
		state->is_error = FALSE;
	} else
		state->is_error = TRUE;
}

static void
gmathml_parser_end_element (void *user_data,
			    const xmlChar *name)
{
	GMathmlSaxParserState *state = user_data;

	if (state->is_error)
		return;

	state->current_node = gdom_node_get_parent_node (state->current_node);
}

static void
gmathml_parser_characters (void *user_data, const xmlChar *ch, int len)
{
	GMathmlSaxParserState *state = user_data;

	if (GMATHML_IS_PRESENTATION_TOKEN (state->current_node) &&
	    !state->is_error) {
		GDomNode *node;
		char *text;

		text = g_strndup ((char *) ch, len);
		node = GDOM_NODE (gdom_document_create_text_node (GDOM_DOCUMENT (state->document), text));
		gdom_node_append_child (state->current_node, node);
		g_message ("%s", text);
		g_free (text);
	}
}

static xmlSAXHandler sax_handler = {
	.startDocument = gmathml_parser_start_document,
	.endDocument = gmathml_parser_end_document,
	.startElement = gmathml_parser_start_element,
	.endElement = gmathml_parser_end_element,
	.characters = gmathml_parser_characters
};

GDomNode *
gmathml_document_from_file (const char *filename)
{
	static GMathmlSaxParserState state;

	if (xmlSAXUserParseFile (&sax_handler, &state, filename) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GMathmlParser::from_memory] invalid document");
		return NULL;
	}

	return state.document;
}

GDomNode *
gmathml_document_from_memory (const char *buffer)
{
	static GMathmlSaxParserState state;

	state.document = NULL;

	if (xmlSAXUserParseMemory (&sax_handler, &state, buffer, strlen (buffer) - 10) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GMathmlParser::from_memory] invalid document");
		return NULL;
	}

	return state.document;
}
