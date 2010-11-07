/* Lasem - SVG and Mathml library
 *
 * Copyright © 2007-2008 Emmanuel Pacaud
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

#ifndef LSM_DOM_TEXT_H
#define LSM_DOM_TEXT_H

#include <lsmdomcharacterdata.h>

G_BEGIN_DECLS

#define LSM_TYPE_DOM_TEXT             (lsm_dom_text_get_type ())
#define LSM_DOM_TEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_TEXT, LsmDomText))
#define LSM_DOM_TEXT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_TEXT, LsmDomTextClass))
#define LSM_IS_DOM_TEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_TEXT))
#define LSM_IS_DOM_TEXT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_TEXT))
#define LSM_DOM_TEXT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_TEXT, LsmDomTextClass))

typedef struct _LsmDomTextClass LsmDomTextClass;

struct _LsmDomText {
	LsmDomCharacterData	character_data;
};

struct _LsmDomTextClass {
	LsmDomCharacterDataClass  parent_class;
};

GType lsm_dom_text_get_type (void);

LsmDomNode 	*lsm_dom_text_new 		(const char *data);

G_END_DECLS

#endif

