/* gcharacterdata.h
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

#ifndef GDOM_CHARACTER_DATA_H
#define GDOM_CHARACTER_DATA_H

#include <gdom.h>
#include <gdomnode.h>

G_BEGIN_DECLS

#define GDOM_TYPE_CHARACTER_DATA             (gdom_character_data_get_type ())
#define GDOM_CHARACTER_DATA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDOM_TYPE_CHARACTER_DATA, GDomCharacterData))
#define GDOM_CHARACTER_DATA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GDOM_TYPE_CHARACTER_DATA, GDomNodeClass))
#define GDOM_IS_CHARACTER_DATA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDOM_TYPE_CHARACTER_DATA))
#define GDOM_IS_CHARACTER_DATA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDOM_TYPE_CHARACTER_DATA))
#define GDOM_CHARACTER_DATA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GDOM_TYPE_CHARACTER_DATA, GDomCharacterDataClass))

typedef struct _GDomCharacterDataClass GDomCharacterDataClass;

struct _GDomCharacterData
{
	GDomNode node;

	char *data;
};

struct _GDomCharacterDataClass {
	GDomNodeClass parent_class;
};

GType gdom_character_data_get_type (void);

char* gdom_character_data_get_data (GDomCharacterData* self);
void gdom_character_data_set_data (GDomCharacterData* self, const char* value);

G_END_DECLS

#endif

