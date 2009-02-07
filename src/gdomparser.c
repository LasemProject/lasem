/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#include <gdomdebug.h>
#include <gdomimplementation.h>
#include <gmathmlpresentationtoken.h>
#include <gmathmlentitydictionary.h>
#include <libxml/parser.h>
#include <string.h>

typedef enum {
	STATE
} GDomSaxParserStateEnum;

typedef struct {
	GDomSaxParserStateEnum state;

	GDomDocument *document;
	GDomNode *current_node;

	gboolean is_error;
} GDomSaxParserState;

static void
gdom_parser_start_document (void *user_data)
{
	GDomSaxParserState *state = user_data;

	state->state = STATE;
	state->document = NULL;
	state->is_error = FALSE;
}

static void
gdom_parser_end_document (void *user_data)
{
}

static void
gdom_parser_start_element(void *user_data,
			  const xmlChar *name,
			  const xmlChar **attrs)
{
	GDomSaxParserState *state = user_data;
	GDomNode *node;
	int i;

	if (strcmp ((char *) name, "math") == 0 ||
	    strcmp ((char *) name, "svg") == 0) {
		if (state->document != NULL)
			g_object_unref (state->document);

		state->document = gdom_implementation_create_document ((char *)name);
		state->current_node = GDOM_NODE (state->document);
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
gdom_parser_end_element (void *user_data,
			    const xmlChar *name)
{
	GDomSaxParserState *state = user_data;

	if (state->is_error)
		return;

	state->current_node = gdom_node_get_parent_node (state->current_node);
}

static void
gdom_parser_characters (void *user_data, const xmlChar *ch, int len)
{
	GDomSaxParserState *state = user_data;

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
gdom_parser_get_entity (void *user_data, const xmlChar *name)
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

#if 0
static void
gdom_parser_warning (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_WARNING, msg, args);
	va_end(args);
}

static void
gdom_parser_error (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_CRITICAL, msg, args);
	va_end(args);
}

static void
gdom_parser_fatal_error (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_ERROR, msg, args);
	va_end(args);
}
#endif

static xmlSAXHandler sax_handler = {
#if 0
	.warning = gdom_parser_warning,
	.error = gdom_parser_error,
	.fatalError = gdom_parser_fatal_error,
#endif
	.startDocument = gdom_parser_start_document,
	.endDocument = gdom_parser_end_document,
	.startElement = gdom_parser_start_element,
	.endElement = gdom_parser_end_element,
	.characters = gdom_parser_characters,
	.getEntity = gdom_parser_get_entity
};

GDomDocument *
gdom_document_new_from_file (const char *filename)
{
	static GDomSaxParserState state;

	if (xmlSAXUserParseFile (&sax_handler, &state, filename) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GDomParser::from_memory] invalid document");
		return NULL;
	}

	return GDOM_DOCUMENT (state.document);
}

GDomDocument *
gdom_document_new_from_memory (const char *buffer)
{
	static GDomSaxParserState state;

	if (buffer == NULL)
		return NULL;

	state.document = NULL;

	if (xmlSAXUserParseMemory (&sax_handler, &state, buffer, strlen (buffer)) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[GDomParser::from_memory] invalid document");
		return NULL;
	}

	return GDOM_DOCUMENT (state.document);
}
