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

#include <pango/pango-attributes.h>
#include <lsmmathmltraits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

static void
lsm_mathml_boolean_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	gboolean *value = (gboolean *) abstract_trait;

	*value = (g_strcmp0 (string, "true") == 0);
}

static char *
lsm_mathml_boolean_trait_to_string (LsmTrait *abstract_trait)
{
	gboolean *value = (gboolean *) abstract_trait;

	return g_strdup_printf ("%s", *value ? "true" : "false");
}

const LsmTraitClass lsm_mathml_boolean_trait_class = {
	.size = sizeof (gboolean),
	.from_string = lsm_mathml_boolean_trait_from_string,
	.to_string = lsm_mathml_boolean_trait_to_string
};

static void
lsm_mathml_unsigned_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	unsigned int *value = (unsigned int *) abstract_trait;

	*value = atoi (string);
}

static char *
lsm_mathml_unsigned_trait_to_string (LsmTrait *abstract_trait)
{
	unsigned int *value = (unsigned int *) abstract_trait;

	return g_strdup_printf ("%u", *value);
}

const LsmTraitClass lsm_mathml_unsigned_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_unsigned_trait_from_string,
	.to_string = lsm_mathml_unsigned_trait_to_string
};

static void
lsm_mathml_display_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlDisplay *value = (LsmMathmlDisplay *) abstract_trait;

	*value = lsm_mathml_display_from_string (string);
}

static char *
lsm_mathml_display_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlDisplay *value = (LsmMathmlDisplay *) abstract_trait;

	return g_strdup (lsm_mathml_display_to_string (*value));
}

const LsmTraitClass lsm_mathml_display_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_display_trait_from_string,
	.to_string = lsm_mathml_display_trait_to_string
};

static void
lsm_mathml_mode_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlMode *value = (LsmMathmlMode *) abstract_trait;

	*value = lsm_mathml_mode_from_string (string);
}

static char *
lsm_mathml_mode_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlMode *value = (LsmMathmlMode *) abstract_trait;

	return g_strdup (lsm_mathml_mode_to_string (*value));
}

const LsmTraitClass lsm_mathml_mode_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_mode_trait_from_string,
	.to_string = lsm_mathml_mode_trait_to_string
};

static void
lsm_mathml_line_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlLine *value = (LsmMathmlLine *) abstract_trait;

	*value = lsm_mathml_line_from_string (string);
}

static char *
lsm_mathml_line_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlLine *value = (LsmMathmlLine *) abstract_trait;

	return g_strdup (lsm_mathml_line_to_string (*value));
}

const LsmTraitClass lsm_mathml_line_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_line_trait_from_string,
	.to_string = lsm_mathml_line_trait_to_string
};

static void
lsm_mathml_font_style_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlFontStyle *value = (LsmMathmlFontStyle *) abstract_trait;

	*value = lsm_mathml_font_style_from_string (string);
}

static char *
lsm_mathml_font_style_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlFontStyle *value = (LsmMathmlFontStyle *) abstract_trait;

	return g_strdup (lsm_mathml_font_style_to_string (*value));
}

const LsmTraitClass lsm_mathml_font_style_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_font_style_trait_from_string,
	.to_string = lsm_mathml_font_style_trait_to_string
};

static void
lsm_mathml_font_weight_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlFontWeight *value = (LsmMathmlFontWeight *) abstract_trait;

	*value = lsm_mathml_font_weight_from_string (string);
}

static char *
lsm_mathml_font_weight_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlFontWeight *value = (LsmMathmlFontWeight *) abstract_trait;

	return g_strdup (lsm_mathml_font_weight_to_string (*value));
}

const LsmTraitClass lsm_mathml_font_weight_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_font_weight_trait_from_string,
	.to_string = lsm_mathml_font_weight_trait_to_string
};

static void
lsm_mathml_variant_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlVariant *value = (LsmMathmlVariant *) abstract_trait;

	*value = lsm_mathml_variant_from_string (string);
}

static char *
lsm_mathml_variant_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlVariant *value = (LsmMathmlVariant *) abstract_trait;

	return g_strdup (lsm_mathml_variant_to_string (*value));
}

