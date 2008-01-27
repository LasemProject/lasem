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

static const char *gmathml_mode_strings[] = {
	"display",
	"inline"
};

const char *
gmathml_mode_to_string (GMathmlMode mode)
{
	return gmathml_mode_strings[CLAMP (mode, GMATHML_MODE_DISPLAY, GMATHML_MODE_INLINE)];
}

GMathmlMode
gmathml_mode_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_MODE_DISPLAY;

	for (i = 0; i < G_N_ELEMENTS (gmathml_mode_strings) - 1; i++)
		if (strcmp (string, gmathml_mode_strings[i]) == 0)
			return i;

	return GMATHML_MODE_DISPLAY;
}

static const char *gmathml_space_name_strings[] = {
	"veryverythinmathspace",
	"verythinmathspace",
	"thinmathspace",
	"mediummathspace",
	"thickmathspace",
	"verythickmathspace",
	"veryverythickmathspace",
	"mediummathspace",
};

const char *
gmathml_space_name_to_string (GMathmlSpaceName space_name)
{
	return gmathml_space_name_strings[CLAMP (space_name,
						 GMATHML_SPACE_NAME_VERY_VERY_THIN,
						 GMATHML_SPACE_NAME_ERROR)];
}

GMathmlSpaceName
gmathml_space_name_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_SPACE_NAME_ERROR;

	for (i = 0; i < G_N_ELEMENTS (gmathml_space_name_strings) - 1; i++)
		if (strcmp (string, gmathml_space_name_strings[i]) == 0)
			return i;

	return GMATHML_SPACE_NAME_ERROR;
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
	return gmathml_unit_strings[CLAMP (unit,
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

static const char *gmathml_form_strings[] = {
	"prefix",
	"postfix",
	"infix",
	"normal"
};

const char *
gmathml_form_to_string (GMathmlForm form)
{
	return gmathml_form_strings[CLAMP (form,
					   GMATHML_FORM_PREFIX,
					   GMATHML_FORM_ERROR)];
}

GMathmlForm
gmathml_form_from_string (const char *string)
{
	int i;

	if (string == NULL)
		return GMATHML_FORM_ERROR;

	for (i = 0; i < G_N_ELEMENTS (gmathml_form_strings) - 1; i++)
		if (strcmp (string, gmathml_form_strings[i]) == 0)
			return i;

	return GMATHML_FORM_ERROR;
}
