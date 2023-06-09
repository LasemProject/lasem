#include <lsmmisc.h>

char *
lsm_g_string_free_and_steal (GString *string)
{
#if GLIB_CHECK_VERSION(2,75,4)
        return g_string_free_and_steal(string);
#else
        char *buffer = string->str;

        g_string_free (string, FALSE);

        return buffer;
#endif
}
