/* gmathmlattributes.h
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

#ifndef GMATHML_ATTRIBUTES_H
#define GMATHML_ATTRIBUTES_H

#include <gmathml.h>
#include <gmathmlenums.h>
#include <pango/pango-attributes.h>

G_BEGIN_DECLS

typedef struct {
	double red;
	double green;
	double blue;
	double alpha;
} GMathmlColor;

typedef struct {
	/* mstyle */
	int script_level;
	gboolean display_style;
	double script_size_multiplier;
	double script_min_size;
	GMathmlColor background;
	double very_very_thin_math_space;
	double very_thin_math_space;
	double thin_math_space;
	double medium_math_space;
	double thick_math_space;
	double very_thick_math_space;
	double very_very_thick_math_space;
	/* tokens */
	GMathmlVariant math_variant;
	double math_size;
	GMathmlColor math_color;
	GMathmlColor math_background;
	/* mfrac */
	gboolean bevelled;
} GMathmlStyle;

GMathmlStyle *	gmathml_style_new 			(void);
void		gmathml_style_free 			(GMathmlStyle *style);
GMathmlStyle *	gmathml_style_duplicate 		(const GMathmlStyle *from);
void 		gmathml_style_change_script_level 	(GMathmlStyle *style, int increment);
void 		gmathml_style_dump 			(const GMathmlStyle *style);

typedef enum {
	GMATHML_CSS_TYPE_USER,
	GMATHML_CSS_TYPE_AUTHOR,
	GMATHML_CSS_TYPE_AUTHOR_IMPORTANT,
	GMATHML_CSS_TYPE_USER_IMPORTANT
} GMathmlCssType;

typedef struct {
	char *value;
	char *css_value;
	GMathmlCssType css_type;
} GMathmlAttributeValue;

typedef struct {
	GHashTable *hash;
	GArray *array;
} GMathmlAttributeMap;

GMathmlAttributeMap *	gmathml_attribute_map_new 		(void);
void			gmathml_attribute_map_free 		(GMathmlAttributeMap *map);

void			gmathml_attribute_map_add_attribute 		(GMathmlAttributeMap *map,
									 char const *name,
									 ptrdiff_t offset);

void			gmathml_attribute_map_free_attributes 		(GMathmlAttributeMap *map,
									 void *instance);
void			gmathml_attribute_map_free_css_values		(GMathmlAttributeMap *map,
									 void *instance);

gboolean		gmathml_attribute_map_set_attribute		(GMathmlAttributeMap *map,
									 void *instance,
									 char const *attr_name,
									 char const *attr_value);
char const *		gmathml_attribute_map_get_attribute		(GMathmlAttributeMap *map,
									 void *instance,
									 char const *name);
void			gmathml_attribute_map_set_css_attribute		(GMathmlAttributeMap *map,
									 void *instance,
									 char const *name,
									 char const *value,
									 GMathmlCssType css_type);
gboolean		gmathml_attribute_map_is_attribute_defined	(GMathmlAttributeMap *map,
									 void *instance,
									 char const *name);


typedef struct {
	GMathmlAttributeValue attr;
	gboolean value;
} GMathmlAttributeBoolean;

typedef struct {
	GMathmlAttributeValue attr;
	double value;
} GMathmlAttributeDouble;

typedef struct {
	GMathmlAttributeValue attr;
	int value;
} GMathmlAttributeScriptLevel;

typedef struct {
	GMathmlAttributeValue attr;
	GMathmlColor color;
} GMathmlAttributeColor;

typedef struct {
	GMathmlAttributeValue attr;
	GMathmlVariant value;
} GMathmlAttributeVariant;

typedef struct {
	GMathmlAttributeValue attr;
	double value;
} GMathmlAttributeLength;

void 		gmathml_attribute_boolean_parse		(GMathmlAttributeBoolean *attribute,
							 gboolean *default_value);
void 		gmathml_attribute_double_parse		(GMathmlAttributeDouble *attribute,
							 double *default_value);
void 		gmathml_attribute_script_level_parse	(GMathmlAttributeScriptLevel *attribute,
							 int *default_value);
void		gmathml_attribute_color_parse		(GMathmlAttributeColor *attribute,
							 GMathmlColor *default_color);
void		gmathml_attribute_length_parse		(GMathmlAttributeLength *attribute,
							 double *default_value,
							 double font_size);

G_END_DECLS

#endif
