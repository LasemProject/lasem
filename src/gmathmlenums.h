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
	GMATHML_FORM_PREFIX,
	GMATHML_FORM_POSTFIX,
	GMATHML_FORM_INFIX
} GMathmlForm;

typedef enum {
	GMATHML_MODE_DISPLAY,
	GMATHML_MODE_INLINE
} GMathmlMode;

const char *		gmathml_mode_to_string			(GMathmlMode mode);
GMathmlMode		gmathml_mode_from_string		(const char *string);

typedef enum {
	GMATHML_NAMED_SPACE_VERY_VERY_THIN,
	GMATHML_NAMED_SPACE_VERY_THIN,
	GMATHML_NAMED_SPACE_THIN,
	GMATHML_NAMED_SPACE_MEDIUM,
	GMATHML_NAMED_SPACE_THICK,
	GMATHML_NAMED_SPACE_VERY_THICK,
	GMATHML_NAMED_SPACE_VERY_VERY_THICK,
	GMATHML_NAMED_SPACE_ERROR
} GMathmlNamedSpace;

const char * 		gmathml_named_space_to_string 		(GMathmlNamedSpace named_space);
GMathmlNamedSpace 	gmathml_named_space_from_string 	(const char *string);

typedef enum {
	GMATHML_UNIT_EM,
	GMATHML_UNIT_EX,
	GMATHML_UNIT_IN,
	GMATHML_UNIT_CM,
	GMATHML_UNIT_MM,
	GMATHML_UNIT_PT,
	GMATHML_UNIT_PX,
	GMATHML_UNIT_PC,
	GMATHML_UNIT_PERCENT,
	GMATHML_UNIT_NONE
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

#define GMATHML_SIZE_INFINITY 10
#define GMATHML_ZERO_EM 0
#define GMATHML_1_PX 1
#define GMATHML_0_PX 0

G_END_DECLS

#endif

