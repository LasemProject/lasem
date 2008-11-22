/* gmathmlunderoverelement.c
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

#include <gdomdebug.h>
#include <gmathmlunderoverelement.h>
#include <gmathmloperatorelement.h>
#include <gmathmllayoututils.h>
#include <gmathmlview.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
gmathml_under_over_get_node_name (GDomNode *node)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (node);

	switch (under_over->type) {
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
			return "munder";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
			return "mover";
		case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER:
		default:
			return "munderover";
	}
}

static gboolean
gmathml_under_over_element_can_append_child (GDomNode *self, GDomNode *child)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	if (!GMATHML_ELEMENT (child))
		return FALSE;

	if (self->first_child == NULL ||
	    self->first_child->next_sibling == NULL)
		return TRUE;

	if (under_over->type != GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER)
		return FALSE;

	return (self->first_child->next_sibling->next_sibling == NULL);
}

static void
gmathml_under_over_element_update_child_pointers (GMathmlUnderOverElement *self)
{
	GDomNode *node;

	node = GDOM_NODE (self)->first_child;
	if (node == NULL) {
		self->base = NULL;
		self->underscript = NULL;
		self->overscript = NULL;
		return;
	} else
		self->base = GMATHML_ELEMENT (node);

	node = node->next_sibling;
	if (node != NULL)
		switch (self->type) {
			case GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER:
				self->underscript = NULL;
				self->overscript = GMATHML_ELEMENT (node);
				break;
			case GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER:
				self->underscript = GMATHML_ELEMENT (node);
				self->overscript = NULL;
				break;
			default:
				self->underscript = GMATHML_ELEMENT (node);
				node = node->next_sibling;
				if (node != NULL)
					self->overscript = GMATHML_ELEMENT (node);
		}
	else {
		self->underscript = NULL;
		self->overscript = NULL;
	}
}

static void
gmathml_under_over_element_post_new_child (GDomNode *self, GDomNode *child)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	gmathml_under_over_element_update_child_pointers (under_over);
}

/* GMathmlElement implementation */

static void
gmathml_under_over_element_update (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);

	under_over->display = style->display;

}

static gboolean
gmathml_under_over_element_update_children (GMathmlElement *self, GMathmlStyle *style)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);
	GMathmlStyle *overscript_style;
	gboolean accent = FALSE;
	gboolean accent_under = FALSE;
	gboolean movable_limits = FALSE;
	double very_thick_math_space_value;
	double very_thin_math_space_value;
	gboolean need_measure = FALSE;

	very_thin_math_space_value = style->very_thin_math_space_value;
	very_thick_math_space_value = style->very_thick_math_space_value;

	if (under_over->base != NULL)
		if (gmathml_element_update (GMATHML_ELEMENT (under_over->base), style))
			need_measure = TRUE;

	style->display = GMATHML_DISPLAY_INLINE;

	overscript_style = gmathml_style_duplicate (style);

	if (under_over->underscript != NULL) {
		if (!under_over->accent_under.value)
			gmathml_style_change_script_level (style, +1);

		if (gmathml_element_update (GMATHML_ELEMENT (under_over->underscript), style))
			need_measure = TRUE;
	}

	if (under_over->overscript != NULL) {
		if (!under_over->accent.value)
			gmathml_style_change_script_level (overscript_style, +1);

		if (gmathml_element_update (GMATHML_ELEMENT (under_over->overscript), overscript_style))
			need_measure = TRUE;
	}

	gmathml_style_free (overscript_style);

	if (under_over->base != NULL) {
		const GMathmlOperatorElement *operator;

		operator = gmathml_element_get_embellished_core (under_over->base);
		if (operator != NULL) {
			movable_limits = operator->movable_limits.value;
			gdom_debug ("[UnderOver::update] movable_limits found");
		}
	}

	if (under_over->overscript != NULL) {
		const GMathmlOperatorElement *operator;

		operator = gmathml_element_get_embellished_core (under_over->overscript);
		if (operator != NULL) {
			accent = operator->accent.value;
			if (accent)
				gdom_debug ("[UnderOver::update] Overscript is%s accent (%s)",
					    accent ? "" : " not",
					    gdom_node_get_node_name (GDOM_NODE (operator)));
		}
	}

	gmathml_attribute_boolean_parse (&under_over->accent, &accent);

	if (under_over->underscript != NULL) {
		const GMathmlOperatorElement *operator;

		operator = gmathml_element_get_embellished_core (under_over->underscript);
		if (operator != NULL) {
			accent_under = operator->accent.value;
			gdom_debug ("[UnderOver::update] Underscript is%s accent (%s)",
				    accent_under ? "" : " not",
				    gdom_node_get_node_name (GDOM_NODE (operator)));
		}
	}

	gmathml_attribute_boolean_parse (&under_over->accent_under, &accent_under);

	under_over->under_space = accent_under ? very_thin_math_space_value : very_thick_math_space_value;
	under_over->over_space  = accent       ? very_thin_math_space_value : very_thick_math_space_value;

	under_over->as_script = under_over->display == GMATHML_DISPLAY_INLINE && movable_limits;

	gdom_debug ("[UnderOver::update] space under = %g, over = %g",
		    under_over->under_space, under_over->over_space);

	return need_measure;
}

