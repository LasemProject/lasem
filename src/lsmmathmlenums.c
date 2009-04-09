/* lsmmathmlenums.c
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

#include <lsmmathmlenums.h>
#include <string.h>

static unsigned int
lsm_mathml_value_from_string (const char *string, const char **strings, unsigned int n_strings)
{
	int i;

	if (string == NULL)
		return 0;

	for (i = 0; i < n_strings; i++)
		if (strcmp (string, strings[i]) == 0)
			return i;

	return 0;
}

static const char *lsm_mathml_mode_strings[] = {
	"display",
	"inline"
};

const char *
lsm_mathml_mode_to_string (LsmMathmlMode mode)
{
	return lsm_mathml_mode_strings[CLAMP (mode, 0, LSM_MATHML_MODE_INLINE)];
}

LsmMathmlMode
lsm_mathml_mode_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_mode_strings,
					  G_N_ELEMENTS (lsm_mathml_mode_strings));
}

static const char *lsm_mathml_display_strings[] = {
	"block",
	"inline"
};

const char *
lsm_mathml_display_to_string (LsmMathmlDisplay display)
{
	return lsm_mathml_display_strings[CLAMP (display, 0, LSM_MATHML_DISPLAY_INLINE)];
}

LsmMathmlDisplay
lsm_mathml_display_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_display_strings,
					  G_N_ELEMENTS (lsm_mathml_display_strings));
}

static const char *lsm_mathml_space_name_strings[] = {
	"errormathspace",
	"veryverythinmathspace",
	"verythinmathspace",
	"thinmathspace",
	"mediummathspace",
	"thickmathspace",
	"verythickmathspace",
	"veryverythickmathspace",
	"infinity"
};

const char *
lsm_mathml_space_name_to_string (LsmMathmlSpaceName space_name)
{
	return lsm_mathml_space_name_strings[CLAMP (space_name, 0, LSM_MATHML_SPACE_NAME_INFINITY)];
}

LsmMathmlSpaceName
lsm_mathml_space_name_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_space_name_strings,
					  G_N_ELEMENTS (lsm_mathml_space_name_strings));
}

static const char *lsm_mathml_unit_strings[] = {
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
lsm_mathml_unit_to_string (LsmMathmlUnit unit)
{
	return lsm_mathml_unit_strings[CLAMP (unit, 0, LSM_MATHML_UNIT_PERCENT)];
}

LsmMathmlUnit
lsm_mathml_unit_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_unit_strings,
					  G_N_ELEMENTS (lsm_mathml_unit_strings));
}

static const char *lsm_mathml_font_style_strings[] = {
	"normal",
	"italic",
};

LsmMathmlFontStyle
lsm_mathml_font_style_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_font_style_strings,
					  G_N_ELEMENTS (lsm_mathml_font_style_strings));
}

static const char *lsm_mathml_font_weight_strings[] = {
	"normal",
	"bold",
};

LsmMathmlFontWeight
lsm_mathml_font_weight_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_font_weight_strings,
					  G_N_ELEMENTS (lsm_mathml_font_weight_strings));
}

static const char *lsm_mathml_variant_strings[] = {
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
lsm_mathml_variant_to_string (LsmMathmlVariant variant)
{
	return lsm_mathml_variant_strings[CLAMP (variant, 0, LSM_MATHML_VARIANT_MONOSPACE)];
}

LsmMathmlVariant
lsm_mathml_variant_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_variant_strings,
					  G_N_ELEMENTS (lsm_mathml_variant_strings));
}

void
lsm_mathml_variant_set_font_style (LsmMathmlVariant *variant, LsmMathmlFontStyle style)
{
	if (variant == NULL)
		return;

	switch (style) {
		case LSM_MATHML_FONT_STYLE_ITALIC:
			switch (*variant) {
				case LSM_MATHML_VARIANT_NORMAL:
					*variant = LSM_MATHML_VARIANT_ITALIC;
					return;
				case LSM_MATHML_VARIANT_BOLD:
					*variant = LSM_MATHML_VARIANT_BOLD_ITALIC;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_ITALIC;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC;
					return;
				case LSM_MATHML_VARIANT_ITALIC:
				case LSM_MATHML_VARIANT_BOLD_ITALIC:
				case LSM_MATHML_VARIANT_DOUBLE_STRUCK:
				case LSM_MATHML_VARIANT_FRAKTUR_BOLD:
				case LSM_MATHML_VARIANT_SCRIPT:
				case LSM_MATHML_VARIANT_SCRIPT_BOLD:
				case LSM_MATHML_VARIANT_FRAKTUR:
				case LSM_MATHML_VARIANT_SANS_SERIF_ITALIC:
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC:
				case LSM_MATHML_VARIANT_MONOSPACE:
				case LSM_MATHML_VARIANT_ERROR:
					return;
			}
		case LSM_MATHML_FONT_WEIGHT_NORMAL:
			switch (*variant) {
				case LSM_MATHML_VARIANT_ITALIC:
					*variant = LSM_MATHML_VARIANT_NORMAL;
					return;
				case LSM_MATHML_VARIANT_BOLD_ITALIC:
					*variant = LSM_MATHML_VARIANT_BOLD;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_ITALIC:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_BOLD;
					return;
				case LSM_MATHML_VARIANT_BOLD:
				case LSM_MATHML_VARIANT_NORMAL:
				case LSM_MATHML_VARIANT_SANS_SERIF:
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD:
				case LSM_MATHML_VARIANT_DOUBLE_STRUCK:
				case LSM_MATHML_VARIANT_FRAKTUR_BOLD:
				case LSM_MATHML_VARIANT_SCRIPT:
				case LSM_MATHML_VARIANT_SCRIPT_BOLD:
				case LSM_MATHML_VARIANT_FRAKTUR:
				case LSM_MATHML_VARIANT_MONOSPACE:
				case LSM_MATHML_VARIANT_ERROR:
					return;
			}
		default:
			return;
	}
}

void
lsm_mathml_variant_set_font_weight (LsmMathmlVariant *variant, LsmMathmlFontWeight weight)
{
	if (variant == NULL)
		return;

	switch (weight) {
		case LSM_MATHML_FONT_WEIGHT_BOLD:
			switch (*variant) {
				case LSM_MATHML_VARIANT_NORMAL:
					*variant = LSM_MATHML_VARIANT_BOLD;
					return;
				case LSM_MATHML_VARIANT_ITALIC:
					*variant = LSM_MATHML_VARIANT_BOLD_ITALIC;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_BOLD;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_ITALIC:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC;
					return;
				case LSM_MATHML_VARIANT_SCRIPT:
					*variant = LSM_MATHML_VARIANT_SCRIPT_BOLD;
					return;
				case LSM_MATHML_VARIANT_FRAKTUR:
					*variant = LSM_MATHML_VARIANT_FRAKTUR_BOLD;
					return;
				case LSM_MATHML_VARIANT_BOLD:
				case LSM_MATHML_VARIANT_BOLD_ITALIC:
				case LSM_MATHML_VARIANT_DOUBLE_STRUCK:
				case LSM_MATHML_VARIANT_FRAKTUR_BOLD:
				case LSM_MATHML_VARIANT_SCRIPT_BOLD:
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD:
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC:
				case LSM_MATHML_VARIANT_MONOSPACE:
				case LSM_MATHML_VARIANT_ERROR:
					return;
			}
		case LSM_MATHML_FONT_WEIGHT_NORMAL:
			switch (*variant) {
				case LSM_MATHML_VARIANT_BOLD:
					*variant = LSM_MATHML_VARIANT_NORMAL;
					return;
				case LSM_MATHML_VARIANT_BOLD_ITALIC:
					*variant = LSM_MATHML_VARIANT_ITALIC;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF;
					return;
				case LSM_MATHML_VARIANT_SANS_SERIF_BOLD_ITALIC:
					*variant = LSM_MATHML_VARIANT_SANS_SERIF_ITALIC;
					return;
				case LSM_MATHML_VARIANT_FRAKTUR_BOLD:
					*variant = LSM_MATHML_VARIANT_FRAKTUR;
					return;
				case LSM_MATHML_VARIANT_SCRIPT_BOLD:
					*variant = LSM_MATHML_VARIANT_SCRIPT;
					return;
				case LSM_MATHML_VARIANT_ITALIC:
				case LSM_MATHML_VARIANT_NORMAL:
				case LSM_MATHML_VARIANT_SANS_SERIF:
				case LSM_MATHML_VARIANT_SANS_SERIF_ITALIC:
				case LSM_MATHML_VARIANT_DOUBLE_STRUCK:
				case LSM_MATHML_VARIANT_SCRIPT:
				case LSM_MATHML_VARIANT_FRAKTUR:
				case LSM_MATHML_VARIANT_MONOSPACE:
				case LSM_MATHML_VARIANT_ERROR:
					return;
			}
		default:
			return;
	}
}

static const char *lsm_mathml_form_strings[] = {
	"prefix",
	"postfix",
	"infix"
};

const char *
lsm_mathml_form_to_string (LsmMathmlForm form)
{
	return lsm_mathml_form_strings[CLAMP (form, 0, LSM_MATHML_FORM_INFIX)];
}

LsmMathmlForm
lsm_mathml_form_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_form_strings,
					  G_N_ELEMENTS (lsm_mathml_form_strings));
}

static const char *lsm_mathml_row_align_strings[] = {
	"baseline",
	"top",
	"bottom",
	"center",
	"axis"
};

const char *
lsm_mathml_row_align_to_string (LsmMathmlRowAlign row_align)
{
	return lsm_mathml_row_align_strings[CLAMP (row_align, 0, LSM_MATHML_ROW_ALIGN_AXIS)];
}

LsmMathmlRowAlign
lsm_mathml_row_align_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_row_align_strings,
					  G_N_ELEMENTS (lsm_mathml_row_align_strings));
}

static const char *lsm_mathml_column_align_strings[] = {
	"center",
	"left",
	"right"
};

const char *
lsm_mathml_column_align_to_string (LsmMathmlColumnAlign column_align)
{
	return lsm_mathml_column_align_strings[CLAMP (column_align, 0, LSM_MATHML_COLUMN_ALIGN_RIGHT)];
}

LsmMathmlColumnAlign
lsm_mathml_column_align_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_column_align_strings,
					  G_N_ELEMENTS (lsm_mathml_column_align_strings));
}

static const char *lsm_mathml_line_strings[] = {
	"none",
	"solid",
	"dashed"
};

const char *
lsm_mathml_line_to_string (LsmMathmlLine line)
{
	return lsm_mathml_line_strings [CLAMP (line, 0, LSM_MATHML_LINE_DASHED)];
}

LsmMathmlLine
lsm_mathml_line_from_string (const char *string)
{
	return lsm_mathml_value_from_string (string, lsm_mathml_line_strings,
					  G_N_ELEMENTS (lsm_mathml_line_strings));
}
