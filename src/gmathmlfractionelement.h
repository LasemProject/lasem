/* gmathmlfractionelement.h
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

#ifndef GMATHML_FRACTION_ELEMENT_H
#define GMATHML_FRACTION_ELEMENT_H

#include <gmathml.h>
#include <gmathmlpresentationcontainer.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_FRACTION_ELEMENT             (gmathml_fraction_element_get_type ())
#define GMATHML_FRACTION_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_FRACTION_ELEMENT, GMathmlFractionElement))
#define GMATHML_FRACTION_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_FRACTION_ELEMENT, GMathmlFractionElementClass))
#define GMATHML_IS_FRACTION_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_FRACTION_ELEMENT))
#define GMATHML_IS_FRACTION_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_FRACTION_ELEMENT))
#define GMATHML_FRACTION_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_FRACTION_ELEMENT, GMathmlFractionElementClass))

typedef struct _GMathmlFractionElementClass GMathmlFractionElementClass;

struct _GMathmlFractionElement {
	GMathmlElement presentation_container;

	GMathmlAttributeLength line_thickness;
	GMathmlAttributeBoolean bevelled;

	double offset;
	double v_space;
	double h_space;
};

struct _GMathmlFractionElementClass {
	GMathmlElementClass parent_class;
};

GType gmathml_fraction_element_get_type (void);

GDomNode *gmathml_fraction_element_new (void);

G_END_DECLS

#endif

