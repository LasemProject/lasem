/* gdomtext.h
 *
 * Copyright (C) 2007  Emmanuel Pacaud
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

#ifndef GDOM_TEXT_H
#define GDOM_TEXT_H

#include <gdom.h>
#include <gdomcharacterdata.h>

G_BEGIN_DECLS

#define GDOM_TYPE_TEXT             (gdom_text_get_type ())
#define GDOM_TEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_TEXT, GDomText))
#define GDOM_TEXT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_TEXT, GDomTextClass))
#define GDOM_IS_TEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_TEXT))
#define GDOM_IS_TEXT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_TEXT))
#define GDOM_TEXT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_TEXT, GDomTextClass))

typedef struct _GDomTextClass GDomTextClass;

struct _GDomText {
	GDomCharacterData	character_data;
};

struct _GDomTextClass {
	GDomCharacterDataClass  parent_class;
};

GType gdom_text_get_type (void);

GDomNode 	*gdom_text_new 		(const char *data);

G_END_DECLS

#endif

