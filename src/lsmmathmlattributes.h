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

#ifndef LSM_MATHML_ATTRIBUTES_H
#define LSM_MATHML_ATTRIBUTES_H

#include <lsmattributes.h>
#include <lsmdom.h>
#include <lsmmathml.h>
#include <lsmmathmlenums.h>
#include <lsmmathmltraits.h>

G_BEGIN_DECLS

typedef struct {
	LsmAttribute base;
	gboolean value;
} LsmMathmlBooleanAttribute;

typedef struct {
	LsmAttribute base;
	unsigned int value;
} LsmMathmlUnsignedAttribute;

typedef struct {
	LsmAttribute base;
	unsigned int value;
} LsmMathmlEnumAttribute;

typedef struct {
	LsmAttribute base;
	double value;
} LsmMathmlDoubleAttribute;

typedef struct {
	LsmAttribute base;
	LsmMathmlScriptLevel value;
} LsmMathmlScriptLevelAttribute;

typedef struct {
	LsmAttribute base;
	LsmMathmlColor color;
} LsmMathmlColorAttribute;

typedef struct {
	LsmAttribute base;
	char *value;
} LsmMathmlStringAttribute;

gboolean		lsm_mathml_boolean_attribute_inherit 	(LsmMathmlBooleanAttribute *attribute,
								 gboolean value);
double 			lsm_mathml_double_attribute_inherit 	(LsmMathmlDoubleAttribute *attribute,
								 double value);
LsmMathmlColor		lsm_mathml_color_attribute_inherit 	(LsmMathmlColorAttribute *attribute,
								 LsmMathmlColor value);
const char *		lsm_mathml_string_attribute_inherit	(LsmMathmlStringAttribute *attribute,
								 const char *string);

int			lsm_mathml_script_level_attribute_apply	(LsmMathmlScriptLevelAttribute *attribute,
								 int script_level);

unsigned int		lsm_mathml_enum_attribute_inherit	(LsmMathmlEnumAttribute *attribute,
								 unsigned int value);

typedef struct {
	LsmAttribute base;
	LsmMathmlEnumList enum_list;
} LsmMathmlEnumListAttribute;

typedef struct {
	LsmAttribute base;
	LsmMathmlLength length;
        double value;
} LsmMathmlLengthAttribute;

double 		lsm_mathml_length_attribute_normalize	(LsmMathmlLengthAttribute *atribute,
							 const LsmMathmlLength *default_value,
							 const LsmMathmlStyle *style);

typedef struct {
	LsmAttribute base;
	LsmMathmlSpace space;
	double value;
} LsmMathmlSpaceAttribute;

double 		lsm_mathml_space_attribute_normalize 	(LsmMathmlSpaceAttribute *attribute,
							 const LsmMathmlSpace *default_value,
							 const LsmMathmlStyle *style);

typedef struct {
	LsmAttribute base;
	LsmMathmlSpaceList space_list;
	double *values;
} LsmMathmlSpaceListAttribute;

void 		lsm_mathml_space_list_attribute_normalize 	(LsmMathmlSpaceListAttribute *attribute,
								 const LsmMathmlSpaceList *default_value,
								 const LsmMathmlStyle *style);

G_END_DECLS

#endif
