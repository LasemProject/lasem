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

#ifndef GMATHML_ATTRIBUTES_H
#define GMATHML_ATTRIBUTES_H

#include <gdomattributes.h>
#include <gmathml.h>
#include <gmathmlenums.h>
#include <pango/pango-attributes.h>

G_BEGIN_DECLS

#define GMATHML_SPACE_EM_VERY_VERY_THIN		0.055556
#define GMATHML_SPACE_EM_VERY_THIN		0.111111
#define GMATHML_SPACE_EM_THIN			0.166667
#define GMATHML_SPACE_EM_MEDIUM		 	0.222222
#define GMATHML_SPACE_EM_THICK			0.277778
#define GMATHML_SPACE_EM_VERY_THICK		0.333333
#define GMATHML_SPACE_EM_VERY_VERY_THICK	0.388889

typedef struct {
	double red;
	double green;
	double blue;
	double alpha;
} GMathmlColor;

typedef struct {
	double value;
	GMathmlUnit unit;
} GMathmlLength;

typedef struct {
	GMathmlSpaceName name;
	GMathmlLength length;
} GMathmlSpace;

typedef struct {
	unsigned int n_spaces;
	GMathmlSpace *spaces;
} GMathmlSpaceList;

double 	gmathml_length_compute 		(const GMathmlLength *length, double default_value, double font_size);

typedef struct {
	GDomAttribute attr;
	int value;
} GMathmlScriptLevelAttribute;

typedef struct {
	GDomAttribute attr;
	GMathmlColor color;
} GMathmlColorAttribute;

typedef struct {
	GDomAttribute attr;
	GMathmlLength length;
	double value;
} GMathmlLengthAttribute;

typedef struct {
	GDomAttribute attr;
	GMathmlSpace space;
	double value;
} GMathmlSpaceAttribute;

typedef struct {
	GDomAttribute attr;
	GMathmlSpaceList *space_list;
	double *values;
} GMathmlSpaceListAttribute;

void 		gmathml_script_level_attribute_parse	(GMathmlScriptLevelAttribute *attribute,
							 int *default_value);
void 		gmathml_mode_attribute_parse 		(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		gmathml_display_attribute_parse 	(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		gmathml_form_attribute_parse	 	(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		gmathml_font_style_attribute_parse 	(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		gmathml_font_weight_attribute_parse 	(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void 		gmathml_variant_attribute_parse 	(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void		gmathml_line_attribute_parse		(GDomEnumAttribute *attribute,
							 unsigned int *style_value);
void		gmathml_color_attribute_parse		(GMathmlColorAttribute *attribute,
							 GMathmlColor *default_color);
void 		gmathml_length_attribute_parse 		(GMathmlLengthAttribute *attribute,
							 GMathmlLength *style_value,
							 double font_size);
void 		gmathml_space_attribute_parse 		(GMathmlSpaceAttribute *attribute,
							 GMathmlSpace *style_value,
							 GMathmlStyle *style);

GMathmlSpaceList *	gmathml_space_list_new  	(unsigned int n_spaces);
void 			gmathml_space_list_free 	(GMathmlSpaceList *space_list);
GMathmlSpaceList *	gmathml_space_list_duplicate	(const GMathmlSpaceList *space_list);

void 		gmathml_space_list_attribute_parse 	(GMathmlSpaceListAttribute *attribute,
							 GMathmlSpaceList *style_value,
							 const GMathmlStyle *style);

void 	gmathml_row_align_list_attribute_parse 		(GDomEnumListAttribute *attribute,
							 GDomEnumList *style_value);
void 	gmathml_column_align_list_attribute_parse 	(GDomEnumListAttribute *attribute,
							 GDomEnumList *style_value);
void 	gmathml_line_list_attribute_parse 		(GDomEnumListAttribute *attribute,
							 GDomEnumList *style_value);

void 	gdom_attribute_map_add_space_list 		(GDomAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);
G_END_DECLS

#endif
