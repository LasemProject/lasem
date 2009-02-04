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

#include <gdomattributes.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	ptrdiff_t bag_offset;
	const GDomAttributeBagClass *bag_class;
} GDomAttributeBagInfos;

typedef struct {
	ptrdiff_t attribute_offset;
	void *default_value;
	const GDomAttributeClass *attribute_class;
	GDomAttributeBagInfos *bag_infos;
} GDomAttributeInfos;

GDomAttributeMap *
gdom_attribute_map_new (void)
{
	GDomAttributeMap *map;

	map = g_new0 (GDomAttributeMap, 1);
	g_return_val_if_fail (map != NULL,  NULL);

	map->attribute_hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
	map->bag_hash = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, NULL);

	return map;
}

GDomAttributeMap *
gdom_attribute_map_duplicate (const GDomAttributeMap *from)
{
	GDomAttributeMap *map;
	GHashTableIter iter;
	void *key;
	void *value;

	map = gdom_attribute_map_new ();
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
gdom_attribute_map_free (GDomAttributeMap *map)
{
	g_return_if_fail (map != NULL);

	g_hash_table_unref (map->attribute_hash);
	g_hash_table_unref (map->bag_hash);
	g_free (map);
}

void
gdom_attribute_map_add_bag_attribute  (GDomAttributeMap *map,
				       const char *name,
				       ptrdiff_t attribute_offset,
				       void *default_value,
				       const GDomAttributeClass *attribute_class,
				       ptrdiff_t bag_offset,
				       const GDomAttributeBagClass *bag_class)
{
	GDomAttributeInfos *attribute_infos;
	GDomAttributeBagInfos *bag_infos;

	g_return_if_fail (map != NULL);
	g_return_if_fail (name != NULL);
	g_return_if_fail (attribute_offset >= 0);

	if (g_hash_table_lookup (map->attribute_hash, name) != NULL) {
		g_warning ("[GDomAttributeMap::add_attribute] %s already defined", name);
		return;
	}

	attribute_infos = g_new (GDomAttributeInfos, 1);
	attribute_infos->attribute_offset = attribute_offset;
	attribute_infos->attribute_class = attribute_class;

	g_hash_table_insert (map->attribute_hash, (char *) name, attribute_infos);

	if (bag_class != NULL) {
		bag_infos = g_hash_table_lookup (map->bag_hash, bag_class);
		if (bag_infos == NULL) {
			bag_infos = g_new (GDomAttributeBagInfos, 1);
			bag_infos->bag_offset = bag_offset;
			bag_infos->bag_class = bag_class;

			g_hash_table_insert (map->bag_hash, (void *) bag_class, bag_infos);
		}
		attribute_infos->bag_infos = bag_infos;
	} else
		attribute_infos->bag_infos = NULL;

}

void
gdom_attribute_map_add_attribute_full (GDomAttributeMap *map,
				       const char *name,
				       ptrdiff_t offset,
				       const GDomAttributeClass *attribute_class)
{
	gdom_attribute_map_add_bag_attribute (map, name, offset, 0, attribute_class, 0, NULL);
}

void
gdom_attribute_map_add_attribute_full_width_default (GDomAttributeMap *map,
						     const char *name,
						     ptrdiff_t offset,
						     void *default_value,
						     const GDomAttributeClass *attribute_class)
{
	gdom_attribute_map_add_bag_attribute (map, name, offset, 0, attribute_class, 0, NULL);
}

void
gdom_attribute_map_add_attribute (GDomAttributeMap *map,
				  const char *name,
				  ptrdiff_t offset)
{
	gdom_attribute_map_add_bag_attribute (map, name, offset, 0, NULL, 0, NULL);
}

gboolean
gdom_attribute_map_set_attribute (GDomAttributeMap *map,
				  void *instance,
				  const char *name,
				  const char *value)
{
	GDomAttributeInfos *attribute_infos;
	GDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return FALSE;

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		GDomAttributeBag **bag_ptr;

		bag_ptr = (void *)(instance + attribute_infos->bag_infos->bag_offset);
		g_return_val_if_fail (bag_ptr != NULL, FALSE);
		if (*bag_ptr == NULL)
			*bag_ptr = attribute_infos->bag_infos->bag_class->init ();
		g_return_val_if_fail (*bag_ptr != NULL, FALSE);
		attribute = (void *)(*bag_ptr + attribute_infos->attribute_offset);
	}

	g_return_val_if_fail (attribute != NULL, FALSE);

	g_free (attribute->value);
	attribute->value = value != NULL ? g_strdup (value) : NULL;

	return TRUE;
}

