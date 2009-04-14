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

#include <lsmdebug.h>
#include <lsmdomimplementation.h>
#include <lsmmathmlpresentationtoken.h>
#include <lsmmathmlentitydictionary.h>
#include <lsmsvgtextelement.h>
#include <libxml/parser.h>
#include <string.h>

typedef enum {
	STATE
} LsmDomSaxParserStateEnum;

typedef struct {
	LsmDomSaxParserStateEnum state;

	LsmDomDocument *document;
	LsmDomNode *current_node;

	gboolean is_error;

	int error_depth;
} LsmDomSaxParserState;

static void
lsm_dom_parser_start_document (void *user_data)
{
	LsmDomSaxParserState *state = user_data;

	state->state = STATE;
	state->document = NULL;
	state->is_error = FALSE;
	state->error_depth = 0;
}

static void
lsm_dom_parser_end_document (void *user_data)
{
}

static void
lsm_dom_parser_start_element(void *user_data,
			     const xmlChar *name,
			     const xmlChar **attrs)
{
	LsmDomSaxParserState *state = user_data;
	LsmDomNode *node;
	int i;

	if (state->is_error) {
		state->error_depth++;
		return;
	}

	if (strcmp ((char *) name, "math") == 0 ||
	    strcmp ((char *) name, "svg") == 0) {
		if (state->document != NULL)
			g_object_unref (state->document);

		state->document = lsm_dom_implementation_create_document ((char *)name);
		state->current_node = LSM_DOM_NODE (state->document);
	}

	g_return_if_fail (LSM_DOM_IS_DOCUMENT (state->document));

	node = LSM_DOM_NODE (lsm_dom_document_create_element (LSM_DOM_DOCUMENT (state->document), (char *) name));

	if (LSM_DOM_IS_NODE (node)) {
		lsm_dom_node_append_child (state->current_node, node);

		if (attrs != NULL)
			for (i = 0; attrs[i] != NULL && attrs[i+1] != NULL; i += 2)
				lsm_dom_element_set_attribute (LSM_DOM_ELEMENT (node),
							    (char *) attrs[i],
							    (char *) attrs[i+1]);

		state->current_node = node;
		state->is_error = FALSE;
		state->error_depth = 0;
	} else {
		state->is_error = TRUE;
		state->error_depth = 1;
	}
}

static void
lsm_dom_parser_end_element (void *user_data,
			    const xmlChar *name)
{
	LsmDomSaxParserState *state = user_data;

	if (state->is_error) {
		state->error_depth--;
		if (state->error_depth > 0)
			return;

		state->is_error = FALSE;
		return;
	}

	state->current_node = lsm_dom_node_get_parent_node (state->current_node);
}

static void
lsm_dom_parser_characters (void *user_data, const xmlChar *ch, int len)
{
	LsmDomSaxParserState *state = user_data;

	if ((LSM_MATHML_IS_PRESENTATION_TOKEN (state->current_node) ||
	     LSM_SVG_IS_TEXT_ELEMENT (state->current_node)) &&
	    !state->is_error) {
		LsmDomNode *node;
		char *text;

		text = g_strndup ((char *) ch, len);
		node = LSM_DOM_NODE (lsm_dom_document_create_text_node (LSM_DOM_DOCUMENT (state->document), text));
		lsm_dom_node_append_child (state->current_node, node);
		g_free (text);
	}
}

static xmlEntityPtr
lsm_dom_parser_get_entity (void *user_data, const xmlChar *name)
{
	const char *utf8;

	utf8 = lsm_mathml_entity_get_utf8 ((char *) name);
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
lsm_dom_parser_warning (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_WARNING, msg, args);
	va_end(args);
}

static void
lsm_dom_parser_error (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_CRITICAL, msg, args);
	va_end(args);
}

static void
lsm_dom_parser_fatal_error (void *user_data, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	g_logv("XML", G_LOG_LEVEL_ERROR, msg, args);
	va_end(args);
}
#endif

static xmlSAXHandler sax_handler = {
#if 0
	.warning = lsm_dom_parser_warning,
	.error = lsm_dom_parser_error,
	.fatalError = lsm_dom_parser_fatal_error,
#endif
	.startDocument = lsm_dom_parser_start_document,
	.endDocument = lsm_dom_parser_end_document,
	.startElement = lsm_dom_parser_start_element,
	.endElement = lsm_dom_parser_end_element,
	.characters = lsm_dom_parser_characters,
	.getEntity = lsm_dom_parser_get_entity
};

LsmDomDocument *
lsm_dom_document_new_from_file (const char *filename)
{
	static LsmDomSaxParserState state;

	if (xmlSAXUserParseFile (&sax_handler, &state, filename) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[LsmDomParser::from_memory] invalid document");
		return NULL;
	}

	return LSM_DOM_DOCUMENT (state.document);
}

LsmDomDocument *
lsm_dom_document_new_from_memory (const char *buffer)
{
	static LsmDomSaxParserState state;

	if (buffer == NULL)
		return NULL;

	state.document = NULL;

	if (xmlSAXUserParseMemory (&sax_handler, &state, buffer, strlen (buffer)) < 0) {
		if (state.document !=  NULL)
			g_object_unref (state.document);
		g_warning ("[LsmDomParser::from_memory] invalid document");
		return NULL;
	}

	return LSM_DOM_DOCUMENT (state.document);
}