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

#include <lsmsvgstyle.h>
#include <string.h>

LsmSvgStyle *
lsm_svg_style_new (void)
{
	LsmSvgStyle *style;

	style = g_new0 (LsmSvgStyle, 1);
	g_return_val_if_fail (style != NULL, NULL);

	style->text.font_family = g_strdup ("sans");

	return style;
}

void
lsm_svg_style_free (LsmSvgStyle *style)
{
	g_return_if_fail (style != NULL);

	g_free (style->text.font_family);
	g_free (style);
}

LsmSvgStyle *
lsm_svg_style_duplicate (const LsmSvgStyle *from)
{
	LsmSvgStyle *style;

	g_return_val_if_fail (from != NULL, NULL);

	style = lsm_svg_style_new ();
	g_return_val_if_fail (style != NULL, NULL);

	memcpy (style, from, sizeof (LsmSvgStyle));

	style->text.font_family = g_strdup (from->text.font_family);

	return style;
}
