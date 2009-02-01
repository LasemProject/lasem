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

#ifndef GDOM_ATTRIBUTES_H
#define GDOM_ATTRIBUTES_H

#include <gdom.h>

G_BEGIN_DECLS

typedef enum {
	GDOM_CSS_TYPE_USER,
	GDOM_CSS_TYPE_AUTHOR,
	GDOM_CSS_TYPE_AUTHOR_IMPORTANT,
	GDOM_CSS_TYPE_USER_IMPORTANT
} GDomCssType;

typedef struct {
	char *value;
	char *css_value;
	GDomCssType css_type;
} GDomAttributeValue;

typedef struct {
	GHashTable *hash;
	GArray *array;
} GDomAttributeMap;

typedef void (*GDomAttributeFinalizeFunc) (void *);

GDomAttributeMap *	gdom_attribute_map_new 		(void);
void			gdom_attribute_map_free		(GDomAttributeMap *map);

void			gdom_attribute_map_add_attribute_full	(GDomAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset,
								 GDomAttributeFinalizeFunc finalize);
void			gdom_attribute_map_add_attribute 	(GDomAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset);

void			gdom_attribute_map_free_attributes 	(GDomAttributeMap *map,
								 void *instance);
void			gdom_attribute_map_free_css_values	(GDomAttributeMap *map,
								 void *instance);

gboolean		gdom_attribute_map_set_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *attr_name,
								 char const *attr_value);
char const *		gdom_attribute_map_get_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *name);
void			gdom_attribute_map_set_css_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value,
								 GDomCssType css_type);
gboolean		gdom_attribute_map_is_attribute_defined	(GDomAttributeMap *map,
								 void *instance,
								 char const *name);

char const * 		gdom_attribute_value_get_actual_value 	(const GDomAttributeValue *attribute);

typedef struct {
	GDomAttributeValue attr;
	gboolean value;
} GDomAttributeBoolean;

typedef struct {
	GDomAttributeValue attr;
	unsigned int value;
} GDomAttributeUnsigned;

typedef struct {
	GDomAttributeValue attr;
	double value;
} GDomAttributeDouble;

typedef struct {
	GDomAttributeValue attr;
	char *value;
} GDomAttributeString;

typedef struct {
	GDomAttributeValue attr;
	unsigned int value;
} GDomAttributeNamed;

typedef struct {
	unsigned int n_values;
	unsigned int *values;
} GDomNamedList;

typedef struct {
	GDomAttributeValue attr;
	unsigned int n_values;
	unsigned int *values;
} GDomAttributeNamedList;

typedef unsigned int (*GDomNamedConvert) (const char *string);

void 	gdom_attribute_boolean_parse		(GDomAttributeBoolean *attribute,
						 gboolean *default_value);
void 	gdom_attribute_unsigned_parse		(GDomAttributeUnsigned *attribute,
						 unsigned *default_value);
void 	gdom_attribute_double_parse		(GDomAttributeDouble *attribute,
						 double *default_value);
void	gdom_attribute_string_parse		(GDomAttributeString *attribute,
						 char **default_value);
void 	gdom_attribute_named_parse 		(GDomAttributeNamed *attribute,
						 unsigned int *default_value,
						 GDomNamedConvert convert);
void 	gdom_attribute_named_list_parse 	(GDomAttributeNamedList *attribute,
						 GDomNamedList *style_value,
						 GDomNamedConvert convert);

void	gdom_attribute_string_finalize		(void *abstract);
void 	gdom_attribute_named_list_finalize 	(void *abstract);

G_END_DECLS

#endif
