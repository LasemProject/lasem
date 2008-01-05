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
#include <pango/pango-attributes.h>

G_BEGIN_DECLS

typedef enum {
	GMATHML_ATTRIBUTE_BOOLEAN,
	GMATHML_ATTRIBUTE_INT,
	GMATHML_ATTRIBUTE_STRING,
	GMATHML_ATTRIBUTE_COLOR
} GMathmlAttributeType;

typedef struct _GMathmlAttributes GMathmlAttributes;

typedef struct {
	gboolean	is_defined;
} GMathmlAttribute;

typedef struct {
	GMathmlAttribute base;
	char *string;
	int value;
} GMathmlIntAttribute;

typedef struct {
	GMathmlAttribute base;
	char *string;
} GMathmlStringAttribute;

typedef struct {
	GMathmlAttribute base;
	gboolean value;
} GMathmlBooleanAttribute;

typedef struct {
	GMathmlAttribute base;
	char *string;
	PangoColor color;
} GMathmlColorAttribute;

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
