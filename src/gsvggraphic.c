/*
 * Copyright © 2009 Emmanuel Pacaud
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

#include <gsvggraphic.h>
#include <gsvgview.h>
#include <gsvgutils.h>
#include <gdomdebug.h>

static GObjectClass *parent_class;

static void
gsvg_graphic_update (GSvgElement *self, GSvgStyle *parent_style)
{
	GSvgGraphic *graphic = GSVG_GRAPHIC (self);

	/* Handle inline style here for now. This should move to the CSS handling part. */

	if (graphic->style.value != NULL) {
		GSvgElementClass *s_element_class = GSVG_ELEMENT_GET_CLASS (self);
		char *inline_style;

		inline_style = g_strdup (graphic->style.value);

		if (inline_style) {
			char *end_ptr = inline_style;
			char *name;
			char *value;

			while (*end_ptr != '\0') {

				gsvg_str_skip_spaces (&end_ptr);

				name = end_ptr;

				while (*end_ptr != '\0' && *end_ptr != ':' && !g_ascii_isspace (*end_ptr))
					end_ptr++;

				if (*end_ptr != '\0') {
					*end_ptr = '\0';
					end_ptr++;

					gsvg_str_skip_colon_and_spaces (&end_ptr);

					if (*end_ptr != '\0') {
						char old_char;

						value = end_ptr;

						gsvg_str_skip_semicolon_and_spaces (&end_ptr);

						old_char = *end_ptr;
						*end_ptr = '\0';

						gdom_debug ("[GSvgGraphic::update] inline_style %s = %s",
							    name, value);

						gdom_attribute_map_set_css_attribute (s_element_class->attributes,
										      self,
										      name,
										      value,
										      GDOM_CSS_TYPE_AUTHOR);

						*end_ptr = old_char;

						gsvg_str_skip_char (&end_ptr, ';');
					}
				}
			}
			g_free (inline_style);
		}
	}

	if (graphic->fill != NULL) {
		gdom_debug ("[GSvgGraphic::update] fill");

		gsvg_paint_attribute_parse (&graphic->fill->paint, &parent_style->fill.paint);
		gsvg_fill_rule_attribute_parse (&graphic->fill->rule, &parent_style->fill.rule);
		gdom_double_attribute_parse (&graphic->fill->opacity, &parent_style->fill.opacity);
	}

	if (graphic->stroke != NULL) {
		gdom_debug ("[GSvgGraphic::update] stroke");

		gsvg_paint_attribute_parse (&graphic->stroke->paint, &parent_style->stroke.paint);
		gsvg_length_attribute_parse (&graphic->stroke->width, &parent_style->stroke.width, 0.0);
		gdom_double_attribute_parse (&graphic->stroke->opacity, &parent_style->stroke.opacity);
	}

	if (graphic->transform != NULL) {
		gsvg_transform_attribute_parse (&graphic->transform->transform);
	}
}

static void
_graphic_render (GSvgElement *self, GSvgView *view)
{
	GDomNode *node;

	for (node = GDOM_NODE (self)->first_child; node != NULL; node = node->next_sibling)
		if (GSVG_IS_ELEMENT (node))
		    gsvg_element_render (GSVG_ELEMENT (node), view);
}

static void
gsvg_graphic_render (GSvgElement *self, GSvgView *view)
{
	GSvgGraphic *graphic = GSVG_GRAPHIC (self);

	gdom_debug ("[GSvgGraphic::render] Render %s", gdom_node_get_node_name (GDOM_NODE (self)));

	if (graphic->fill != NULL)
		gsvg_view_push_fill_attributes (view, graphic->fill);
	if (graphic->stroke != NULL)
		gsvg_view_push_stroke_attributes (view, graphic->stroke);
	if (graphic->transform != NULL)
		gsvg_view_push_transform (view, &graphic->transform->transform.matrix);

	GSVG_GRAPHIC_GET_CLASS (graphic)->graphic_render (self, view);

	if (graphic->transform != NULL)
		gsvg_view_pop_transform (view);
	if (graphic->stroke != NULL)
		gsvg_view_pop_stroke_attributes (view);
	if (graphic->fill != NULL)
		gsvg_view_pop_fill_attributes (view);
}

/* GSvgGraphic implementation */

static void
gsvg_graphic_init (GSvgGraphic *self)
{
}

static void
gsvg_graphic_finalize (GObject *object)
{
}

/* GSvgGraphic class */

static void
gsvg_graphic_class_init (GSvgGraphicClass *s_graphic_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_graphic_class);
	GSvgElementClass *s_element_class = GSVG_ELEMENT_CLASS (s_graphic_class);

	parent_class = g_type_class_peek_parent (s_graphic_class);

	object_class->finalize = gsvg_graphic_finalize;

	s_element_class->update = gsvg_graphic_update;
	s_element_class->render = gsvg_graphic_render;

	s_graphic_class->graphic_render = _graphic_render;

	s_element_class->attributes = gdom_attribute_map_duplicate (s_element_class->attributes);

	gdom_attribute_map_add_attribute (s_element_class->attributes, "class",
					  offsetof (GSvgGraphic, class_name));
	gdom_attribute_map_add_attribute (s_element_class->attributes, "style",
					  offsetof (GSvgGraphic, style));

	gdom_attribute_map_add_fill_attribute_bag (s_element_class->attributes, offsetof (GSvgGraphic, fill));
	gdom_attribute_map_add_stroke_attribute_bag (s_element_class->attributes, offsetof (GSvgGraphic, stroke));
	gdom_attribute_map_add_transform_attribute_bag (s_element_class->attributes, offsetof (GSvgGraphic, transform));
}

G_DEFINE_ABSTRACT_TYPE (GSvgGraphic, gsvg_graphic, GSVG_TYPE_ELEMENT)