const LsmTraitClass lsm_mathml_variant_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_variant_trait_from_string,
	.to_string = lsm_mathml_variant_trait_to_string
};

static void
lsm_mathml_form_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlForm *value = (LsmMathmlForm *) abstract_trait;

	*value = lsm_mathml_form_from_string (string);
}

static char *
lsm_mathml_form_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlForm *value = (LsmMathmlForm *) abstract_trait;

	return g_strdup (lsm_mathml_form_to_string (*value));
}

const LsmTraitClass lsm_mathml_form_trait_class = {
	.size = sizeof (unsigned),
	.from_string = lsm_mathml_form_trait_from_string,
	.to_string = lsm_mathml_form_trait_to_string
};

static void
lsm_mathml_double_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	double *value = (double *) abstract_trait;

	*value = atof (string);
}

static char *
lsm_mathml_double_trait_to_string (LsmTrait *abstract_trait)
{
	double *value = (double *) abstract_trait;

	return g_strdup_printf ("%g", *value);
}

const LsmTraitClass lsm_mathml_double_trait_class = {
	.size = sizeof (double),
	.from_string = lsm_mathml_double_trait_from_string,
	.to_string = lsm_mathml_double_trait_to_string
};

static LsmMathmlColor *
lsm_mathml_color_copy (LsmMathmlColor *color)
{
	LsmMathmlColor *copy;

	copy = g_new (LsmMathmlColor, 1);
	memcpy (copy, color, sizeof (LsmMathmlColor));

	return copy;
}

GType
lsm_mathml_color_get_type (void)
{
	static GType our_type = 0;
	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlColor",
			 (GBoxedCopyFunc) lsm_mathml_color_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

static void
lsm_mathml_color_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlColor *color = (LsmMathmlColor *) abstract_trait;

	if (strcmp (string, "transparent") == 0) {
		color->red = 0.0;
		color->green = 0.0;
		color->blue = 0.0;
		color->alpha = 0.0;
	} else {
		PangoColor pango_color;

		pango_color_parse (&pango_color, string);
		color->alpha = 1.0;
		color->red = pango_color.red / 65535.0;
		color->green = pango_color.green / 65535.0;
		color->blue = pango_color.blue / 65535.0;
	}
}

static char *
lsm_mathml_color_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlColor *color = (LsmMathmlColor *) abstract_trait;
	PangoColor pango_color;

	if (color->alpha <= 0.0)
		return g_strdup ("transparent");

	pango_color.red = ((int) ((double) 0.5 + 65535.0 * color->red));
	pango_color.blue = ((int) ((double) 0.5 + 65535.0 * color->blue));
	pango_color.green = ((int) ((double) 0.5 + 65535.0 * color->green));

	return pango_color_to_string (&pango_color);
}

const LsmTraitClass lsm_mathml_color_trait_class = {
	.size = sizeof (LsmMathmlColor),
	.from_string = lsm_mathml_color_trait_from_string,
	.to_string = lsm_mathml_color_trait_to_string
};

static void
lsm_mathml_string_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	char **value = (char **) abstract_trait;

	g_free (*value);
	*value = g_strdup (string);
}

static char *
lsm_mathml_string_trait_to_string (LsmTrait *abstract_trait)
{
	char **value = (char **) abstract_trait;

	return g_strdup (*value);
}

static void
lsm_mathml_string_trait_finalize (LsmTrait *abstract_trait)
{
	char **value = (char **) abstract_trait;

	g_free (*value);
	*value = NULL;
}

const LsmTraitClass lsm_mathml_string_trait_class = {
	.size = sizeof (char *),
	.from_string = lsm_mathml_string_trait_from_string,
	.to_string = lsm_mathml_string_trait_to_string,
	.finalize = lsm_mathml_string_trait_finalize
};

static LsmMathmlLength *
lsm_mathml_length_copy (LsmMathmlLength *length)
{
	LsmMathmlLength *copy;

	copy = g_new (LsmMathmlLength, 1);
	memcpy (copy, length, sizeof (LsmMathmlLength));

	return copy;
}

GType
lsm_mathml_length_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlLength",
			 (GBoxedCopyFunc) lsm_mathml_length_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