char const *
gdom_attribute_map_get_attribute (GDomAttributeMap *map,
				  void *instance,
				  const char *name)
{
	GDomAttributeInfos *attribute_infos;
	GDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, NULL);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return NULL;

	if (attribute_infos->bag_infos == NULL)
		attribute = (void *)(instance + attribute_infos->attribute_offset);
	else {
		GDomAttributeBag **bag_ptr;

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
gdom_attribute_map_is_attribute_defined (GDomAttributeMap *map,
					 void *instance,
					 const char *name)
{
	GDomAttributeInfos *attribute_infos;
	GDomAttribute *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attribute_infos = g_hash_table_lookup (map->attribute_hash, name);
	if (attribute_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attribute_infos->attribute_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

static void
gdom_attribute_finalize_cb (gpointer key,
			    gpointer value,
			    gpointer instance)
{
	GDomAttributeInfos *attribute_infos = value;
	GDomAttribute *attribute;

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
gdom_attribute_bag_finalize_cb (gpointer key,
				gpointer value,
				gpointer instance)
{
	GDomAttributeBagInfos *bag_infos = value;
	GDomAttributeBag **bag_ptr;

	bag_ptr = (void *)(instance + bag_infos->bag_offset);
	if (*bag_ptr != NULL) {
		if (bag_infos->bag_class->finalize != NULL)
			bag_infos->bag_class->finalize (*bag_ptr);

		*bag_ptr = NULL;
	}
}

void
gdom_attribute_map_free_attributes (GDomAttributeMap *map, void *instance)
{
	g_return_if_fail (map != NULL);

	g_hash_table_foreach (map->attribute_hash, gdom_attribute_finalize_cb, instance);
	g_hash_table_foreach (map->bag_hash, gdom_attribute_bag_finalize_cb, instance);
}

char const *
gdom_attribute_get_value (const GDomAttribute *attribute)
{
	g_return_val_if_fail (attribute != NULL, NULL);

	if (attribute->css_value != NULL &&
	    attribute->css_type >= GDOM_CSS_TYPE_AUTHOR)
		return attribute->css_value;

	return attribute->value;
}

void
gdom_attribute_boolean_parse (GDomAttributeBoolean *attribute,
			      gboolean *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = (strcmp (string, "true") == 0);
	*style_value = attribute->value;
}

void
gdom_attribute_unsigned_parse (GDomAttributeUnsigned *attribute,
			       unsigned int *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atoi (string);
	*style_value = attribute->value;
}

void
gdom_attribute_double_parse (GDomAttributeDouble *attribute,
			     double *style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = atof (string);
	*style_value = attribute->value;
}

void
gdom_attribute_string_parse (GDomAttributeString *attribute,
			     char **style_value)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);
	g_return_if_fail (*style_value != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
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
gdom_attribute_named_parse (GDomAttributeNamed *attribute,
			    unsigned int *style_value,
			    GDomNamedConvert convert)
{
	const char *string;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
	if (string == NULL) {
		attribute->value = *style_value;
		return;
	}

	attribute->value = convert (string);
	*style_value = attribute->value;
}

void
gdom_attribute_named_list_parse (GDomAttributeNamedList *attribute,
				 GDomNamedList *style_value,
				 GDomNamedConvert convert)
{
	const char *string;
	char **items;
	unsigned int i;

	g_return_if_fail (attribute != NULL);
	g_return_if_fail (style_value != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;

	string = gdom_attribute_get_value ((GDomAttribute *) attribute);
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
gdom_attribute_string_finalize (void *abstract)
{
	GDomAttributeString *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->value);
	attribute->value = NULL;
}

void
gdom_attribute_named_list_finalize (void *abstract)
{
	GDomAttributeNamedList *attribute = abstract;

	g_return_if_fail (attribute != NULL);

	g_free (attribute->values);
	attribute->n_values = 0;
	attribute->values = NULL;
}

static const GDomAttributeClass attribute_string_class = {
	.finalize = gdom_attribute_string_finalize
};

void
gdom_attribute_map_add_string (GDomAttributeMap *map,
			       char const *name,
			       ptrdiff_t offset)
{
	gdom_attribute_map_add_attribute_full (map, name, offset, &attribute_string_class);
}

static const GDomAttributeClass attribute_named_list_class = {
	.finalize = gdom_attribute_named_list_finalize
};

void
gdom_attribute_map_add_named_list (GDomAttributeMap *map,
				   char const *name,
				   ptrdiff_t offset)
{
	gdom_attribute_map_add_attribute_full (map, name, offset, &attribute_named_list_class);
}

