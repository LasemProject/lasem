#ifndef LSM_DEBUG_H
#define LSM_DEBUG_H

#include <glib.h>

G_BEGIN_DECLS

void 		lsm_debug 			(const char *domain, char const *format, ...);
gboolean 	lsm_debug_check 		(const char *domain);
void 		lsm_debug_enable 		(const char *domains);

G_END_DECLS

#endif
