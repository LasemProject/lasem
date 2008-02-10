/* gmathmlenums.h
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

#ifndef GMATHML_ENUMS_H
#define GMATHML_ENUMS_H

#include <gmathml.h>

G_BEGIN_DECLS

typedef enum {
	GMATHML_MODE_DISPLAY,
	GMATHML_MODE_INLINE
} GMathmlMode;

const char *		gmathml_mode_to_string			(GMathmlMode mode);
GMathmlMode		gmathml_mode_from_string		(const char *string);

typedef enum {
	GMATHML_SPACE_NAME_ERROR,
	GMATHML_SPACE_NAME_VERY_VERY_THIN,
	GMATHML_SPACE_NAME_VERY_THIN,
	GMATHML_SPACE_NAME_THIN,
	GMATHML_SPACE_NAME_MEDIUM,
	GMATHML_SPACE_NAME_THICK,
	GMATHML_SPACE_NAME_VERY_THICK,
	GMATHML_SPACE_NAME_VERY_VERY_THICK
} GMathmlSpaceName;

const char * 		gmathml_space_name_to_string 		(GMathmlSpaceName name);
GMathmlSpaceName 	gmathml_space_name_from_string 		(const char *string);

typedef enum {
	GMATHML_UNIT_NONE,
	GMATHML_UNIT_EM,
	GMATHML_UNIT_EX,
	GMATHML_UNIT_IN,
	GMATHML_UNIT_CM,
	GMATHML_UNIT_MM,
	GMATHML_UNIT_PT,
	GMATHML_UNIT_PX,
	GMATHML_UNIT_PC,
	GMATHML_UNIT_PERCENT
} GMathmlUnit;

const char * 		gmathml_unit_to_string 			(GMathmlUnit unit);
GMathmlUnit 		gmathml_unit_from_string 		(const char *string);

typedef enum {
	GMATHML_VARIANT_NORMAL,
	GMATHML_VARIANT_BOLD,
	GMATHML_VARIANT_ITALIC,
	GMATHML_VARIANT_BOLD_ITALIC,
	GMATHML_VARIANT_DOUBLE_STRUCK,
	GMATHML_VARIANT_BOLD_FRAKTUR,
	GMATHML_VARIANT_SCRIPT,
	GMATHML_VARIANT_BOLD_SCRIPT,
	GMATHML_VARIANT_FRAKTUR,
	GMATHML_VARIANT_SANS_SERIF,
	GMATHML_VARIANT_BOLD_SANS_SERIF,
	GMATHML_VARIANT_SANS_SERIF_ITALIC,
	GMATHML_VARIANT_SANS_SERIF_BOLD_ITALIC,
	GMATHML_VARIANT_MONOSPACE,
	GMATHML_VARIANT_ERROR
} GMathmlVariant;

const char * 		gmathml_variant_to_string 	(GMathmlVariant variant);
GMathmlVariant 		gmathml_variant_from_string 	(const char *string);

typedef enum {
	GMATHML_FORM_PREFIX,
	GMATHML_FORM_POSTFIX,
	GMATHML_FORM_INFIX,
	GMATHML_FORM_ERROR
} GMathmlForm;

const char *		gmathml_form_to_string 			(GMathmlForm form);
GMathmlForm 		gmathml_form_from_string 		(const char *string);

typedef enum {
	GMATHML_ROW_ALIGN_BASELINE,
	GMATHML_ROW_ALIGN_TOP,
	GMATHML_ROW_ALIGN_BOTTOM,
	GMATHML_ROW_ALIGN_CENTER,
	GMATHML_ROW_ALIGN_AXIS,
	GMATHML_ROW_ALIGN_ERROR
} GMathmlRowAlign;

const char *		gmathml_row_align_to_string		(GMathmlRowAlign row_align);
GMathmlRowAlign		gmathml_row_align_from_string		(const char *string);

typedef enum {
	GMATHML_COLUMN_ALIGN_CENTER,
	GMATHML_COLUMN_ALIGN_LEFT,
	GMATHML_COLUMN_ALIGN_RIGHT
} GMathmlColumnAlign;

const char *		gmathml_column_align_to_string		(GMathmlColumnAlign column_align);
GMathmlColumnAlign	gmathml_column_align_from_string	(const char *string);

G_END_DECLS

#endif

