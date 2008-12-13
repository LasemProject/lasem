/* gdomnode.h
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

#ifndef GDOM_NODE_H
#define GDOM_NODE_H

#include <gdom.h>

G_BEGIN_DECLS

typedef enum  {
	GDOM_NODE_TYPE_ELEMENT_NODE = 1,
	GDOM_NODE_TYPE_ATTRIBUTE_NODE,
	GDOM_NODE_TYPE_TEXT_NODE,
	GDOM_NODE_TYPE_CDATA_SECTION_NODE,
	GDOM_NODE_TYPE_ENTITY_REFERENCE_NODE,
	GDOM_NODE_TYPE_ENTITY_NODE,
	GDOM_NODE_TYPE_PROCESSING_INSTRUCTION_NODE,
	GDOM_NODE_TYPE_COMMENT_NODE,
	GDOM_NODE_TYPE_DOCUMENT_NODE,
	GDOM_NODE_TYPE_DOCUMENT_TYPE_NODE,
	GDOM_NODE_TYPE_DOCUMENT_FRAGMENT_NODE,
	GDOM_NODE_TYPE_NOTATION_NODE
} GDomNodeType;

#define GDOM_TYPE_NODE             (gdom_node_get_type ())
#define GDOM_NODE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_NODE, GDomNode))
#define GDOM_NODE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_NODE, GDomNodeClass))
#define GDOM_IS_NODE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_NODE))
#define GDOM_IS_NODE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_NODE))
#define GDOM_NODE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_NODE, GDomNodeClass))

typedef struct _GDomNodeClass GDomNodeClass;

struct _GDomNode {
	GObject	object;

	GDomNode	*next_sibling;
	GDomNode	*previous_sibling;
	GDomNode	*parent_node;
	GDomNode	*first_child;
	GDomNode	*last_child;
};

struct _GDomNodeClass {
	GObjectClass parent_class;

	/* DOM node virtuals */

	const char* 		(*get_node_name) 	(GDomNode* self);
	const char* 		(*get_node_value) 	(GDomNode* self);
	void 			(*set_node_value) 	(GDomNode* self, const char* new_value);
	GDomNodeType 		(*get_node_type) 	(GDomNode* self);
#if 0
	GDomNodeList* 		(*get_child_nodes) (GDomNode* self);
	GDomNamedNodeMap* 	(*get_attributes) (GDomNode* self);
#endif

	/* Validation virtuals */

	gboolean		(*can_append_child) 	(GDomNode *self, GDomNode *new_child);

	/* Implementation virtuals */

	void			(*post_new_child) 	(GDomNode *parent, GDomNode *child);
	void			(*pre_remove_child) 	(GDomNode *parent, GDomNode *child);
	void			(*changed)		(GDomNode *self);
	gboolean		(*child_changed)	(GDomNode *self, GDomNode *child);
};

GType gdom_node_get_type (void);

const char* 		gdom_node_get_node_name 	(GDomNode* self);
const char* 		gdom_node_get_node_value 	(GDomNode* self);
void 			gdom_node_set_node_value 	(GDomNode* self, const char* new_value);
GDomNodeType 		gdom_node_get_node_type 	(GDomNode* self);
GDomNode* 		gdom_node_get_parent_node 	(GDomNode* self);
#if 0
GDomNodeList* 		gdom_node_get_child_nodes 	(GDomNode* self);
#endif
GDomNode* 		gdom_node_get_first_child 	(GDomNode* self);
GDomNode* 		gdom_node_get_last_child 	(GDomNode* self);
GDomNode* 		gdom_node_get_previous_sibling 	(GDomNode* self);
GDomNode* 		gdom_node_get_next_sibling 	(GDomNode* self);
#if 0
GDomNamedNodeMap* 	gdom_node_get_attributes 	(GDomNode* self);
#endif
GDomNode* 		gdom_node_insert_before		(GDomNode* self, GDomNode* new_child, GDomNode* ref_child);
GDomNode* 		gdom_node_replace_child 	(GDomNode* self, GDomNode* new_child, GDomNode* old_child);
GDomNode* 		gdom_node_append_child 		(GDomNode* self, GDomNode* new_child);
GDomNode* 		gdom_node_remove_child 		(GDomNode* self, GDomNode* old_child);
gboolean 		gdom_node_has_child_nodes 	(GDomNode* self);

void 			gdom_node_changed 		(GDomNode *self);

GDomDocument* 		gdom_node_get_owner_document 	(GDomNode* self);

void 			gdom_node_dump 			(GDomNode *self);

G_END_DECLS

#endif
