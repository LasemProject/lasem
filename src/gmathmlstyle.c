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

#include <gmathmlstyle.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

GMathmlStyle *
gmathml_style_new (void)
{
	GMathmlStyle *style = g_new0 (GMathmlStyle, 1);

	return style;
}

void
gmathml_style_free (GMathmlStyle *style)
{
	g_return_if_fail (style != NULL);

	g_free (style->math_family);
	g_free (style);
}

GMathmlStyle *
gmathml_style_duplicate (const GMathmlStyle *from)
{
	GMathmlStyle *style;

	g_return_val_if_fail (from != NULL, NULL);

	style = g_new (GMathmlStyle, 1);
	g_return_val_if_fail (style != NULL, NULL);

	memcpy (style, from, sizeof (GMathmlStyle));

	style->math_family = g_strdup (from->math_family);

	return style;
}

void
gmathml_style_change_script_level (GMathmlStyle *style, int increment)
{
	g_return_if_fail (style != NULL);

	style->math_size_value = style->math_size_value * pow (style->script_size_multiplier, increment);
	style->script_level += increment;

	if (style->math_size_value < style->script_min_size.value)
		style->math_size_value = style->script_min_size.value;

	style->math_size.value = style->math_size_value;
	style->math_size.unit = GMATHML_UNIT_PT;
}

void
gmathml_style_set_math_size_pt (GMathmlStyle *style, double math_size)
{
	g_return_if_fail (style != NULL);

	style->math_size_value = math_size;
	style->math_size.value = math_size;
	style->math_size.unit = GMATHML_UNIT_PT;
}

void
gmathml_style_set_math_family (GMathmlStyle *style, const char *math_family)
{
	g_return_if_fail (style != NULL);
	g_return_if_fail (math_family != NULL);

	g_free (style->math_family);
	style->math_family = g_strdup (math_family);
}

void
gmathml_style_set_math_variant (GMathmlStyle *style, GMathmlVariant math_variant)
{
	g_return_if_fail (style != NULL);

	style->math_variant = math_variant;
}

void
gmathml_style_set_math_color (GMathmlStyle *style, double red, double green, double blue, double alpha)
{
	g_return_if_fail (style != NULL);

	style->math_color.red = red;
	style->math_color.green = green;
	style->math_color.blue = blue;
	style->math_color.alpha = alpha;
}
