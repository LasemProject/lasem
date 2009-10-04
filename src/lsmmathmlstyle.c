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

#include <lsmmathmlstyle.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

GType
lsm_mathml_style_get_type (void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("LsmMathmlStyle",
                                                (GBoxedCopyFunc) lsm_mathml_style_duplicate,
                                                (GBoxedFreeFunc) lsm_mathml_style_free);
    return our_type;
}

LsmMathmlStyle *
lsm_mathml_style_new (void)
{
	LsmMathmlStyle *style = g_new0 (LsmMathmlStyle, 1);

	return style;
}

void
lsm_mathml_style_free (LsmMathmlStyle *style)
{
	g_return_if_fail (style != NULL);

	g_free (style->math_family);
	g_free (style);
}

LsmMathmlStyle *
lsm_mathml_style_duplicate (const LsmMathmlStyle *from)
{
	LsmMathmlStyle *style;

	g_return_val_if_fail (from != NULL, NULL);

	style = g_new (LsmMathmlStyle, 1);
	g_return_val_if_fail (style != NULL, NULL);

	memcpy (style, from, sizeof (LsmMathmlStyle));

	style->math_family = g_strdup (from->math_family);

	return style;
}

void
lsm_mathml_style_change_script_level (LsmMathmlStyle *style, int increment)
{
	g_return_if_fail (style != NULL);

	style->math_size_value = style->math_size_value * pow (style->script_size_multiplier, increment);
	style->script_level += increment;

	if (style->math_size_value < style->script_min_size.value)
		style->math_size_value = style->script_min_size.value;

	style->math_size.value = style->math_size_value;
	style->math_size.unit = LSM_MATHML_UNIT_PT;
}

void
lsm_mathml_style_set_math_size_pt (LsmMathmlStyle *style, double math_size)
{
	g_return_if_fail (style != NULL);

	style->math_size_value = math_size;
	style->math_size.value = math_size;
	style->math_size.unit = LSM_MATHML_UNIT_PT;
}

void
lsm_mathml_style_set_math_family (LsmMathmlStyle *style, const char *math_family)
{
	g_return_if_fail (style != NULL);
	g_return_if_fail (math_family != NULL);

	g_free (style->math_family);
	style->math_family = g_strdup (math_family);
}

void
lsm_mathml_style_set_math_variant (LsmMathmlStyle *style, LsmMathmlVariant math_variant)
{
	g_return_if_fail (style != NULL);

	style->math_variant = math_variant;
}

void
lsm_mathml_style_set_math_color (LsmMathmlStyle *style, double red, double green, double blue, double alpha)
{
	g_return_if_fail (style != NULL);

	style->math_color.red = red;
	style->math_color.green = green;
	style->math_color.blue = blue;
	style->math_color.alpha = alpha;
}
