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
	ptrdiff_t attr_offset;
	void (*finalize) (void *);
} GDomAttributeInfos;

GDomAttributeMap *
gdom_attribute_map_new (void)
{
	GDomAttributeMap *map;

	map = g_new0 (GDomAttributeMap, 1);
	g_return_val_if_fail (map != NULL,  NULL);

	map->hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);

	return map;
}

void
gdom_attribute_map_free (GDomAttributeMap *map)
{
	g_return_if_fail (map != NULL);

	g_hash_table_unref (map->hash);
	g_free (map);
}

void
gdom_attribute_map_add_attribute_full (GDomAttributeMap *map,
					  const char *attr_name,
					  ptrdiff_t attr_offset,
					  GDomAttributeFinalizeFunc finalize)
{
	GDomAttributeInfos *attr_infos;

	g_return_if_fail (map != NULL);
	g_return_if_fail (attr_name != NULL);
	g_return_if_fail (attr_offset >= 0);

	if (g_hash_table_lookup (map->hash, attr_name) != NULL) {
		g_warning ("[GDomAttributes::add_attribute] %s already defined", attr_name);
		return;
	}

	attr_infos = g_new (GDomAttributeInfos, 1);
	attr_infos->attr_offset = attr_offset;
	attr_infos->finalize = finalize;

	g_hash_table_insert (map->hash, (char *) attr_name, attr_infos);
}

void
gdom_attribute_map_add_attribute (GDomAttributeMap *map,
				     const char *attr_name,
				     ptrdiff_t attr_offset)
{
	gdom_attribute_map_add_attribute_full (map, attr_name, attr_offset, NULL);
}

gboolean
gdom_attribute_map_set_attribute (GDomAttributeMap *map,
				     void *instance,
				     const char *attr_name,
				     const char *attr_value)
{
	GDomAttributeInfos *attr_infos;
	GDomAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	g_free (attribute->value);
	attribute->value = attr_value != NULL ? g_strdup (attr_value) : NULL;

	return TRUE;
}

char const *
gdom_attribute_map_get_attribute (GDomAttributeMap *map,
				     void *instance,
				     const char *attr_name)
{
	GDomAttributeInfos *attr_infos;
	GDomAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, NULL);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return NULL;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, NULL);

	return attribute->value;
}

gboolean
gdom_attribute_map_is_attribute_defined (GDomAttributeMap *map,
					    void *instance,
					    const char *attr_name)
{
	GDomAttributeInfos *attr_infos;
	GDomAttributeValue *attribute;

	g_return_val_if_fail (map != NULL, FALSE);

	attr_infos = g_hash_table_lookup (map->hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	attribute = (void *)(instance + attr_infos->attr_offset);
	g_return_val_if_fail (attribute != NULL, FALSE);

	return attribute->value != NULL;
}

static void
gdom_attribute_finalize_cb (gpointer key,
			       gpointer value,
			       gpointer instance)
{
	GDomAttributeInfos *attr_infos = value;
	GDomAttributeValue *attribute;

	attribute = (void *)(instance + attr_infos->attr_offset);
	if (attribute != NULL) {
		g_free (attribute->value);
		g_free (attribute->css_value);

		if (attr_infos->finalize != NULL)
			attr_infos->finalize (attribute);
	}
}

void
gdom_attribute_map_free_attributes (GDomAttributeMap *map, void *instance)
{
	g_return_if_fail (map != NULL);

	g_hash_table_foreach (map->hash, gdom_attribute_finalize_cb, instance);
}

char const *
gdom_attribute_value_get_actual_value (const GDomAttributeValue *attribute)
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

	string = gdom_attribute_value_get_actual_value ((GDomAttributeValue *) attribute);
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