static const GMathmlBbox *
gmathml_under_over_element_measure (GMathmlElement *self, GMathmlView *view, const GMathmlBbox *stretch_bbox)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);
	const GMathmlOperatorElement *operator;
	GDomNode *node;
	const GMathmlBbox *child_bbox;
	GMathmlBbox regular_stretch_bbox;
	GMathmlBbox operator_stretch_bbox;
	GMathmlBbox script_bbox;
	gboolean stretchy_found = FALSE;
	gboolean all_stretchy = TRUE;
	unsigned int index;

	if (under_over->as_script) {
		gmathml_measure_sub_sup (self, view,
					 under_over->base,
					 under_over->underscript,
					 under_over->overscript,
					 0.0, 0.0,
					 GMATHML_DISPLAY_INLINE,
					 stretch_bbox, &self->bbox,
					 &under_over->underscript_offset,
					 &under_over->overscript_offset);

		return &self->bbox;
	}

	self->bbox = gmathml_bbox_null;

	if (under_over->base == NULL)
		return &self->bbox;

	regular_stretch_bbox = gmathml_bbox_null;
	operator_stretch_bbox = gmathml_bbox_null;

	gdom_debug ("[UnderOver::measure] Begin");

	index = 0;
	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (GMATHML_IS_ELEMENT (node)) {
			operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
			if (operator != NULL && operator->stretchy.value) {
				stretchy_found = TRUE;

				if (node == GDOM_NODE (under_over->base))
					child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view,
									      stretch_bbox);
				else
					child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view,
									      &gmathml_bbox_null);
				gmathml_bbox_stretch_horizontally (&operator_stretch_bbox, child_bbox);
			} else {
				all_stretchy = FALSE;

				child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view,
								      &gmathml_bbox_null);
				gmathml_bbox_stretch_horizontally (&regular_stretch_bbox, child_bbox);
			}
		}

		index++;
	}

	if (stretchy_found) {
		if (all_stretchy) {
			gdom_debug ("[UnderOver::measure] All stretchy");
			regular_stretch_bbox = operator_stretch_bbox;
		}

		gdom_debug ("[UnderOver::measure] Stretchy found (width = %g, height = %g, depth = %g)",
			    regular_stretch_bbox.width,
			    regular_stretch_bbox.height,
			    regular_stretch_bbox.depth);

		for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
			if (GMATHML_IS_ELEMENT (node)) {
				operator = gmathml_element_get_embellished_core (GMATHML_ELEMENT (node));
				if (operator != NULL && operator->stretchy.value) {
					if (node == GDOM_NODE (under_over->base)) {
						GMathmlBbox base_stretch_bbox;

						base_stretch_bbox = regular_stretch_bbox;
						gmathml_bbox_stretch (&base_stretch_bbox, stretch_bbox);

						child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view,
										      &base_stretch_bbox);
					} else
						child_bbox = gmathml_element_measure (GMATHML_ELEMENT (node), view,
										      &regular_stretch_bbox);
				}
			}
		}
	}

	self->bbox = *gmathml_element_get_bbox (under_over->base);

	if (under_over->overscript != NULL) {
		script_bbox = *gmathml_element_get_bbox (under_over->overscript);
		script_bbox.depth += gmathml_view_measure_length (view, under_over->over_space);
		gmathml_bbox_add_over (&self->bbox, &script_bbox);
	}

	if (under_over->underscript != NULL) {
		script_bbox = *gmathml_element_get_bbox (under_over->underscript);
		script_bbox.height += gmathml_view_measure_length (view, under_over->under_space);
		gmathml_bbox_add_under (&self->bbox, &script_bbox);
	}

	gdom_debug ("[UnderOver::measure] End");

	return &self->bbox;
}

