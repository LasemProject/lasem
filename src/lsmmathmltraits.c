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
