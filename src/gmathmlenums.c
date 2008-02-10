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

static unsigned int
gmathml_value_from_string (const char *string, const char **strings, unsigned int n_strings)
{
	int i;

	if (string == NULL)
		return 0;

	for (i = 0; i < n_strings; i++)
		if (strcmp (string, strings[i]) == 0)
			return i;

	return 0;
}

static const char *gmathml_mode_strings[] = {
	"display",
	"inline"
};

const char *
gmathml_mode_to_string (GMathmlMode mode)
{
	return gmathml_mode_strings[CLAMP (mode, 0, GMATHML_MODE_INLINE)];
}

GMathmlMode
gmathml_mode_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_mode_strings,
					  G_N_ELEMENTS (gmathml_mode_strings));
}

static const char *gmathml_space_name_strings[] = {
	"errormathspace",
	"veryverythinmathspace",
	"verythinmathspace",
	"thinmathspace",
	"mediummathspace",
	"thickmathspace",
	"verythickmathspace",
	"veryverythickmathspace"
};

const char *
gmathml_space_name_to_string (GMathmlSpaceName space_name)
{
	return gmathml_space_name_strings[CLAMP (space_name, 0, GMATHML_SPACE_NAME_VERY_VERY_THICK)];
}

GMathmlSpaceName
gmathml_space_name_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_space_name_strings,
					  G_N_ELEMENTS (gmathml_space_name_strings));
}

static const char *gmathml_unit_strings[] = {
	"",
	"em",
	"ex",
	"in",
	"cm",
	"mm",
	"pt",
	"px",
	"pc",
	"%"
};

const char *
gmathml_unit_to_string (GMathmlUnit unit)
{
	return gmathml_unit_strings[CLAMP (unit, 0, GMATHML_UNIT_PERCENT)];
}

GMathmlUnit
gmathml_unit_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_unit_strings,
					  G_N_ELEMENTS (gmathml_unit_strings));
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
	"monospace"
};

const char *
gmathml_variant_to_string (GMathmlVariant variant)
{
	return gmathml_variant_strings[CLAMP (variant, 0, GMATHML_VARIANT_MONOSPACE)];
}

GMathmlVariant
gmathml_variant_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_variant_strings,
					  G_N_ELEMENTS (gmathml_variant_strings));
}

static const char *gmathml_form_strings[] = {
	"prefix",
	"postfix",
	"infix"
};

const char *
gmathml_form_to_string (GMathmlForm form)
{
	return gmathml_form_strings[CLAMP (form, 0, GMATHML_FORM_INFIX)];
}

GMathmlForm
gmathml_form_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_form_strings,
					  G_N_ELEMENTS (gmathml_form_strings));
}

static const char *gmathml_row_align_strings[] = {
	"baseline",
	"top",
	"bottom",
	"center",
	"axis"
};

const char *
gmathml_row_align_to_string (GMathmlRowAlign row_align)
{
	return gmathml_row_align_strings[CLAMP (row_align, 0, GMATHML_ROW_ALIGN_AXIS)];
}

GMathmlRowAlign
gmathml_row_align_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_row_align_strings,
					  G_N_ELEMENTS (gmathml_row_align_strings));
}

static const char *gmathml_column_align_strings[] = {
	"center",
	"left",
	"right"
};

const char *
gmathml_column_align_to_string (GMathmlColumnAlign column_align)
{
	return gmathml_column_align_strings[CLAMP (column_align, 0, GMATHML_COLUMN_ALIGN_RIGHT)];
}

GMathmlColumnAlign
gmathml_column_align_from_string (const char *string)
{
	return gmathml_value_from_string (string, gmathml_column_align_strings,
					  G_N_ELEMENTS (gmathml_column_align_strings));
}
