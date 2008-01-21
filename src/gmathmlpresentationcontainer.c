/* gmathmlpresentationcontainer.c
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

#include <gmathmlpresentationcontainer.h>
#include <gmathmlview.h>

/* GMathmlElement implementation */

/* GMathmlPresentationContainer implementation */

static void
gmathml_presentation_container_init (GMathmlPresentationContainer *container)
{
}

/* GMathmlPresentationContainer class */

static void
gmathml_presentation_container_class_init (GMathmlPresentationContainerClass *klass)
{
}

G_DEFINE_ABSTRACT_TYPE (GMathmlPresentationContainer, gmathml_presentation_container, GMATHML_TYPE_ELEMENT)
