/*
 * Copyright © 2009 Emmanuel Pacaud
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

#ifndef LSM_MATHML_TRAITS_H
#define LSM_MATHML_TRAITS_H

#include <lsmmathmlenums.h>
#include <lsmtraits.h>

G_BEGIN_DECLS

#define LSM_TYPE_MATHML_COLOR (lsm_mathml_color_get_type())

typedef struct {
	double red;
	double green;
	double blue;
	double alpha;
} LsmMathmlColor;

GType lsm_mathml_color_get_type (void);

#define LSM_TYPE_MATHML_LENGTH (lsm_mathml_length_get_type())

typedef struct {
	double value;
	LsmMathmlUnit unit;
} LsmMathmlLength;

GType lsm_mathml_length_get_type (void);
double 	lsm_mathml_length_normalize 	(const LsmMathmlLength *length, double default_value, double font_size);

#define LSM_TYPE_MATHML_SPACE (lsm_mathml_space_get_type())

typedef struct {
	LsmMathmlSpaceName name;
	LsmMathmlLength length;
} LsmMathmlSpace;

GType 	lsm_mathml_space_get_type 	(void);

#define LSM_TYPE_MATHML_SPACE_LIST (lsm_mathml_space_list_get_type())

typedef struct {
	unsigned int n_spaces;
	LsmMathmlSpace *spaces;
} LsmMathmlSpaceList;

GType 			lsm_mathml_space_list_get_type 	(void);
LsmMathmlSpaceList *	lsm_mathml_space_list_new  	(unsigned int n_spaces);
void 			lsm_mathml_space_list_free 	(LsmMathmlSpaceList *space_list);
LsmMathmlSpaceList *	lsm_mathml_space_list_duplicate	(const LsmMathmlSpaceList *space_list);

extern const LsmTraitClass lsm_mathml_boolean_trait_class;

extern const LsmTraitClass lsm_mathml_unsigned_trait_class;
extern const LsmTraitClass lsm_mathml_display_trait_class;
extern const LsmTraitClass lsm_mathml_mode_trait_class;
extern const LsmTraitClass lsm_mathml_line_trait_class;
extern const LsmTraitClass lsm_mathml_font_style_trait_class;
extern const LsmTraitClass lsm_mathml_font_weight_trait_class;
extern const LsmTraitClass lsm_mathml_variant_trait_class;
extern const LsmTraitClass lsm_mathml_form_trait_class;

extern const LsmTraitClass lsm_mathml_double_trait_class;
extern const LsmTraitClass lsm_mathml_string_trait_class;
extern const LsmTraitClass lsm_mathml_length_trait_class;

extern const LsmTraitClass lsm_mathml_color_trait_class;

G_END_DECLS

#endif
