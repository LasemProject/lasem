/* lsmmathmltableelement.h
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef LSM_MATHML_TABLE_ELEMENT_H
#define LSM_MATHML_TABLE_ELEMENT_H

#include <lsmmathml.h>
#include <lsmmathmlelement.h>

G_BEGIN_DECLS

#define LSM_TYPE_MATHML_TABLE_ELEMENT             (lsm_mathml_table_element_get_type ())
#define LSM_MATHML_TABLE_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_MATHML_TABLE_ELEMENT, LsmMathmlTableElement))
#define LSM_MATHML_TABLE_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_MATHML_TABLE_ELEMENT, LsmMathmlTableElementClass))
#define LSM_IS_MATHML_TABLE_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_MATHML_TABLE_ELEMENT))
#define LSM_IS_MATHML_TABLE_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_MATHML_TABLE_ELEMENT))
#define LSM_MATHML_TABLE_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_MATHML_TABLE_ELEMENT, LsmMathmlTableElementClass))

typedef struct _LsmMathmlTableElementClass LsmMathmlTableElementClass;

struct _LsmMathmlTableElement {
	LsmMathmlElement	element;

	LsmDomEnumListAttribute row_align;
	LsmDomEnumListAttribute column_align;
	LsmMathmlSpaceListAttribute row_spacing;
	LsmMathmlSpaceListAttribute column_spacing;
	LsmDomEnumListAttribute row_lines;
	LsmDomEnumListAttribute column_lines;
	LsmDomEnumAttribute frame;
	LsmMathmlSpaceListAttribute frame_spacing;
	LsmDomBooleanAttribute equal_rows;
	LsmDomBooleanAttribute equal_columns;
	LsmDomBooleanAttribute display_style;

	/* View data */

	unsigned int n_columns;
	unsigned int n_rows;
	double *widths;
	double *heights;
	double *depths;
	double line_width;
};

struct _LsmMathmlTableElementClass {
	LsmMathmlElementClass  parent_class;
};

GType lsm_mathml_table_element_get_type (void);

LsmDomNode * 	lsm_mathml_table_element_new 	(void);

G_END_DECLS

#endif

