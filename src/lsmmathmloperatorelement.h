/* lsmmathmloperatorelement.h
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

#ifndef LSM_MATHML_OPERATOR_ELEMENT_H
#define LSM_MATHML_OPERATOR_ELEMENT_H

#include <lsmmathml.h>
#include <lsmmathmlpresentationtoken.h>

G_BEGIN_DECLS

#define LSM_TYPE_MATHML_OPERATOR_ELEMENT             (lsm_mathml_operator_element_get_type ())
#define LSM_MATHML_OPERATOR_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_MATHML_OPERATOR_ELEMENT, LsmMathmlOperatorElement))
#define LSM_MATHML_OPERATOR_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_MATHML_OPERATOR_ELEMENT, LsmMathmlOperatorElementClass))
#define LSM_IS_MATHML_OPERATOR_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_MATHML_OPERATOR_ELEMENT))
#define LSM_IS_MATHML_OPERATOR_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_MATHML_OPERATOR_ELEMENT))
#define LSM_MATHML_OPERATOR_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_MATHML_OPERATOR_ELEMENT, LsmMathmlOperatorElementClass))

typedef struct _LsmMathmlOperatorElementClass LsmMathmlOperatorElementClass;

struct _LsmMathmlOperatorElement {
	LsmMathmlPresentationToken presentation_token;

	LsmDomEnumAttribute form;
	LsmDomBooleanAttribute fence;
	LsmDomBooleanAttribute separator;
	LsmMathmlSpaceAttribute left_space;
	LsmMathmlSpaceAttribute right_space;
	LsmDomBooleanAttribute stretchy;
	LsmDomBooleanAttribute symmetric;
	LsmMathmlSpaceAttribute max_size;
	LsmMathmlSpaceAttribute min_size;
	LsmDomBooleanAttribute large_op;
	LsmDomBooleanAttribute movable_limits;
	LsmDomBooleanAttribute accent;

	gboolean is_large_op;
};

struct _LsmMathmlOperatorElementClass {
	LsmMathmlPresentationTokenClass parent_class;
};

GType lsm_mathml_operator_element_get_type (void);

LsmDomNode *	lsm_mathml_operator_element_new 	(void);

double 		lsm_mathml_operator_element_get_slant 	(const LsmMathmlOperatorElement *operator_element,
							 LsmMathmlView *view);

G_END_DECLS

#endif
