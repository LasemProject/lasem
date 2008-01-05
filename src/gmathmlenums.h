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
	GMATHML_VERY_VERY_THIN_MATH_SPACE,
	GMATHML_VERY_THIN_MATH_SPACE,
	GMATHML_THIN_MATH_SPACE,
	GMATHML_MEDIUM_MATH_SPACE,
	GMATHML_THICK_MATH_SPACE,
	GMATHML_VERY_THICK_MATH_SPACE,
	GMATHML_VERY_VERY_THICK_MATH_SPACE
} GMathmlNamedSpace;

#define GMATHML_SIZE_INFINITY 10
#define GMATHML_ZERO_EM 0
#define GMATHML_1_PX 1
#define GMATHML_0_PX 0

G_END_DECLS

#endif

