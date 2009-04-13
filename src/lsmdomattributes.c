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

#include <lsmdomattributes.h>
#include <lsmdebug.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	ptrdiff_t bag_offset;
	const LsmDomAttributeBagClass *bag_class;
} LsmDomAttributeBagInfos;

typedef struct {
	ptrdiff_t attribute_offset;
	const LsmDomAttributeClass *attribute_class;
	LsmDomAttributeBagInfos *bag_infos;
} LsmDomAttributeInfos;

LsmDomAttributeMap *
lsm_dom_attribute_map_new (void)
{
	LsmDomAttributeMap *map;

	map = g_new0 (LsmDomAttributeMap, 1);
	g_return_val_if_fail (map != NULL,  NULL);

	map->attribute_hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
	map->bag_hash = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, NULL);

	return map;
}

LsmDomAttributeMap *
lsm_dom_attribute_map_duplicate (const LsmDomAttributeMap *from)
{
	LsmDomAttributeMap *map;
	GHashTableIter iter;
	void *key;
	void *value;

	map = lsm_dom_attribute_map_new ();
	g_return_val_if_fail (map != NULL, NULL);

	g_hash_table_iter_init (&iter, from->attribute_hash);
	while (g_hash_table_iter_next (&iter, &key, &value))
		g_hash_table_insert (map->attribute_hash, key, value);

	g_hash_table_iter_init (&iter, from->bag_hash);
	while (g_hash_table_iter_next (&iter, &key, &value))
		g_hash_table_insert (map->bag_hash, key, value);

	return map;
}

void
lsm_dom_attribute_map_free (LsmDomAttributeMap *map)
{
	g_return_if_fail (map != NULL);

	g_hash_table_unref (map->attribute_hash);
	g_hash_table_unref (map->bag_hash);
	g_free (map);
}

void
lsm_dom_attribute_map_add_bag_attribute  (LsmDomAttributeMap *map,
				       const char *name,
				       ptrdiff_t attribute_offset,
				       const LsmDomAttributeClass *attribute_class,
				       ptrdiff_t bag_offset,
				       const LsmDomAttributeBagClass *bag_class)
{
	LsmDomAttributeInfos *attribute_infos;
	LsmDomAttributeBagInfos *bag_infos;

	g_return_if_fail (map != NULL);
	g_return_if_fail (name != NULL);
	g_return_if_fail (attribute_offset >= 0);

	if (g_hash_table_lookup (map->attribute_hash, name) != NULL) {
		g_warning ("[LsmDomAttributeMap::add_attribute] %s already defined", name);
		return;
	}

	attribute_infos = g_new (LsmDomAttributeInfos, 1);
	attribute_infos->attribute_offset = attribute_offset;
	attribute_infos->attribute_class = attribute_class;

	lsm_debug ("name = %s - attribute_offset = %d", name, attribute_offset);

	g_hash_table_insert (map->attribute_hash, (char *) name, attribute_infos);

	if (bag_class != NULL) {
		bag_infos = g_hash_table_lookup (map->bag_hash, bag_class);
		if (bag_infos == NULL) {
			bag_infos = g_new (LsmDomAttributeBagInfos, 1);
			bag_infos->bag_offset = bag_offset;
			bag_infos->bag_class = bag_class;

			g_hash_table_insert (map->bag_hash, (void *) bag_class, bag_infos);
		}
		attribute_infos->bag_infos = bag_infos;
	} else
		attribute_infos->bag_infos = NULL;

}

void
lsm_dom_attribute_map_add_attribute_full (LsmDomAttributeMap *map,
					  const char *name,
					  ptrdiff_t offset,
					  const LsmDomAttributeClass *attribute_class)
{
	lsm_dom_attribute_map_add_bag_attribute (map, name, offset, attribute_class, 0, NULL);
}

void
lsm_dom_attribute_map_add_attribute_full_width_default (LsmDomAttributeMap *map,
						     const char *name,
						     ptrdiff_t offset,
						     const LsmDomAttributeClass *attribute_class)
{
	lsm_dom_attribute_map_add_bag_attribute (map, name, offset, attribute_class, 0, NULL);
}

