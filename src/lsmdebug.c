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
