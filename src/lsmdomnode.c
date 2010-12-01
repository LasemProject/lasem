/* Lasem - SVG and Mathml library
 *
 * Copyright © 2007-2010 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

/**
 * SECTION:lsmdomnode
 * @short_description: Base class for DOM nodes
 */

#include <lsmdomnode.h>
#include <lsmdomnodelist.h>
#include <lsmdomdocument.h>
#include <lsmdebug.h>
#include <glib/gprintf.h>
#include <stdio.h>

/* LsmDomNodeChildList */


#define LSM_TYPE_DOM_NODE_CHILD_LIST             (lsm_dom_node_child_list_get_type ())
#define LSM_DOM_NODE_CHILD_LIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_NODE_CHILD_LIST, LsmDomNodeChildList))
#define LSM_DOM_NODE_CHILD_LIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_NODE_CHILD_LIST, LsmDomNodeChildListClass))
#define LSM_IS_DOM_NODE_CHILD_LIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_NODE_CHILD_LIST))
#define LSM_IS_DOM_NODE_CHILD_LIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_NODE_CHILD_LIST))
#define LSM_DOM_NODE_CHILD_LIST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_NODE_CHILD_LIST, LsmDomNodeChildListClass))

typedef struct _LsmDomNodeChildListClass LsmDomNodeChildListClass;

typedef struct {
	LsmDomNodeList base;

	LsmDomNode *parent_node;
} LsmDomNodeChildList;

struct _LsmDomNodeChildListClass {
	LsmDomNodeListClass parent_class;
};

GType lsm_dom_node_child_list_get_type (void);

static GObjectClass *child_list_parent_class = NULL;

static void
lsm_dom_node_child_list_weak_notify_cb (void *user_data, GObject *object)
{
	LsmDomNodeChildList *list = user_data;

	list->parent_node = NULL;
}

static LsmDomNode *
lsm_dom_node_child_list_get_item (LsmDomNodeList *list, unsigned int index)
{
	LsmDomNodeChildList *child_list = LSM_DOM_NODE_CHILD_LIST (list);
	LsmDomNode *iter;
	unsigned int i = 0;

	if (child_list->parent_node == NULL)
		return NULL;

	for (iter = child_list->parent_node->first_child; iter != NULL; iter = iter->next_sibling) {
		if (i == index)
			return iter;
		i++;
	}

	return NULL;
}

static unsigned int
lsm_dom_node_child_list_get_length (LsmDomNodeList *list)
{
	LsmDomNodeChildList *child_list = LSM_DOM_NODE_CHILD_LIST (list);
	LsmDomNode *iter;
	unsigned int length = 0;

	if (child_list->parent_node == NULL)
		return 0;

	for (iter = child_list->parent_node->first_child; iter != NULL; iter = iter->next_sibling)
		length++;

	return length;
}

LsmDomNodeList *
lsm_dom_node_child_list_new (LsmDomNode *parent_node)
{
	LsmDomNodeChildList *list;

	g_return_val_if_fail (LSM_IS_DOM_NODE (parent_node), NULL);

	list = g_object_new (LSM_TYPE_DOM_NODE_CHILD_LIST, NULL);
	list->parent_node = parent_node;

	g_object_weak_ref (G_OBJECT (parent_node), lsm_dom_node_child_list_weak_notify_cb, list);

	return LSM_DOM_NODE_LIST (list);
}

static void
lsm_dom_node_child_list_init (LsmDomNodeChildList *list)
{
}

static void
lsm_dom_node_child_list_finalize (GObject *object)
{
	LsmDomNodeChildList *list = LSM_DOM_NODE_CHILD_LIST (object);

	if (list->parent_node != NULL) {
		g_object_weak_unref (G_OBJECT (list->parent_node), lsm_dom_node_child_list_weak_notify_cb, list);
		list->parent_node = NULL;
	}

	child_list_parent_class->finalize (object);
}

static void
lsm_dom_node_child_list_class_init (LsmDomNodeChildListClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	LsmDomNodeListClass *node_list_class = LSM_DOM_NODE_LIST_CLASS (klass);

	child_list_parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_dom_node_child_list_finalize;

	node_list_class->get_item = lsm_dom_node_child_list_get_item;
	node_list_class->get_length = lsm_dom_node_child_list_get_length;
}

G_DEFINE_TYPE (LsmDomNodeChildList, lsm_dom_node_child_list, LSM_TYPE_DOM_NODE_LIST)

static GObjectClass *parent_class = NULL;

/* LsmDomNode implementation */


/**
 * lsm_dom_node_get_node_name:
 * @self: a #LsmDomNode
 * Return value: the node name.
 *
 * Gets the node name.
 */

