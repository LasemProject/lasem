/* gmathmlparser.c
 *
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

#include <gdom.h>
#include <gdomdocument.h>
#include <gmathml.h>
#include <gmathmldocument.h>
#include <gmathmlpresentationtoken.h>
#include <gmathmlentitydictionary.h>
#include <libxml/parser.h>
#include <string.h>

#include <../itex2mml/itex2MML.h>

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
		g_free (text);
	}
}

static xmlEntityPtr
gmathml_parser_get_entity (void *user_data, const xmlChar *name)
{
	const char *utf8;

	utf8 = gmathml_entity_get_utf8 ((char *) name);
	if (utf8 != NULL) {
		xmlEntity *entity;

		entity = g_new0 (xmlEntity, 1);

		entity->type = XML_ENTITY_DECL;
		entity->name = name;
		entity->content = (xmlChar *) utf8;
		entity->length = g_utf8_strlen (utf8, -1);
		entity->etype = XML_INTERNAL_PREDEFINED_ENTITY;

		return entity;
	}

	return xmlGetPredefinedEntity(name);
}

static xmlSAXHandler sax_handler = {
	.startDocument = gmathml_parser_start_document,
	.endDocument = gmathml_parser_end_document,
	.startElement = gmathml_parser_start_element,
	.endElement = gmathml_parser_end_element,
	.characters = gmathml_parser_characters,
	.getEntity = gmathml_parser_get_entity
};

GMathmlDocument *
gmathml_document_new_from_file (const char *filename)
{
	static GMathmlSaxParserState state;

	if (xmlSAXUserParseFile (&sax_handler, &state, filename) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GMathmlParser::from_memory] invalid document");
		return NULL;
	}

	return GMATHML_DOCUMENT (state.document);
}

GMathmlDocument *
gmathml_document_new_from_memory (const char *buffer)
{
	static GMathmlSaxParserState state;

	if (buffer == NULL)
		return GMATHML_DOCUMENT (gmathml_document_new ());

	state.document = NULL;

	if (xmlSAXUserParseMemory (&sax_handler, &state, buffer, strlen (buffer) - 10) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GMathmlParser::from_memory] invalid document");
		return NULL;
	}

	return GMATHML_DOCUMENT (state.document);
}

static void
_dummy_error (const char *msg)
{
}

GMathmlDocument *
gmathml_document_new_from_itex (const char *itex)
{
	GMathmlDocument *document;
	char *mathml;

	g_return_val_if_fail (itex != NULL, NULL);

	itex2MML_error = _dummy_error;

	mathml = itex2MML_parse (itex, strlen (itex));
	document = gmathml_document_new_from_memory (mathml);
	itex2MML_free_string (mathml);

	return document;
}
