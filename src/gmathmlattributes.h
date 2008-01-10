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

typedef enum {
	GMATHML_ATTRIBUTE_BOOLEAN,
	GMATHML_ATTRIBUTE_INT,
	GMATHML_ATTRIBUTE_INCREMENT,
	GMATHML_ATTRIBUTE_DOUBLE,
	GMATHML_ATTRIBUTE_LENGTH,
	GMATHML_ATTRIBUTE_STRING,
	GMATHML_ATTRIBUTE_COLOR
} GMathmlAttributeType;

#define GMATHML_ATTRIBUTE_VLENGTH GMATHML_ATTRIBUTE_LENGTH
#define GMATHML_ATTRIBUTE_HLENGTH GMATHML_ATTRIBUTE_LENGTH

typedef struct _GMathmlAttributes GMathmlAttributes;

/* All attribute structures must begin with the is_defined field */

typedef struct {
	gboolean	is_defined;
} GMathmlAttribute;

typedef struct {
	gboolean	is_defined;
	gboolean value;
} GMathmlBooleanAttribute;

void 	gmathml_boolean_attribute_set_default 	(GMathmlBooleanAttribute *self,
						 gboolean value);

typedef struct {
	gboolean	is_defined;
	char *string;
	int value;
} GMathmlIntAttribute;

void 	gmathml_int_attribute_set_default 	(GMathmlIntAttribute *self,
						 int value);

typedef struct {
	gboolean	is_defined;
	char *string;
	int value;
	GMathmlLevelType type;
} GMathmlIncrementAttribute;

void 	gmathml_increment_attribute_set_default (GMathmlIncrementAttribute *self,
						 unsigned int value,
						 GMathmlLevelType type);

typedef struct {
	gboolean	is_defined;
	char *string;
	double value;
} GMathmlDoubleAttribute;

void 	gmathml_double_attribute_set_default	(GMathmlDoubleAttribute *self,
						 double value);

/* FIXME real HUnit and VUnit */

typedef struct {
	gboolean	is_defined;
	char *string;
	double value;
	GMathmlUnit unit;
} GMathmlLengthAttribute;

#define GMathmlVLengthAttribute GMathmlLengthAttribute
#define GMathmlHLengthAttribute GMathmlLengthAttribute

void 	gmathml_length_attribute_set_default 	(GMathmlLengthAttribute *self,
						 double value,
						 GMathmlUnit unit);

typedef struct {
	gboolean	is_defined;
	char *string;
} GMathmlStringAttribute;

void 	gmathml_string_attribute_set_default 	(GMathmlStringAttribute *self,
						 const char *string);

typedef struct {
	gboolean	is_defined;
	char *string;
	PangoColor color;
} GMathmlColorAttribute;

void 	gmathml_color_attribute_set_default 	(GMathmlColorAttribute *self,
						 const PangoColor *color);

typedef struct {
	GMathmlStringAttribute	class_name;
	GMathmlStringAttribute	id;
} GMathmlElementAttributes;

typedef struct {
	GMathmlIncrementAttribute script_level;
	GMathmlBooleanAttribute display_style;
	GMathmlDoubleAttribute script_size_multiplier;
	GMathmlVLengthAttribute script_min_size;
	GMathmlColorAttribute background;
	GMathmlHLengthAttribute very_very_thin_math_space;
	GMathmlVLengthAttribute very_thin_math_space;
	GMathmlVLengthAttribute thin_math_space;
	GMathmlVLengthAttribute medium_math_space;
	GMathmlVLengthAttribute thick_math_space;
	GMathmlVLengthAttribute very_thick_math_space;
	GMathmlVLengthAttribute very_very_thick_math_space;
} GMathmlStyleAttributes;

typedef struct {
#if 0
	GMathmlVariantAttribute math_variant;
#endif
	GMathmlVLengthAttribute math_size;
	GMathmlColorAttribute math_color;
	GMathmlColorAttribute math_background;
} GMathmlTokenAttributes;

GMathmlAttributes * 	gmathml_attributes_new 			(void);
void 			gmathml_attributes_add_attribute 	(GMathmlAttributes *attributes,
								 const char *attr_name,
								 GMathmlAttributeType attr_type,
								 ptrdiff_t attr_offset);
gboolean 		gmathml_attributes_set_attribute 	(GMathmlAttributes *attributes,
								 void *instance,
								 const char *attr_name,
								 const char *attr_value);
char const * 		gmathml_attributes_get_attribute 	(GMathmlAttributes *attributes,
								 void *instance,
								 const char *attr_name);
gboolean 		gmathml_attributes_is_attribute_defined	(GMathmlAttributes *attributes,
								 void *instance,
								 const char *attr_name);
void 			gmathml_attributes_finalize_attributes 	(GMathmlAttributes *attributes, void *instance);

G_END_DECLS

#endif