const char*
lsm_dom_node_get_node_name (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, NULL);

	if (node_class->get_node_name)
		return node_class->get_node_name (self);

	return NULL;
}

/**
 * lsm_dom_node_get_node_value:
 * @self: a #LsmDomNode
 * Return value: the node value.
 *
 * Gets the node value.
 */

const char*
lsm_dom_node_get_node_value (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, NULL);

	if (node_class->get_node_value)
		return node_class->get_node_value (self);

	return NULL;
}

void
lsm_dom_node_set_node_value (LsmDomNode* self, const char* new_value)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_if_fail (node_class != NULL);
	g_return_if_fail (new_value != NULL);

	if (node_class->set_node_value)
		node_class->set_node_value (self, new_value);
}

LsmDomNodeType lsm_dom_node_get_node_type (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, 0);

	if (node_class->get_node_type)
		return node_class->get_node_type (self);

	return 0;
}

LsmDomNode*
lsm_dom_node_get_parent_node (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->parent_node;
}

LsmDomNodeList*
lsm_dom_node_get_child_nodes (LsmDomNode* self)
{
	LsmDomNodeList *list;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	list = g_object_get_data (G_OBJECT (self), "child-nodes");

	if (list == NULL) {
		list = lsm_dom_node_child_list_new (self);
		g_object_set_data_full (G_OBJECT (self), "child-nodes", list, g_object_unref);
	}

	return list;
}

LsmDomNode*
lsm_dom_node_get_first_child (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->first_child;
}

LsmDomNode*
lsm_dom_node_get_last_child (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->last_child;
}

LsmDomNode*
lsm_dom_node_get_previous_sibling (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->previous_sibling;
}

LsmDomNode*
lsm_dom_node_get_next_sibling (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->next_sibling;
}

/*LsmDomNamedNodeMap**/
/*lsm_dom_node_get_attributes (LsmDomNode* self)*/
/*{*/
/*        return LSM_DOM_NODE_GET_CLASS (self)->get_attributes (self);*/
/*}*/

LsmDomDocument*
lsm_dom_node_get_owner_document (LsmDomNode* self)
{
	LsmDomNode *parent;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	for (parent = self;
	     parent != NULL &&
	     !LSM_IS_DOM_DOCUMENT (parent);
	     parent = parent->parent_node);

	return LSM_DOM_DOCUMENT (parent);
}

LsmDomNode*
lsm_dom_node_insert_before (LsmDomNode* self, LsmDomNode* new_child, LsmDomNode* ref_child)
{
	LsmDomNodeClass *node_class;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (new_child), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (ref_child), NULL);

	if (ref_child->parent_node != self) {
		lsm_debug ("dom", "[LsmDomNode::insert_before] Ref child '%s' doesn't belong to '%s'",
			   lsm_dom_node_get_node_name (ref_child),
			   lsm_dom_node_get_node_name (self));
		return NULL;
	}

	if (!LSM_DOM_NODE_GET_CLASS (self)->can_append_child (self, new_child)) {
		lsm_debug ("dom", "[LsmDomNode::insert_before] Can't append '%s' to '%s'",
			   lsm_dom_node_get_node_name (new_child),
			   lsm_dom_node_get_node_name (self));
		return NULL;
	}

	new_child->parent_node = self;
	new_child->next_sibling = ref_child;
	new_child->previous_sibling = ref_child->previous_sibling;

	if (ref_child->previous_sibling == NULL)
		self->first_child = new_child;
	else
		ref_child->previous_sibling->next_sibling = new_child;

	ref_child->previous_sibling = new_child;

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->post_new_child)
		node_class->post_new_child (self, new_child);

	lsm_dom_node_changed (self);

	return new_child;
}

LsmDomNode*
lsm_dom_node_replace_child (LsmDomNode* self, LsmDomNode* new_child, LsmDomNode* old_child)
{
	LsmDomNode *next_sibling;
	LsmDomNode *node;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (old_child), NULL);

	if (old_child->parent_node != self)
		return NULL;

	next_sibling = old_child->next_sibling;

	node = lsm_dom_node_remove_child (self, old_child);
	if (node != old_child)
		return NULL;

	if (new_child == NULL)
		return NULL;

	g_return_val_if_fail (LSM_IS_DOM_NODE (new_child), NULL);

	if (next_sibling == NULL)
		lsm_dom_node_append_child (self, new_child);
	else
		lsm_dom_node_insert_before (self, new_child, next_sibling);

	return old_child;
}

