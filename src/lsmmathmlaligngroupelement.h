/*
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

#ifndef LSM_MATHML_ALIGN_GROUP_ELEMENT_H
#define LSM_MATHML_ALIGN_GROUP_ELEMENT_H

#include <lsmmathml.h>
#include <lsmmathmlelement.h>

G_BEGIN_DECLS

#define LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT             (lsm_mathml_align_group_element_get_type ())
#define LSM_MATHML_ALIGN_GROUP_ELEMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT, LsmMathmlAlignGroupElement))
#define LSM_MATHML_ALIGN_GROUP_ELEMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT, LsmMathmlAlignGroupElementClass))
#define LSM_MATHML_IS_ALIGN_GROUP_ELEMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT))
#define LSM_MATHML_IS_ALIGN_GROUP_ELEMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT))
#define LSM_MATHML_ALIGN_GROUP_ELEMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_MATHML_TYPE_ALIGN_GROUP_ELEMENT, LsmMathmlAlignGroupElementClass))

typedef struct _GMathmlAlignGroupElementClass LsmMathmlAlignGroupElementClass;

struct _GMathmlAlignGroupElement {
	LsmMathmlElement	element;
};

struct _GMathmlAlignGroupElementClass {
	LsmMathmlElementClass  parent_class;
};

GType lsm_mathml_align_group_element_get_type (void);

LsmDomNode * 	lsm_mathml_align_group_element_new 	(void);

G_END_DECLS

#endif

