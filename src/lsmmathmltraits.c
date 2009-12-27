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
