/* lsmmathmloperatordictionary.h
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

#ifndef LSM_MATHML_OPERATOR_DICTIONARY_H
#define LSM_MATHML_OPERATOR_DICTIONARY_H

#include <lsmmathml.h>
#include <lsmmathmlenums.h>
#include <lsmmathmlattributes.h>

G_BEGIN_DECLS

typedef struct {
	const char 	*name;
	LsmMathmlForm	 form;
	LsmMathmlSpace	 left_space;
	LsmMathmlSpace	 right_space;
	gboolean	 stretchy;
	gboolean	 fence;
	gboolean	 accent;
	gboolean	 large_op;
	gboolean	 movable_limits;
	gboolean	 separator;
	LsmMathmlSpace	 min_size;
	LsmMathmlSpace	 max_size;
	gboolean	 symmetric;
} LsmMathmlOperatorDictionaryEntry;

const LsmMathmlOperatorDictionaryEntry *	lsm_mathml_operator_dictionary_lookup (const char *utf8, LsmMathmlForm form);

G_END_DECLS

#endif

