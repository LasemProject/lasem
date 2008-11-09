/* gmathmloperatorelement.h
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef GMATHML_STYLE_ELEMENT_H
#define GMATHML_STYLE_ELEMENT_H

#include <gmathml.h>
#include <gmathmlpresentationcontainer.h>

G_BEGIN_DECLS

#define GMATHML_TYPE_STYLE_ELEMENT             (gmathml_style_element_get_type ())
#define GMATHML_STYLE_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMATHML_TYPE_STYLE_ELEMENT, GMathmlStyleElement))
#define GMATHML_STYLE_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GMATHML_TYPE_STYLE_ELEMENT, GMathmlStyleElementClass))
#define GMATHML_IS_STYLE_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMATHML_TYPE_STYLE_ELEMENT))
#define GMATHML_IS_STYLE_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GMATHML_TYPE_STYLE_ELEMENT))
#define GMATHML_STYLE_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GMATHML_TYPE_STYLE_ELEMENT, GMathmlStyleElementClass))

typedef struct _GMathmlStyleElementClass GMathmlStyleElementClass;

struct _GMathmlStyleElement {
	GMathmlPresentationContainer presentation_container;

	/* Style attributes */

	GMathmlAttributeBoolean display_style;
	GMathmlAttributeScriptLevel script_level;
	GMathmlAttributeDouble script_size_multiplier;
	GMathmlAttributeLength script_min_size;
	GMathmlAttributeLength very_very_thin_math_space;
	GMathmlAttributeLength very_thin_math_space;
	GMathmlAttributeLength thin_math_space;
	GMathmlAttributeLength medium_math_space;
	GMathmlAttributeLength thick_math_space;
	GMathmlAttributeLength very_thick_math_space;
	GMathmlAttributeLength very_very_thick_math_space;

	/* Presentation token attribute */

	GMathmlAttributeNamed math_variant;
	GMathmlAttributeLength math_size;
	GMathmlAttributeColor math_color;
	GMathmlAttributeColor math_background;

	/* Fraction attributes */

	GMathmlAttributeLength line_thickness;
};

struct _GMathmlStyleElementClass {
	GMathmlPresentationContainerClass parent_class;
};

GType gmathml_style_element_get_type (void);

GDomNode *	gmathml_style_element_new 			(void);

G_END_DECLS

#endif

