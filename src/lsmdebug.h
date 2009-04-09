#ifndef LSM_DEBUG_H
#define LSM_DEBUG_H

#include <glib.h>

G_BEGIN_DECLS

void lsm_debug 		(char const *format, ...);
void lsm_debug_enable 	(void);

G_END_DECLS

#endif
