/*
 * Copyright © 2007-2009  Emmanuel Pacaud
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

#include <lsmattributes.h>
#include <lsmdebug.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

gboolean
lsm_attribute_is_defined (const LsmAttribute *attribute)
{
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

#define ATTRIBUTE_TRAIT(attribute) ((void *) (((void *) attribute) + sizeof (LsmAttribute)))

struct _LsmAttributeManager {
	GHashTable *			hash_by_name;
};

static LsmAttributeManager *
lsm_attribute_manager_create (void)
{
	LsmAttributeManager *manager;

	manager = g_new0 (LsmAttributeManager, 1);
	manager->hash_by_name = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);

	return manager;
}

LsmAttributeManager *
lsm_attribute_manager_new (unsigned int n_attributes, const LsmAttributeInfos *attribute_infos)
{
	LsmAttributeManager *manager;

	manager = lsm_attribute_manager_create ();

	lsm_attribute_manager_add_attributes (manager, n_attributes, attribute_infos);

	return manager;
}

LsmAttributeManager *
lsm_attribute_manager_duplicate (const LsmAttributeManager *origin)
{
	LsmAttributeManager *manager;
	GHashTableIter iter;
	gpointer key, value;

	g_return_val_if_fail (origin != NULL, NULL);

	manager = lsm_attribute_manager_create ();

	g_hash_table_iter_init (&iter, origin->hash_by_name);
	while (g_hash_table_iter_next (&iter, &key, &value))
		g_hash_table_insert (manager->hash_by_name, key, value);

	return manager;
}

void
lsm_attribute_manager_add_attributes (LsmAttributeManager *manager,
				      unsigned int n_attributes,
				      const LsmAttributeInfos *attribute_infos)
{
	unsigned int i;

	g_return_if_fail (n_attributes > 0);
	g_return_if_fail (attribute_infos != NULL);

	for (i = 0; i < n_attributes; i++) {
		g_assert (attribute_infos[i].name != NULL);
		g_assert (attribute_infos[i].attribute_offset >= 0);
		g_assert (attribute_infos[i].trait_class != NULL);

		g_hash_table_insert (manager->hash_by_name,
				     (void *) attribute_infos[i].name,
				     (void *) &attribute_infos[i]);
	}

}

void
lsm_attribute_manager_free (LsmAttributeManager *manager)
{
	g_return_if_fail (manager != NULL);

	g_hash_table_unref (manager->hash_by_name);
	g_free (manager);
}

gboolean
lsm_attribute_manager_set_attribute (LsmAttributeManager *manager,
				     void *instance,
				     const char *name,
				     const char *value)
{
	LsmAttribute *attribute;
	LsmAttributeInfos *attribute_infos;
	const LsmTraitClass *trait_class;

	g_return_val_if_fail (manager != NULL, FALSE);

	attribute_infos = g_hash_table_lookup (manager->hash_by_name, name);
	if (attribute_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	trait_class = attribute_infos->trait_class;

	g_free (attribute->value);
	attribute->value = g_strdup (value);

	if (attribute->value != NULL) {
		if (trait_class->from_string)
			trait_class->from_string (ATTRIBUTE_TRAIT (attribute), (char *) value);
	} else {
		if (trait_class->init)
			trait_class->init (ATTRIBUTE_TRAIT (attribute), attribute_infos->trait_default);
		else
			/* Simple memcpy for default init implementation, discarde by a NULL default value. */
			if (attribute_infos->trait_default != NULL)
				memcpy (ATTRIBUTE_TRAIT (attribute),
					attribute_infos->trait_default,
					trait_class->size);
	}

	return TRUE;
}

char const *
lsm_attribute_manager_get_attribute (LsmAttributeManager *manager,
				     void *instance,
				     const char *name)
{
	LsmAttributeInfos *attribute_infos;
	LsmAttribute *attribute;

	g_return_val_if_fail (manager != NULL, NULL);

	attribute_infos = g_hash_table_lookup (manager->hash_by_name, name);
	if (attribute_infos == NULL)
		return NULL;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute->value;
}

void
lsm_attribute_manager_clean_attributes (LsmAttributeManager *manager,
					void *instance)
{
	LsmAttributeInfos *attribute_infos;
	LsmAttribute *attribute;
	const LsmTraitClass *trait_class;
	GHashTableIter iter;
	gpointer key, value;

	g_return_if_fail (manager != NULL);

	g_hash_table_iter_init (&iter, manager->hash_by_name);
	while (g_hash_table_iter_next (&iter, &key, &value)) {
		attribute_infos = value;
		trait_class = attribute_infos->trait_class;

		if (trait_class->finalize) {
			attribute = (void *)(instance + attribute_infos->attribute_offset);
			trait_class->finalize (ATTRIBUTE_TRAIT (attribute));
		}
	}
}

char *
lsm_attribute_manager_serialize	(LsmAttributeManager *manager,
				 void *instance)
{
	LsmAttributeInfos *attribute_infos;
	LsmAttribute *attribute;
	GString *string;
	GHashTableIter iter;
	char *c_string;
	gpointer key, value;
	gboolean is_first = TRUE;

	g_return_val_if_fail (manager != NULL, NULL);

	string = g_string_new ("");

	g_hash_table_iter_init (&iter, manager->hash_by_name);
	while (g_hash_table_iter_next (&iter, &key, &value)) {
		attribute_infos = value;
		attribute = (void *)(instance + attribute_infos->attribute_offset);

		if (is_first) {
			g_string_append_printf (string, "%s=\"%s\"",
						attribute_infos->name,
						attribute->value);
			is_first = FALSE;
		} else {
			g_string_append_printf (string, " %s=\"%s\"",
						attribute_infos->name,
						attribute->value);
		}
	}

	c_string = string->str;

	g_string_free (string, FALSE);

	return c_string;
}