static void
lsm_mathml_length_trait_from_string (LsmTrait *abstract_trait, char *string)
{
	LsmMathmlLength *length = (LsmMathmlLength *) abstract_trait;
	char *unit_str;

	length->value = g_strtod (string, &unit_str);
	length->unit = lsm_mathml_unit_from_string (unit_str);

	/* TODO Handle "big", "small", normal" sizes */
}

static char *
lsm_mathml_length_trait_to_string (LsmTrait *abstract_trait)
{
	LsmMathmlLength *length = (LsmMathmlLength *) abstract_trait;

	return g_strdup_printf ("%g %s", length->value,
				lsm_mathml_unit_to_string (length->unit));
}

const LsmTraitClass lsm_mathml_length_trait_class = {
	.size = sizeof (char *),
	.from_string = lsm_mathml_length_trait_from_string,
	.to_string = lsm_mathml_length_trait_to_string
};

double
lsm_mathml_length_normalize (const LsmMathmlLength *length, double default_value, double font_size)
{
	double value;

	g_return_val_if_fail (length != NULL, 0.0);

	switch (length->unit) {
		case LSM_MATHML_UNIT_PX:
		case LSM_MATHML_UNIT_PT:
			value = length->value;
			break;
		case LSM_MATHML_UNIT_PC:
			value = length->value * 72.0 / 6.0;
			break;
		case LSM_MATHML_UNIT_CM:
			value = length->value * 72.0 / 2.54;
			break;
		case LSM_MATHML_UNIT_MM:
			value = length->value * 72.0 / 25.4;
			break;
		case LSM_MATHML_UNIT_IN:
			value = length->value * 72.0;
			break;
		case LSM_MATHML_UNIT_EM:
			value = length->value * font_size;
			break;
		case LSM_MATHML_UNIT_EX:
			value = length->value * font_size * 0.5;
			break;
		case LSM_MATHML_UNIT_PERCENT:
			value = default_value * length->value / 100.0;
			break;
		case LSM_MATHML_UNIT_NONE:
			value = default_value * length->value;
			break;
		default:
			value = 0;
	}

	return value;
}

static LsmMathmlSpace *
lsm_mathml_space_copy (LsmMathmlSpace *space)
{
	LsmMathmlSpace *copy;

	copy = g_new (LsmMathmlSpace, 1);
	memcpy (copy, space, sizeof (LsmMathmlSpace));

	return copy;
}

GType
lsm_mathml_space_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlSpace",
			 (GBoxedCopyFunc) lsm_mathml_space_copy,
			 (GBoxedFreeFunc) g_free);
	return our_type;
}

GType
lsm_mathml_space_list_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static
			("LsmMathmlSpaceList",
			 (GBoxedCopyFunc) lsm_mathml_space_list_duplicate,
			 (GBoxedFreeFunc) lsm_mathml_space_list_free);
	return our_type;
}

LsmMathmlSpaceList *
lsm_mathml_space_list_new (unsigned int n_spaces)
{
	LsmMathmlSpaceList *space_list;

	space_list = g_new (LsmMathmlSpaceList, 1);
	if (space_list == NULL)
		return NULL;

	space_list->n_spaces = n_spaces;

	if (n_spaces > 0) {
		space_list->spaces = g_new (LsmMathmlSpace, n_spaces);

		if (space_list->spaces == NULL) {
			g_free (space_list);
			return NULL;
		}
	} else
		space_list->spaces = NULL;

	return space_list;
}

void
lsm_mathml_space_list_free (LsmMathmlSpaceList *space_list)
{
	if (space_list == NULL)
		return;

	space_list->n_spaces = 0;

	g_free (space_list->spaces);
	g_free (space_list);
}

LsmMathmlSpaceList *
lsm_mathml_space_list_duplicate (const LsmMathmlSpaceList *space_list)
{
	LsmMathmlSpaceList *new_space_list;

	g_return_val_if_fail (space_list != NULL, NULL);

	new_space_list = lsm_mathml_space_list_new (space_list->n_spaces);
	memcpy (new_space_list->spaces, space_list->spaces,
		sizeof (LsmMathmlSpace) * space_list->n_spaces);

	return new_space_list;
}