void
lsm_dom_attribute_map_add_attribute (LsmDomAttributeMap *map,
				  const char *name,
				  ptrdiff_t offset)
{
	lsm_dom_attribute_map_add_bag_attribute (map, name, offset, NULL, 0, NULL);
}

static LsmDomAttribute *
_get_attribute (LsmDomAttributeMap *map,
		void *instance,
		const char *name)
{
	LsmDomAttributeInfos *attribute_infos;
	LsmDomAttribute *attribute;

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL) {
		lsm_debug ("[LsmDomAttribute] Attribute not found (%s)", name);
		return NULL;
	}

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		LsmDomAttributeBag **bag_ptr;

		bag_ptr = (void *)(instance + attribute_infos->bag_infos->bag_offset);
		g_return_val_if_fail (bag_ptr != NULL, FALSE);

		if (*bag_ptr == NULL)
			*bag_ptr = attribute_infos->bag_infos->bag_class->init ();
		g_return_val_if_fail (*bag_ptr != NULL, FALSE);

		attribute = (((void *) *bag_ptr) + attribute_infos->attribute_offset);
	}

	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute;
}

gboolean
lsm_dom_attribute_map_set_attribute (LsmDomAttributeMap *map,
				     void *instance,
				     const char *name,
				     const char *value)
{
	LsmDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute = _get_attribute (map, instance, name);
	if (attribute == NULL)
		return FALSE;

	g_free (attribute->value);
	attribute->value = value != NULL ? g_strdup (value) : NULL;

	return TRUE;
}

gboolean
lsm_dom_attribute_map_set_css_attribute (LsmDomAttributeMap *map,
				      void *instance,
				      const char *name,
				      const char *value,
				      LsmDomCssType type)
{
	LsmDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute = _get_attribute (map, instance, name);
	if (attribute == NULL)
		return FALSE;

	g_free (attribute->css_value);
	attribute->css_value = value != NULL ? g_strdup (value) : NULL;
	attribute->css_type = type;

	return TRUE;
}

char const *
lsm_dom_attribute_map_get_attribute (LsmDomAttributeMap *map,
				     void *instance,
				     const char *name)
{
	LsmDomAttributeInfos *attribute_infos;
	LsmDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, NULL);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return NULL;

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		LsmDomAttributeBag **bag_ptr;

		bag_ptr = (void *)(instance + attribute_infos->bag_infos->bag_offset);
		g_return_val_if_fail (bag_ptr != NULL, NULL);
		if (*bag_ptr == NULL)
			return NULL;
		attribute = (void *)(*bag_ptr + attribute_infos->attribute_offset);
	}
	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute->value;
}

