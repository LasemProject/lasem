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
} GDomAttribute;

typedef struct {
	GHashTable *attribute_hash;
	GHashTable *bag_hash;
} GDomAttributeMap;

typedef struct {
} GDomAttributeBag;

typedef struct {
	void * 			(*init) 	(void);
	void 			(*finalize) 	(void *bag);
} GDomAttributeBagClass;

typedef struct {
	void (*finalize) (void *attribute);
} GDomAttributeClass;

typedef void (*GDomAttributeFinalizeFunc) (void *);

GDomAttributeMap *	gdom_attribute_map_new 			(void);
GDomAttributeMap *	gdom_attribute_map_duplicate		(const GDomAttributeMap *from);
void			gdom_attribute_map_free			(GDomAttributeMap *map);

void 			gdom_attribute_map_add_bag_attribute  	(GDomAttributeMap *map,
								 const char *name,
								 ptrdiff_t attribute_offset,
								 const GDomAttributeClass *attribute_class,
								 ptrdiff_t bag_offset,
								 const GDomAttributeBagClass *bag_class);
void			gdom_attribute_map_add_attribute_full	(GDomAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset,
								 const GDomAttributeClass *attribute_class);
void			gdom_attribute_map_add_attribute 	(GDomAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset);

void			gdom_attribute_map_free_attributes 	(GDomAttributeMap *map,
								 void *instance);

gboolean		gdom_attribute_map_set_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value);
char const *		gdom_attribute_map_get_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *name);
gboolean		gdom_attribute_map_set_css_attribute	(GDomAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value,
								 GDomCssType css_type);
gboolean		gdom_attribute_map_is_attribute_defined	(GDomAttributeMap *map,
								 void *instance,
								 char const *name);

char const * 		gdom_attribute_get_value 		(const GDomAttribute *attribute);

typedef struct {
	GDomAttribute attr;
	gboolean value;
} GDomBooleanAttribute;

typedef struct {
	GDomAttribute attr;
	unsigned int value;
} GDomUnsignedAttribute;

typedef struct {
	GDomAttribute attr;
	double value;
} GDomDoubleAttribute;

typedef struct {
	GDomAttribute attr;
	char *value;
} GDomStringAttribute;

typedef struct {
	GDomAttribute attr;
	unsigned int value;
} GDomEnumAttribute;

typedef struct {
	unsigned int n_values;
	unsigned int *values;
} GDomEnumList;

typedef struct {
	GDomAttribute attr;
	unsigned int n_values;
	unsigned int *values;
} GDomEnumListAttribute;

typedef unsigned int (*GDomNamedConvert) (const char *string);

void 	gdom_boolean_attribute_parse		(GDomBooleanAttribute *attribute,
						 gboolean *default_value);
void 	gdom_unsigned_attribute_parse		(GDomUnsignedAttribute *attribute,
						 unsigned *default_value);
void 	gdom_double_attribute_parse		(GDomDoubleAttribute *attribute,
						 double *default_value);
void	gdom_string_attribute_parse		(GDomStringAttribute *attribute,
						 char **default_value);
void 	gdom_enum_attribute_parse 		(GDomEnumAttribute *attribute,
						 unsigned int *default_value,
						 GDomNamedConvert convert);
void 	gdom_enum_list_attribute_parse 	(GDomEnumListAttribute *attribute,
						 GDomEnumList *style_value,
						 GDomNamedConvert convert);

void	gdom_string_attribute_finalize		(void *abstract);
void 	gdom_enum_list_attribute_finalize 	(void *abstract);


void 	gdom_attribute_map_add_string 		(GDomAttributeMap *map,
						 char const *name,
						 ptrdiff_t offset);
void 	gdom_attribute_map_add_enum_list 	(GDomAttributeMap *map,
						 char const *name,
						 ptrdiff_t offset);
G_END_DECLS

#endif
