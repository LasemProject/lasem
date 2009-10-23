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

#ifndef LSM_MATHML_STYLE_H
#define LSM_MATHML_STYLE_H

#include <lsmmathml.h>
#include <lsmmathmlattributes.h>

G_BEGIN_DECLS

GType lsm_mathml_style_get_type (void);

#define LSM_TYPE_MATHML_STYLE (lsm_mathml_style_get_type())

struct _LsmMathmlStyle {
	double math_size_value;
	/* mstyle */
	LsmMathmlDisplay display;

	int script_level;
	double script_size_multiplier;
	LsmMathmlLength script_min_size;

	LsmMathmlLength very_very_thin_math_space;
	LsmMathmlLength very_thin_math_space;
	LsmMathmlLength thin_math_space;
	LsmMathmlLength medium_math_space;
	LsmMathmlLength thick_math_space;
	LsmMathmlLength very_thick_math_space;
	LsmMathmlLength very_very_thick_math_space;

	double very_very_thin_math_space_value;
	double very_thin_math_space_value;
	double thin_math_space_value;
	double medium_math_space_value;
	double thick_math_space_value;
	double very_thick_math_space_value;
	double very_very_thick_math_space_value;
	/* tokens */
	char *math_family;
	LsmMathmlVariant math_variant;
	LsmMathmlLength math_size;
	LsmMathmlColor math_color;
	LsmMathmlColor math_background;
	/* mfrac */
	gboolean bevelled;
	/* msubsup */
	LsmMathmlLength subscript_shift;
	LsmMathmlLength superscript_shift;
	/* mfrac */
	LsmMathmlLength line_thickness;
};

LsmMathmlStyle *	lsm_mathml_style_new 			(void);
void			lsm_mathml_style_free 			(LsmMathmlStyle *style);
LsmMathmlStyle *	lsm_mathml_style_duplicate 		(const LsmMathmlStyle *from);

void 			lsm_mathml_style_change_script_level 	(LsmMathmlStyle *style, int increment);

void 			lsm_mathml_style_set_math_size_pt 	(LsmMathmlStyle *style, double math_size);
void 			lsm_mathml_style_set_script_min_size_pt	(LsmMathmlStyle *style, double script_min_size);
void 			lsm_mathml_style_set_math_family 	(LsmMathmlStyle *style, const char *math_family);
void 			lsm_mathml_style_set_math_variant	(LsmMathmlStyle *style, LsmMathmlVariant math_variant);
void 			lsm_mathml_style_set_math_color 	(LsmMathmlStyle *style,
								 double red, double green, double blue, double alpha);

G_END_DECLS

#endif
