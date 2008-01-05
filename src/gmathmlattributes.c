/* gmathmlattributes.c
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#include <gmathmlattributes.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gmem.h>
#include <glib/ghash.h>

typedef struct {
	const char * 	(*to_string)	(GMathmlAttribute *attr);
	void 		(*from_string)	(GMathmlAttribute *attr, const char *string);
	void 		(*finalize) 	(GMathmlAttribute *attr);
} GMathmlAttributeClass;

static const char *
gmathml_boolean_attribute_to_string (GMathmlAttribute *attr)
{
	GMathmlBooleanAttribute *bool_attr = (GMathmlBooleanAttribute *) attr;

	return (bool_attr->value ? "true" : "false");
}

static void
gmathml_boolean_attribute_from_string (GMathmlAttribute *attr, const char *string)
{
	GMathmlBooleanAttribute *bool_attr = (GMathmlBooleanAttribute *) attr;

	if (string != NULL &&
	    strcmp ("true", string) == 0)
		bool_attr->value = TRUE;
	else
		bool_attr->value = FALSE;
}

static const char *
gmathml_int_attribute_to_string (GMathmlAttribute *attr)
{
	GMathmlIntAttribute *int_attr = (GMathmlIntAttribute *) attr;

	if (int_attr->string == NULL)
		int_attr->string = g_strdup_printf ("%i", int_attr->value);

	return int_attr->string;
}

static void
gmathml_int_attribute_from_string (GMathmlAttribute *attr, const char *string)
{
	GMathmlIntAttribute *int_attr = (GMathmlIntAttribute *) attr;

	g_free (int_attr->string);
	int_attr->string = NULL;

	if (string != NULL)
		int_attr->value = atoi (string);
	else
		int_attr = 0;
}

static void
gmathml_int_attribute_finalize (GMathmlAttribute *attr)
{
	GMathmlIntAttribute *int_attr = (GMathmlIntAttribute *) attr;

	g_free (int_attr->string);
	int_attr->string = NULL;
}

static const char *
gmathml_string_attribute_to_string (GMathmlAttribute *attr)
{
	GMathmlStringAttribute *str_attr = (GMathmlStringAttribute *) attr;

	return str_attr->string;
}

static void
gmathml_string_attribute_from_string (GMathmlAttribute *attr, const char *string)
{
	GMathmlStringAttribute *str_attr = (GMathmlStringAttribute *) attr;

	g_free (str_attr->string);

	if (string != NULL)
		str_attr->string = g_strdup (string);
	else
		str_attr->string = NULL;
}

static void
gmathml_string_attribute_finalize (GMathmlAttribute *attr)
{
	GMathmlStringAttribute *str_attr = (GMathmlStringAttribute *) attr;

	g_free (str_attr->string);
	str_attr->string = NULL;
}

static const char *
gmathml_color_attribute_to_string (GMathmlAttribute *attr)
{
	GMathmlColorAttribute *color_attr = (GMathmlColorAttribute *) attr;

	if (color_attr->string != NULL) {
		color_attr->string = pango_color_to_string (&color_attr->color);
	}

	return color_attr->string;
}

static void
gmathml_color_attribute_from_string (GMathmlAttribute *attr, const char *string)
{
	GMathmlColorAttribute *color_attr = (GMathmlColorAttribute *) attr;

	g_free (color_attr->string);

	if (string != NULL) {
		color_attr->string = g_strdup (string);
		pango_color_parse (&color_attr->color, color_attr->string);
	} else
		color_attr->string = NULL;
}

static void
gmathml_color_attribute_finalize (GMathmlAttribute *attr)
{
	GMathmlColorAttribute *color_attr = (GMathmlColorAttribute *) attr;

	g_free (color_attr->string);
	color_attr->string = NULL;
}

inline static const char*
gmathml_attribute_to_string (GMathmlAttribute *attr, GMathmlAttributeClass *attr_class)
{
	return attr_class->to_string (attr);
}

inline static void
gmathml_attribute_from_string (GMathmlAttribute *attr, GMathmlAttributeClass *attr_class,
			       const char *string)
{
	attr_class->from_string (attr, string);
}

inline static void
gmathml_attribute_finalize (GMathmlAttribute *attr, GMathmlAttributeClass *attr_class)
{
	if (attr_class->finalize)
		attr_class->finalize (attr);
}

static const GMathmlAttributeClass gmathml_attribute_classes[] = {
	{
		gmathml_boolean_attribute_to_string,
		gmathml_boolean_attribute_from_string,
		NULL
	},
	{
		gmathml_int_attribute_to_string,
		gmathml_int_attribute_from_string,
		gmathml_int_attribute_finalize
	},
	{
		gmathml_string_attribute_to_string,
		gmathml_string_attribute_from_string,
		gmathml_string_attribute_finalize
	},
	{
		gmathml_color_attribute_to_string,
		gmathml_color_attribute_from_string,
		gmathml_color_attribute_finalize
	}
};

typedef struct {
	const GMathmlAttributeClass *attr_class;
	ptrdiff_t attr_offset;
} GMathmlAttributeInfos;

typedef struct GHashTable _GMathmlAttributes;

GMathmlAttributes *
gmathml_attributes_new (void)
{
	GHashTable *hash;

	hash = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);

	return (GMathmlAttributes *) hash;
}

void
gmathml_attributes_add_attribute (GMathmlAttributes *attributes,
				  const char *attr_name,
				  GMathmlAttributeType attr_type,
				  ptrdiff_t attr_offset)
{
	GHashTable *hash = (GHashTable *) attributes;
	GMathmlAttributeInfos *attr_infos;

	g_return_if_fail (attributes != NULL);
	g_return_if_fail (attr_name != NULL);
	g_return_if_fail (attr_type >= 0 && attr_type < G_N_ELEMENTS (gmathml_attribute_classes));
	g_return_if_fail (attr_offset >= 0);

	if (g_hash_table_lookup (hash, attr_name) != NULL) {
		g_warning ("[GMathmlAttributes::add_attribute] %s already defined", attr_name);
		return;
	}

	attr_infos = g_new (GMathmlAttributeInfos, 1);
	attr_infos->attr_class = &gmathml_attribute_classes [attr_type];
	attr_infos->attr_offset = attr_offset;

	g_hash_table_insert (hash, (char *) attr_name, attr_infos);
}

gboolean
gmathml_attributes_set_attribute (GMathmlAttributes *attributes,
				  void *instance,
				  const char *attr_name,
				  const char *attr_value)
{
	GHashTable *hash = (GHashTable *) attributes;
	GMathmlAttributeInfos *attr_infos;

	g_return_val_if_fail (attributes != NULL, FALSE);

	attr_infos = g_hash_table_lookup (hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	attr_infos->attr_class->from_string (instance + attr_infos->attr_offset, attr_value);

	return TRUE;
}

char const *
gmathml_attributes_get_attribute (GMathmlAttributes *attributes,
				  void *instance,
				  const char *attr_name)
{
	GHashTable *hash = (GHashTable *) attributes;
	GMathmlAttributeInfos *attr_infos;

	g_return_val_if_fail (attributes != NULL, FALSE);

	attr_infos = g_hash_table_lookup (hash, attr_name);
	if (attr_infos == NULL)
		return NULL;

	return attr_infos->attr_class->to_string (instance + attr_infos->attr_offset);
}

gboolean
gmathml_attributes_is_attribute_defined (GMathmlAttributes *attributes,
					 void *instance,
					 const char *attr_name)
{
	GHashTable *hash = (GHashTable *) attributes;
	GMathmlAttributeInfos *attr_infos;

	g_return_val_if_fail (attributes != NULL, FALSE);

	attr_infos = g_hash_table_lookup (hash, attr_name);
	if (attr_infos == NULL)
		return FALSE;

	return ((GMathmlAttribute *)((void *)(instance + attr_infos->attr_offset)))->is_defined;
}

static void
gmathml_attribute_finalize_cb (gpointer key,
			       gpointer value,
			       gpointer instance)
{
	GMathmlAttributeInfos *attr_infos = value;

	if (attr_infos->attr_class->finalize)
		attr_infos->attr_class->finalize (instance + attr_infos->attr_offset);
}

void
gmathml_attributes_finalize_attributes (GMathmlAttributes *attributes, void *instance)
{
	GHashTable *hash = (GHashTable *) attributes;

	g_return_if_fail (attributes != NULL);

	g_hash_table_foreach (hash, gmathml_attribute_finalize_cb, instance);
}
