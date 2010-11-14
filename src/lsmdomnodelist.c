/* Lasem - SVG and Mathml library
 *
 * Copyright © 2010 Emmanuel Pacaud
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

#include <lsmdomnodelist.h>
#include <lsmdomnode.h>

static GObjectClass *parent_class;

/* LsmDomNodeList implementation */

static void
lsm_dom_node_list_weak_notify_cb (void *user_data, GObject *object)
{
	LsmDomNodeList *list = user_data;

	list->parent_node = NULL;
}

LsmDomNodeList *
lsm_dom_node_list_new (LsmDomNode *parent_node)
{
	LsmDomNodeList *list;

	g_return_val_if_fail (LSM_IS_DOM_NODE (parent_node), NULL);

	list = g_object_new (LSM_TYPE_DOM_NODE_LIST, NULL);
	list->parent_node = parent_node;

	g_object_weak_ref (G_OBJECT (parent_node), lsm_dom_node_list_weak_notify_cb, list);

	return list;
}

LsmDomNode *
lsm_dom_node_list_get_item (LsmDomNodeList *list, unsigned int index)
{
	LsmDomNode *iter;
	unsigned int i = 0;

	g_return_val_if_fail (LSM_IS_DOM_NODE_LIST (list), NULL);

	if (list->parent_node == NULL)
		return NULL;

	for (iter = list->parent_node->first_child; iter != NULL; iter = iter->next_sibling) {
		if (i == index)
			return iter;
		i++;
	}

	return NULL;
}

unsigned int
lsm_dom_node_list_get_length (LsmDomNodeList *list)
{
	LsmDomNode *iter;
	unsigned int length = 0;

	g_return_val_if_fail (LSM_IS_DOM_NODE_LIST (list), 0);

	if (list->parent_node == NULL)
		return 0;

	for (iter = list->parent_node->first_child; iter != NULL; iter = iter->next_sibling)
		length++;

	return length;
}

static void
lsm_dom_node_list_init (LsmDomNodeList *list)
{
}

static void
lsm_dom_node_list_finalize (GObject *object)
{
	LsmDomNodeList *list = LSM_DOM_NODE_LIST (object);

	if (list->parent_node != NULL) {
		g_object_weak_unref (G_OBJECT (list->parent_node), lsm_dom_node_list_weak_notify_cb, list);
		list->parent_node = NULL;
	}

	parent_class->finalize (object);
}

/* LsmDomNodeList class */

static void
lsm_dom_node_list_class_init (LsmDomNodeListClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = lsm_dom_node_list_finalize;
}

G_DEFINE_TYPE (LsmDomNodeList, lsm_dom_node_list, G_TYPE_OBJECT)