LsmDomNode*
lsm_dom_node_remove_child (LsmDomNode* self, LsmDomNode* old_child)
{
	LsmDomNode *node;
	LsmDomNodeClass *node_class;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (old_child), NULL);

	for (node = self->first_child;
	     node != NULL && node != old_child;
	     node = node->next_sibling);

	if (node == NULL)
		return NULL;

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->pre_remove_child)
		node_class->pre_remove_child (self, old_child);

	if (self->first_child == old_child)
		self->first_child = old_child->next_sibling;
	if (self->last_child == old_child)
		self->last_child = old_child->previous_sibling;

	if (old_child->next_sibling != NULL)
		old_child->next_sibling->previous_sibling = old_child->previous_sibling;
	if (old_child->previous_sibling != NULL)
		old_child->previous_sibling->next_sibling = old_child->next_sibling;

	old_child->parent_node = NULL;
	old_child->next_sibling = NULL;
	old_child->previous_sibling = NULL;

	lsm_dom_node_changed (self);

	return old_child;
}

LsmDomNode *
lsm_dom_node_append_child (LsmDomNode* self, LsmDomNode* new_child)
{
	LsmDomNodeClass *node_class;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (new_child), NULL);
	g_return_val_if_fail (new_child->parent_node == NULL, NULL);

	if (!LSM_DOM_NODE_GET_CLASS (self)->can_append_child (self, new_child)) {
		lsm_debug ("dom", "[LsmDomNode::append_child] Can't append '%s' to '%s'",
			   lsm_dom_node_get_node_name (new_child),
			   lsm_dom_node_get_node_name (self));
		return NULL;
	}

	if (self->first_child == NULL)
		self->first_child = new_child;
	if (self->last_child != NULL)
		self->last_child->next_sibling = new_child;

	new_child->parent_node = self;
	new_child->next_sibling = NULL;
	new_child->previous_sibling = self->last_child;
	self->last_child = new_child;

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->post_new_child)
		node_class->post_new_child (self, new_child);

	lsm_dom_node_changed (self);

	return new_child;
}

static gboolean
lsm_dom_node_can_append_child_default (LsmDomNode *self, LsmDomNode* new_child)
{
	return FALSE;
}

void
lsm_dom_node_changed (LsmDomNode *self)
{
	LsmDomNode *parent_node;
	LsmDomNode *child_node;
	LsmDomNodeClass *node_class;

	g_return_if_fail (LSM_IS_DOM_NODE (self));

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->changed)
		node_class->changed (self);

	child_node = self;
	for (parent_node = self->parent_node;
	       parent_node != NULL;
	       parent_node = parent_node->parent_node) {
		node_class = LSM_DOM_NODE_GET_CLASS (parent_node);
		if (node_class->child_changed == NULL ||
		    !node_class->child_changed (parent_node, child_node))
			break;
		child_node = parent_node;
	}
}

gboolean
lsm_dom_node_has_child_nodes (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), FALSE);

	return self->first_child != NULL;
}

static void
lsm_dom_node_write_to_stream_default (LsmDomNode *self, GOutputStream *stream, GError **error)
{
	LsmDomNode *child;

	for (child = self->first_child; child != NULL; child = child->next_sibling)
		lsm_dom_node_write_to_stream (child, stream, error);
}

void
lsm_dom_node_write_to_stream (LsmDomNode *self, GOutputStream *stream, GError **error)
{
	LsmDomNodeClass *node_class;

	g_return_if_fail (LSM_IS_DOM_NODE (self));
	g_return_if_fail (G_IS_OUTPUT_STREAM (stream));

	node_class = LSM_DOM_NODE_GET_CLASS (self);
	if (node_class->write_to_stream != NULL)
		node_class->write_to_stream (self, stream, error);
}

static void
lsm_dom_node_init (LsmDomNode *node)
{
}

static void
lsm_dom_node_finalize (GObject *object)
{
	LsmDomNode *node = LSM_DOM_NODE (object);
	LsmDomNode *child, *next_child;

	child = node->first_child;
	while (child != NULL) {
		next_child = child->next_sibling;
		g_object_unref (child);
		child = next_child;
	}

	parent_class->finalize (object);
}

/* LsmDomNode class */

static void
lsm_dom_node_class_init (LsmDomNodeClass *node_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (node_class);

	parent_class = g_type_class_peek_parent (node_class);

	object_class->finalize = lsm_dom_node_finalize;

	node_class->can_append_child = lsm_dom_node_can_append_child_default;
	node_class->write_to_stream = lsm_dom_node_write_to_stream_default;
}

G_DEFINE_ABSTRACT_TYPE (LsmDomNode, lsm_dom_node, G_TYPE_OBJECT)
