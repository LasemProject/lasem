/* Lasem - SVG and Mathml library
 *
 * Copyright © 2010 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmdebug.h>
#include <glib/gprintf.h>
#include <stdlib.h>

static GHashTable *lsm_debug_domains = NULL;

static void
lsm_debug_initialize (const char *debug_var)
{
	if (lsm_debug_domains != NULL)
		return;

	lsm_debug_domains = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

	if (debug_var != NULL) {
		char **domains;
		int i;

		domains = g_strsplit (debug_var, ":", -1);
		for (i = 0; domains[i] != NULL; i++) {
			char *debug_domain;

			debug_domain = g_strdup (domains[i]);
			g_hash_table_insert (lsm_debug_domains, debug_domain, debug_domain);
		}
		g_strfreev (domains);
	}
}

gboolean
lsm_debug_check (const char *domain)
{
	if (domain == NULL)
		return FALSE;

	if (lsm_debug_domains == NULL)
		lsm_debug_initialize (g_getenv ("LSM_DEBUG"));

	return g_hash_table_lookup (lsm_debug_domains, domain) != NULL;
}

void
lsm_debug (const char *domain, char const *format, ...)
{
	va_list args;

	if (!lsm_debug_check (domain))
		return;

	va_start (args, format);
	g_vprintf (format, args);
	g_printf ("\n");
	va_end (args);
}

void
lsm_debug_enable (const char *domains)
{
	lsm_debug_initialize (domains);
}