static void
gmathml_under_over_element_layout (GMathmlElement *self, GMathmlView *view,
				double x, double y, const GMathmlBbox *bbox)
{
	GMathmlUnderOverElement *under_over = GMATHML_UNDER_OVER_ELEMENT (self);
	const GMathmlBbox *child_bbox;

	if (under_over->as_script) {
		gmathml_layout_sub_sup (self, view,
					x, y,
					under_over->base,
					under_over->underscript,
					under_over->overscript,
					under_over->underscript_offset,
					under_over->overscript_offset);

		return;
	}

	if (under_over->base == NULL)
		return;

	child_bbox = gmathml_element_get_bbox (under_over->base);
	gmathml_element_layout (under_over->base, view, x + (bbox->width - child_bbox->width) * 0.5, y,
				child_bbox);

	if (under_over->underscript) {
		child_bbox = gmathml_element_get_bbox (under_over->underscript);
		gmathml_element_layout (under_over->underscript, view,
					x + (bbox->width - child_bbox->width) * 0.5,
					y + self->bbox.depth - child_bbox->depth,
					child_bbox);
	}
	if (under_over->overscript) {
		child_bbox = gmathml_element_get_bbox (under_over->overscript);
		gmathml_element_layout (under_over->overscript, view,
					x + (bbox->width - child_bbox->width) * 0.5,
					y - self->bbox.height + child_bbox->height,
					child_bbox);
	}
}

static const GMathmlOperatorElement *
gmathml_under_over_element_get_embellished_core (const GMathmlElement *self)
{
	if (GDOM_NODE (self)->first_child != NULL)
		return gmathml_element_get_embellished_core (GMATHML_ELEMENT (GDOM_NODE (self)->first_child));

	return NULL;
}

/* GMathmlUnderOverElement implementation */

GDomNode *
gmathml_under_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER;

	return node;
}

GDomNode *
gmathml_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_OVER;

	return node;
}

GDomNode *
gmathml_under_over_element_new (void)
{
	GDomNode *node;

	node = g_object_new (GMATHML_TYPE_UNDER_OVER_ELEMENT, NULL);
	g_return_val_if_fail (node != NULL, NULL);

	GMATHML_UNDER_OVER_ELEMENT (node)->type = GMATHML_UNDER_OVER_ELEMENT_TYPE_UNDER_OVER;

	return node;
}

static void
gmathml_under_over_element_init (GMathmlUnderOverElement *self)
{
}

/* GMathmlUnderOverElement class */

static void
gmathml_under_over_element_class_init (GMathmlUnderOverElementClass *under_over_class)
{
	GDomNodeClass *d_node_class = GDOM_NODE_CLASS (under_over_class);
	GMathmlElementClass *m_element_class = GMATHML_ELEMENT_CLASS (under_over_class);

	parent_class = g_type_class_peek_parent (under_over_class);

	d_node_class->get_node_name = gmathml_under_over_get_node_name;
	d_node_class->can_append_child = gmathml_under_over_element_can_append_child;
	d_node_class->post_new_child = gmathml_under_over_element_post_new_child;

	m_element_class->update = gmathml_under_over_element_update;
	m_element_class->update_children = gmathml_under_over_element_update_children;
	m_element_class->measure = gmathml_under_over_element_measure;
	m_element_class->layout = gmathml_under_over_element_layout;
	m_element_class->get_embellished_core = gmathml_under_over_element_get_embellished_core;
	m_element_class->is_inferred_row = NULL;

	m_element_class->attributes = gmathml_attribute_map_new ();

	gmathml_element_class_add_element_attributes (m_element_class);

	gmathml_attribute_map_add_attribute (m_element_class->attributes, "accent",
					     offsetof (GMathmlUnderOverElement, accent));
	gmathml_attribute_map_add_attribute (m_element_class->attributes, "accentunder",
					     offsetof (GMathmlUnderOverElement, accent_under));
}

G_DEFINE_TYPE (GMathmlUnderOverElement, gmathml_under_over_element, GMATHML_TYPE_ELEMENT)
