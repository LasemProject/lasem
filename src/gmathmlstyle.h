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

#ifndef GMATHML_STYLE_H
#define GMATHML_STYLE_H

#include <gmathml.h>
#include <gmathmlattributes.h>

G_BEGIN_DECLS

struct _GMathmlStyle {
	double math_size_value;
	/* mstyle */
	GMathmlDisplay display;

	int script_level;
	double script_size_multiplier;
	GMathmlLength script_min_size;

	GMathmlLength very_very_thin_math_space;
	GMathmlLength very_thin_math_space;
	GMathmlLength thin_math_space;
	GMathmlLength medium_math_space;
	GMathmlLength thick_math_space;
	GMathmlLength very_thick_math_space;
	GMathmlLength very_very_thick_math_space;

	double very_very_thin_math_space_value;
	double very_thin_math_space_value;
	double thin_math_space_value;
	double medium_math_space_value;
	double thick_math_space_value;
	double very_thick_math_space_value;
	double very_very_thick_math_space_value;
	/* tokens */
	char *math_family;
	GMathmlVariant math_variant;
	GMathmlLength math_size;
	GMathmlColor math_color;
	GMathmlColor math_background;
	/* mfrac */
	gboolean bevelled;
	/* msubsup */
	GMathmlLength subscript_shift;
	GMathmlLength superscript_shift;
	/* mfrac */
	GMathmlLength line_thickness;
};

GMathmlStyle *	gmathml_style_new 			(void);
void		gmathml_style_free 			(GMathmlStyle *style);
GMathmlStyle *	gmathml_style_duplicate 		(const GMathmlStyle *from);
void 		gmathml_style_change_script_level 	(GMathmlStyle *style, int increment);
void 		gmathml_style_set_math_size_pt 		(GMathmlStyle *style, double math_size);
void 		gmathml_style_set_math_family 		(GMathmlStyle *style, const char *math_family);
void 		gmathml_style_set_math_variant		(GMathmlStyle *style, GMathmlVariant math_variant);
void 		gmathml_style_set_math_color 		(GMathmlStyle *style,
							 double red, double green, double blue, double alpha);
void 		gmathml_style_dump 			(const GMathmlStyle *style);

G_END_DECLS

#endif
