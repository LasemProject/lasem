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

#include <lsmdom.h>
#include <lsmmathml.h>
#include <lsmmathmlenums.h>
#include <pango/pango-attributes.h>

G_BEGIN_DECLS

typedef enum {
	LSM_DOM_CSS_TYPE_USER,
	LSM_DOM_CSS_TYPE_AUTHOR,
	LSM_DOM_CSS_TYPE_AUTHOR_IMPORTANT,
	LSM_DOM_CSS_TYPE_USER_IMPORTANT
} LsmDomCssType;

typedef struct {
	char *value;
	char *css_value;
	LsmDomCssType css_type;
} LsmMathmlAttribute;

typedef struct {
	GHashTable *attribute_hash;
	GHashTable *bag_hash;
} LsmMathmlAttributeMap;

typedef struct {
} LsmMathmlAttributeBag;

typedef struct {
	void * 			(*init) 	(void);
	void 			(*finalize) 	(void *bag);
} LsmMathmlAttributeBagClass;

typedef struct {
	void (*finalize) (void *attribute);
} LsmMathmlAttributeClass;

typedef void (*LsmMathmlAttributeFinalizeFunc) (void *);

LsmMathmlAttributeMap *	lsm_mathml_attribute_map_new 		(void);
LsmMathmlAttributeMap *	lsm_mathml_attribute_map_duplicate		(const LsmMathmlAttributeMap *from);
void			lsm_mathml_attribute_map_free		(LsmMathmlAttributeMap *map);

void 		lsm_mathml_attribute_map_add_bag_attribute  	(LsmMathmlAttributeMap *map,
								 const char *name,
								 ptrdiff_t attribute_offset,
								 const LsmMathmlAttributeClass *attribute_class,
								 ptrdiff_t bag_offset,
								 const LsmMathmlAttributeBagClass *bag_class);
void		lsm_mathml_attribute_map_add_attribute_full	(LsmMathmlAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset,
								 const LsmMathmlAttributeClass *attribute_class);
void		lsm_mathml_attribute_map_add_attribute 		(LsmMathmlAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset);

void		lsm_mathml_attribute_map_free_attributes 	(LsmMathmlAttributeMap *map,
								 void *instance);

gboolean	lsm_mathml_attribute_map_set_attribute		(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value);
char const *	lsm_mathml_attribute_map_get_attribute		(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name);
gboolean	lsm_mathml_attribute_map_set_css_attribute	(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value,
								 LsmDomCssType css_type);
gboolean	lsm_mathml_attribute_map_is_attribute_defined	(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name);

gboolean 	lsm_mathml_attribute_is_defined 		(const LsmMathmlAttribute *attribute);
char const * 	lsm_mathml_attribute_get_value 			(const LsmMathmlAttribute *attribute);

typedef unsigned int (*LsmDomNamedConvert) (const char *string);

#define LSM_MATHML_SPACE_EM_VERY_VERY_THIN	0.055556
#define LSM_MATHML_SPACE_EM_VERY_THIN		0.111111
#define LSM_MATHML_SPACE_EM_THIN		0.166667
#define LSM_MATHML_SPACE_EM_MEDIUM	 	0.222222
#define LSM_MATHML_SPACE_EM_THICK		0.277778
#define LSM_MATHML_SPACE_EM_VERY_THICK		0.333333
#define LSM_MATHML_SPACE_EM_VERY_VERY_THICK	0.388889

typedef struct {
	LsmMathmlAttribute attr;
	gboolean value;
} LsmMathmlBooleanAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	unsigned int value;
} LsmMathmlUnsignedAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	double value;
} LsmMathmlDoubleAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	char *value;
} LsmMathmlStringAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	unsigned int value;
} LsmMathmlEnumAttribute;

typedef struct {
	unsigned int n_values;
	unsigned int *values;
} LsmMathmlEnumList;

typedef struct {
	LsmMathmlAttribute attr;
	unsigned int n_values;
	unsigned int *values;
} LsmMathmlEnumListAttribute;

GType lsm_mathml_color_get_type (void);

#define LSM_TYPE_MATHML_COLOR (lsm_mathml_color_get_type())

