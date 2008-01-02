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

static const GMathmlBbox *
gmathml_presentation_container_measure (GMathmlElement *self, GMathmlView *view)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;
	gboolean is_set = FALSE;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node)) {
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
			if (is_set)
				gmathml_bbox_add_to_right (&self->bbox, child_bbox);
			else {
				self->bbox = *child_bbox;
					is_set = TRUE;
			}
		}
	}

	if (!is_set) {
		self->bbox.width = 0.0;
		self->bbox.ascent = 0.0;
		self->bbox.descent = 0.0;
	}

	return &self->bbox;
}

static void
gmathml_presentation_container_layout (GMathmlElement *self, GMathmlView *view)
{
}

static void
gmathml_presentation_container_render (GMathmlElement *self, GMathmlView *view)
{
	GDomNode *node;
	const GMathmlBbox *child_bbox;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GMATHML_IS_ELEMENT (node)) {
			gmathml_element_render (GMATHML_ELEMENT (node), view);
			child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view);
			gmathml_view_rel_move_to (view, child_bbox->width, 0);
		}
}

static void
gmathml_presentation_container_init (GMathmlPresentationContainer *container)
{
}

static void
gmathml_presentation_container_class_init (GMathmlPresentationContainerClass *klass)
{
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (klass);

	m_element_class->measure = gmathml_presentation_container_measure;
	m_element_class->layout = gmathml_presentation_container_layout;
	m_element_class->render = gmathml_presentation_container_render;
}

G_DEFINE_ABSTRACT_TYPE (GMathmlPresentationContainer, gmathml_presentation_container, GMATHML_TYPE_ELEMENT)
