#include <gdomdebug.h>
#include <glib/gprintf.h>

static gboolean debug_enabled = FALSE;

void
gdom_debug (char const *format, ...)
{
	va_list args;

	if (!debug_enabled)
		return;

	va_start (args, format);
	g_vprintf (format, args);
	g_printf ("\n");
	va_end (args);
}

void
gdom_debug_enable (void)
{
	debug_enabled = TRUE;
}