gboolean
lsm_dom_attribute_map_is_attribute_defined (LsmDomAttributeMap *map,
					 void *instance,
					 const char *name)
{
	LsmDomAttributeInfos *attribute_infos;
	LsmDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

gboolean
lsm_dom_attribute_is_defined (const LsmDomAttribute *attribute)
{
	return (attribute != NULL && attribute->value != NULL);
}

static void
lsm_dom_attribute_finalize_cb (gpointer key,
			    gpointer value,
			    gpointer instance)
{
	LsmDomAttributeInfos *attribute_infos = value;
	LsmDomAttribute *attribute;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	if (attribute != NULL) {
		g_free (attribute->value);
		g_free (attribute->css_value);

		if (attribute_infos->attribute_class != NULL &&
		    attribute_infos->attribute_class->finalize != NULL)
			attribute_infos->attribute_class->finalize (attribute);
	}
}

static void
lsm_dom_attribute_bag_finalize_cb (gpointer key,
				gpointer value,
				gpointer instance)
{
	LsmDomAttributeBagInfos *bag_infos = value;
	LsmDomAttributeBag **bag_ptr;

	bag_ptr = (void *)(instance + bag_infos->bag_offset);
	if (*bag_ptr != NULL) {
		if (bag_infos->bag_class->finalize != NULL)
			bag_infos->bag_class->finalize (*bag_ptr);

		*bag_ptr = NULL;
	}
}

void
lsm_dom_attribute_map_free_attributes (LsmDomAttributeMap *map, void *instance)
{
	g_return_if_fail (map != NULL);

	g_hash_table_foreach (map->attribute_hash, lsm_dom_attribute_finalize_cb, instance);
	g_hash_table_foreach (map->bag_hash, lsm_dom_attribute_bag_finalize_cb, instance);
}

char const *
lsm_dom_attribute_get_value (const LsmDomAttribute *attribute)
{
	g_return_val_if_fail (attribute != NULL, NULL);

	if (attribute->css_value != NULL &&
	    attribute->css_type >= LSM_DOM_CSS_TYPE_AUTHOR)
		return attribute->css_value;

	return attribute->value;
}

void
lsm_dom_boolean_attribute_parse (LsmDomBooleanAttribute *attribute,
			      gboolean *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = (strcmp (string, "true") == 0);
	*style_value = attribute->value;
}

void
lsm_dom_unsigned_attribute_parse (LsmDomUnsignedAttribute *attribute,
			       unsigned int *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atoi (string);
	*style_value = attribute->value;
}

void
lsm_dom_double_attribute_parse (LsmDomDoubleAttribute *attribute,
			     double *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atof (string);
	*style_value = attribute->value;
}

void
lsm_dom_string_attribute_parse (LsmDomStringAttribute *attribute,
				char **style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);
	g_return_if_fail (*style_value != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		g_free (attribute->value);
		attribute->value = g_strdup (*style_value);
	} else {
		g_free (*style_value);
		*style_value = g_strdup (string);
		attribute->value = g_strdup (string);
	}
}

void
lsm_dom_enum_attribute_parse (LsmDomEnumAttribute *attribute,
			    unsigned int *style_value,
			    LsmDomNamedConvert convert)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = convert (string);
	*style_value = attribute->value;
}

void
lsm_dom_enum_list_attribute_parse (LsmDomEnumListAttribute *attribute,
				 LsmDomEnumList *style_value,
				 LsmDomNamedConvert convert)
{
	const char *string;
	char **items;
	unsigned int i;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;

	string = lsm_dom_attribute_get_value ((LsmDomAttribute *) attribute);
	if (string == NULL) {
		if (style_value->n_values > 0) {
			attribute->values = g_new (unsigned int, style_value->n_values);
			memcpy (attribute->values, style_value->values,
				sizeof (unsigned int) * style_value->n_values);
		} else
			attribute->values = NULL;
		attribute->n_values = style_value->n_values;

		return;
	}

	items = g_strsplit_set (string, " ", -1);
	attribute->n_values = g_strv_length (items);

	attribute->values = g_new (unsigned int, attribute->n_values);
	for (i = 0; i < attribute->n_values; i++)
		attribute->values[i] = convert (items[i]);

	g_strfreev (items);
}

void
lsm_dom_string_attribute_finalize (void *abstract)
{
	LsmDomStringAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->value);
	attribute->value = NULL;
}

void
lsm_dom_enum_list_attribute_finalize (void *abstract)
{
	LsmDomEnumListAttribute *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;
	attribute->values = NULL;
}

static const LsmDomAttributeClass string_attribute_class = {
	.finalize = lsm_dom_string_attribute_finalize
};

void
lsm_dom_attribute_map_add_string (LsmDomAttributeMap *map,
			       char const *name,
			       ptrdiff_t offset)
{
	lsm_dom_attribute_map_add_attribute_full (map, name, offset, &string_attribute_class);
}

static const LsmDomAttributeClass enum_list_attribute_class = {
	.finalize = lsm_dom_enum_list_attribute_finalize
};

void
lsm_dom_attribute_map_add_enum_list (LsmDomAttributeMap *map,
				   char const *name,
				   ptrdiff_t offset)
{
	lsm_dom_attribute_map_add_attribute_full (map, name, offset, &enum_list_attribute_class);
}

