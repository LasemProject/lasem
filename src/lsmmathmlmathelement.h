/* lsmmathmlmathelement.h
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

#ifndef LSM_MATHML_MATH_ELEMENT_H
#define LSM_MATHML_MATH_ELEMENT_H

#include <lsmmathml.h>
#include <lsmmathmlelement.h>

G_BEGIN_DECLS

#define LSM_MATHML_FONT_SERIF		"Serif"
#define LSM_MATHML_FONT_SANS		"Sans"
#define LSM_MATHML_FONT_MONOSPACE		"Monospace"
#define LSM_MATHML_FONT_FRAKTUR		"Serif"
#define LSM_MATHML_FONT_SCRIPT		"cmsy10"
#define LSM_MATHML_FONT_DOUBLE_STRUCK	"msbm10"

#define LSM_TYPE_MATHML_MATH_ELEMENT             (lsm_mathml_math_element_get_type ())
#define LSM_MATHML_MATH_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_MATHML_MATH_ELEMENT, LsmMathmlMathElement))
#define LSM_MATHML_MATH_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_MATHML_MATH_ELEMENT, LsmMathmlMathElementClass))
#define LSM_IS_MATHML_MATH_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_MATHML_MATH_ELEMENT))
#define LSM_IS_MATHML_MATH_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_MATHML_MATH_ELEMENT))
#define LSM_MATHML_MATH_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_MATHML_MATH_ELEMENT, LsmMathmlMathElementClass))

typedef struct _GMathmlMathElementClass LsmMathmlMathElementClass;

struct _GMathmlMathElement {
	LsmMathmlElement	element;

	LsmMathmlStyle *default_style;

	LsmDomEnumAttribute mode;
	LsmDomEnumAttribute display;
};

struct _GMathmlMathElementClass {
	LsmMathmlElementClass  parent_class;
};

GType lsm_mathml_math_element_get_type (void);

LsmDomNode *		lsm_mathml_math_element_new 		(void);

LsmMathmlStyle * 	lsm_mathml_math_element_get_default_style 	(LsmMathmlMathElement *math_element);
void 			lsm_mathml_math_element_set_default_style 	(LsmMathmlMathElement *math_element,
									 LsmMathmlStyle *style);

void 			lsm_mathml_math_element_update 		(LsmMathmlMathElement *math_element);
const LsmMathmlBbox * 	lsm_mathml_math_element_measure 	(LsmMathmlMathElement *math_element,
								 LsmMathmlView *view);
void 			lsm_mathml_math_element_layout 		(LsmMathmlMathElement *math_element,
								 LsmMathmlView *view,
								 const LsmMathmlBbox *bbox);
void 			lsm_mathml_math_element_render 		(LsmMathmlMathElement *math_element,
								 LsmMathmlView *view);


G_END_DECLS

#endif
