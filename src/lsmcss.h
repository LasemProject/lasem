/* Lasem
 *
 * Copyright © 20015 Emmanuel Pacaud
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

#ifndef LSM_CSS_H
#define LSM_CSS_H

#include <lsmtypes.h>

G_BEGIN_DECLS

typedef enum {
	LSM_CSS_SPECIFITY_NAME_OR_PSEUDO_ELEMENT = 0x0001,
	LSM_CSS_SPECIFITY_ATTRIBUTE_OR_PSEUDO_CLASS = 0x0010,
	LSM_CSS_SPECIFITY_ID = 0x0100,
	LSM_CSS_SPECIFITY_INLINE_STYLE = 0x1000
} LsmCssSpecifity;

#define LSM_TYPE_CSS             (lsm_css_get_type ())
#define LSM_CSS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_CSS, LsmCss))
#define LSM_CSS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_CSS, LsmCssClass))
#define LSM_IS_CSS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_CSS))
#define LSM_IS_CSS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_CSS))
#define LSM_CSS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_CSS, LsmCssClass))

typedef struct _LsmCssPrivate LsmCssPrivate;
typedef struct _LsmCssClass LsmCssClass;

struct _LsmCss {
	GObject	object;

	LsmCssPrivate *priv;
};

struct _LsmCssClass {
	GObjectClass parent_class;
};

GType lsm_css_get_type (void);

LsmCss * 		lsm_css_new_from_memory 	(const void *buffer, gssize size, GError **error);
LsmCss * 		lsm_css_new_from_path 		(const char *path, GError **error);
LsmCss * 		lsm_css_new_from_url 		(const char *url, GError **error);

G_END_DECLS

#endif
