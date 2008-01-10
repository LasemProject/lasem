/* gmathmlenums.c
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

#include <gmathmlenums.h>
#include <string.h>

static const char *gmathml_named_space_strings[] = {
	"veryverythinmathspace",
	"verythinmathspace",
	"thinmathspace",
	"mediummathspace",
	"thickmathspace",
	"verythickmathspace",
	"veryverythickmathspace",
	"mediummathspace"
};

const char *
gmathml_named_space_to_string (GMathmlNamedSpace named_space)
{
	return gmathml_named_space_strings[CLAMP (named_space,
						  GMATHML_NAMED_SPACE_VERY_VERY_THIN,
						  GMATHML_NAMED_SPACE_ERROR)];
}

GMathmlNamedSpace
gmathml_named_space_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_NAMED_SPACE_ERROR;

	for (i = 0; i < G_N_ELEMENTS (gmathml_named_space_strings) - 1; i++)
		if (strcmp (string, gmathml_named_space_strings[i]) == 0)
			return i;

	return GMATHML_NAMED_SPACE_ERROR;
}

static const char *gmathml_unit_strings[] = {
	"em",
	"ex",
	"in",
	"cm",
	"mm",
	"pt",
	"px",
	"pc",
	"%",
	""
};

const char *
gmathml_unit_to_string (GMathmlUnit unit)
{
	return gmathml_named_space_strings[CLAMP (unit,
						  GMATHML_UNIT_EM,
						  GMATHML_UNIT_NONE)];
}

GMathmlUnit
gmathml_unit_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_UNIT_NONE;

	for (i = 0; i < G_N_ELEMENTS (gmathml_unit_strings) - 1; i++)
		if (strcmp (string, gmathml_unit_strings[i]) == 0)
			return i;

	return GMATHML_UNIT_NONE;
}

static const char *gmathml_variant_strings[] = {
	"normal",
	"bold",
	"italic",
	"bold-italic",
	"double-struck",
	"bold-fraktur",
	"script",
	"bold-script",
	"fraktur",
	"sans-serif",
	"bold-sans-serif",
	"sans-serif-italic",
	"sans-serif-bold-italic",
	"monospace",
	"normal"
};

const char *
gmathml_variant_to_string (GMathmlVariant variant)
{
	return gmathml_variant_strings[CLAMP (variant,
					      GMATHML_VARIANT_NORMAL,
					      GMATHML_VARIANT_ERROR)];
}

GMathmlVariant
gmathml_variant_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_VARIANT_ERROR;

	for (i = 0; i < G_N_ELEMENTS (gmathml_variant_strings) - 1; i++)
		if (strcmp (string, gmathml_variant_strings[i]) == 0)
			return i;

	return GMATHML_VARIANT_ERROR;
}
