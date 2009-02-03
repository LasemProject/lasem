/* gmathmlfencedelement.c
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

#include <gmathmlfencedelement.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GDomNode implementation */

static const char *
gmathml_fenced_element_get_node_name (GDomNode *node)
{
	return "mfenced";
}

/* GMathmlElement implementation */

static void
gmathml_fenced_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlFencedElement *fenced = GMATHML_FENCED_ELEMENT (self);
	char *default_string;

	default_string = g_strdup ("(");
	gdom_attribute_string_parse (&fenced->open, &default_string);
	g_free (default_string);

	default_string = g_strdup (")");
	gdom_attribute_string_parse (&fenced->close, &default_string);
	g_free (default_string);

	default_string = g_strdup (",");
	gdom_attribute_string_parse (&fenced->separators, &default_string);
	g_free (default_string);
}

static const GMathmlBbox *
gmathml_fenced_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *bbox)
{
	GMathmlFencedElement *fenced = GMATHML_FENCED_ELEMENT (self);
	GMathmlBbox stretch_bbox = gmathml_bbox_null;
	double axis_offset;
	double spacing = 0.0;
	char *open;
	char *close;

	GMATHML_ELEMENT_CLASS (parent_class)->measure (self, view, bbox);

	stretch_bbox = self->bbox;
	open = fenced->open.value;
	close = fenced->close.value;
	axis_offset = gmathml_view_measure_axis_offset (view, self->style.math_size);

	if (open != NULL && open[0] != '\0') {
		gmathml_view_measure_operator (view, &self->style,
					       open, FALSE, TRUE,
					       axis_offset,
					       &stretch_bbox, &fenced->open_bbox);
		spacing += self->style.math_size * GMATHML_SPACE_EM_THIN;
	} else
		fenced->open_bbox = gmathml_bbox_null;

	if (close != NULL && close[0] != '\0') {
		gmathml_view_measure_operator (view, &self->style,
					       close, FALSE, TRUE,
					       axis_offset,
					       &stretch_bbox, &fenced->close_bbox);
		spacing += self->style.math_size * GMATHML_SPACE_EM_THIN;
	} else
		fenced->close_bbox = gmathml_bbox_null;

	gmathml_bbox_add_horizontally (&self->bbox, &fenced->open_bbox);
	gmathml_bbox_add_horizontally (&self->bbox, &fenced->close_bbox);

	self->bbox.width += spacing;

	return &self->bbox;
}

static void
gmathml_fenced_element_layout (GMathmlElement *self, GMathmlView *view,
			       double x, double y, const GMathmlBbox *bbox)
{
	GMathmlFencedElement *fenced = GMATHML_FENCED_ELEMENT (self);
	double offset;

	offset = (fenced->open.value != NULL && fenced->open.value[0]) ?
		self->style.math_size * GMATHML_SPACE_EM_THIN : 0.0;

	GMATHML_ELEMENT_CLASS (parent_class)->layout (self, view,
						      offset + x + fenced->open_bbox.width,
						      y, bbox);
}

static void
gmathml_fenced_element_render (GMathmlElement *self, GMathmlView *view)
{
	GMathmlFencedElement *fenced = GMATHML_FENCED_ELEMENT (self);

	gmathml_view_show_operator (view, &self->style,
				    self->x, self->y, fenced->open.value,
				    FALSE, &fenced->open_bbox);

	GMATHML_ELEMENT_CLASS (parent_class)->render (self, view);

	gmathml_view_show_operator (view, &self->style,
				    self->bbox.width + self->x - fenced->close_bbox.width,
				    self->y, fenced->close.value,
				    FALSE, &fenced->close_bbox);
}

/* GMathmlFencedElement implementation */

GDomNode *
gmathml_fenced_element_new (void)
{
	return g_object_new (GMATHML_TYPE_FENCED_ELEMENT, NULL);
}

static void
gmathml_fenced_element_init (GMathmlFencedElement *element)
{
}

/* GMathmlFencedElement class */

static void
gmathml_fenced_element_class_init (GMathmlFencedElementClass *m_fenced_element_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (m_fenced_element_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (m_fenced_element_class);

	parent_class = g_type_class_peek_parent (m_fenced_element_class);

	d_node_class->get_node_name = gmathml_fenced_element_get_node_name;

	m_element_class->update =  gmathml_fenced_element_update;
	m_element_class->measure = gmathml_fenced_element_measure;
	m_element_class->layout =  gmathml_fenced_element_layout;
	m_element_class->render =  gmathml_fenced_element_render;

	m_element_class->attributes = gdom_attribute_map_duplicate (m_element_class->attributes);

	gdom_attribute_map_add_string (m_element_class->attributes, "open",
				       offsetof (GMathmlFencedElement, open));
	gdom_attribute_map_add_string (m_element_class->attributes, "close",
				       offsetof (GMathmlFencedElement, close));
	gdom_attribute_map_add_string (m_element_class->attributes, "separators",
				       offsetof (GMathmlFencedElement, separators));
}

G_DEFINE_TYPE (GMathmlFencedElement, gmathml_fenced_element, GMATHML_TYPE_PRESENTATION_CONTAINER)