typedef struct {
	double red;
	double green;
	double blue;
	double alpha;
} LsmMathmlColor;

GType lsm_mathml_length_get_type (void);

#define LSM_TYPE_MATHML_LENGTH (lsm_mathml_length_get_type())

typedef struct {
	double value;
	LsmMathmlUnit unit;
} LsmMathmlLength;

GType lsm_mathml_space_get_type (void);

#define LSM_TYPE_MATHML_SPACE (lsm_mathml_space_get_type())

typedef struct {
	LsmMathmlSpaceName name;
	LsmMathmlLength length;
} LsmMathmlSpace;

GType lsm_mathml_space_list_get_type (void);

#define LSM_TYPE_MATHML_SPACE_LIST (lsm_mathml_space_list_get_type())

typedef struct {
	unsigned int n_spaces;
	LsmMathmlSpace *spaces;
} LsmMathmlSpaceList;

double 	lsm_mathml_length_compute 	(const LsmMathmlLength *length, double default_value, double font_size);

typedef struct {
	LsmMathmlAttribute attr;
	int value;
} LsmMathmlScriptLevelAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlColor color;
} LsmMathmlColorAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlLength length;
	double value;
} LsmMathmlLengthAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlSpace space;
	double value;
} LsmMathmlSpaceAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlSpaceList *space_list;
	double *values;
} LsmMathmlSpaceListAttribute;

void 		lsm_mathml_boolean_attribute_parse	(LsmMathmlBooleanAttribute *attribute,
							 gboolean *default_value);
void 		lsm_mathml_unsigned_attribute_parse	(LsmMathmlUnsignedAttribute *attribute,
							 unsigned *default_value);
void 		lsm_mathml_double_attribute_parse	(LsmMathmlDoubleAttribute *attribute,
							 double *default_value);
void		lsm_mathml_string_attribute_parse	(LsmMathmlStringAttribute *attribute,
							 char **default_value);
void 		lsm_mathml_enum_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *default_value,
							 LsmDomNamedConvert convert);
void 		lsm_mathml_enum_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value,
							 LsmDomNamedConvert convert);

void		lsm_mathml_string_attribute_finalize	(void *abstract);
void 		lsm_mathml_enum_list_attribute_finalize	(void *abstract);


void 		lsm_mathml_attribute_map_add_string 	(LsmMathmlAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);
void 		lsm_mathml_attribute_map_add_enum_list 	(LsmMathmlAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);

void 		lsm_mathml_script_level_attribute_parse	(LsmMathmlScriptLevelAttribute *attribute,
							 int *default_value);
void 		lsm_mathml_mode_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_mathml_display_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_mathml_form_attribute_parse	 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_mathml_font_style_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_mathml_font_weight_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void 		lsm_mathml_variant_attribute_parse 	(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void		lsm_mathml_line_attribute_parse		(LsmMathmlEnumAttribute *attribute,
							 unsigned int *style_value);
void		lsm_mathml_color_attribute_parse	(LsmMathmlColorAttribute *attribute,
							 LsmMathmlColor *default_color);
void 		lsm_mathml_length_attribute_parse 	(LsmMathmlLengthAttribute *attribute,
							 LsmMathmlLength *style_value,
							 double font_size);
void 		lsm_mathml_space_attribute_parse 	(LsmMathmlSpaceAttribute *attribute,
							 LsmMathmlSpace *style_value,
							 LsmMathmlStyle *style);

LsmMathmlSpaceList *	lsm_mathml_space_list_new  	(unsigned int n_spaces);
void 			lsm_mathml_space_list_free 	(LsmMathmlSpaceList *space_list);
LsmMathmlSpaceList *	lsm_mathml_space_list_duplicate	(const LsmMathmlSpaceList *space_list);

void 		lsm_mathml_space_list_attribute_parse 	(LsmMathmlSpaceListAttribute *attribute,
							 LsmMathmlSpaceList *style_value,
							 const LsmMathmlStyle *style);

void 	lsm_mathml_row_align_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);
void 	lsm_mathml_column_align_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);
void 	lsm_mathml_line_list_attribute_parse 		(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);

void 	lsm_mathml_attribute_map_add_space_list 	(LsmMathmlAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);
G_END_DECLS

#endif
