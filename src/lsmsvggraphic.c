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

#include <lsmsvggraphic.h>
#include <lsmsvgview.h>
#include <lsmsvgutils.h>
#include <lsmdomdebug.h>

static GObjectClass *parent_class;

static void
lsm_svg_graphic_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgGraphic *graphic = LSM_SVG_GRAPHIC (self);

	/* Handle inline style here for now. This should move to the CSS handling part. */

	if (graphic->style.value != NULL) {
		LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_GET_CLASS (self);
		char *inline_style;

		inline_style = g_strdup (graphic->style.value);

		if (inline_style) {
			char *end_ptr = inline_style;
			char *name;
			char *value;

			while (*end_ptr != '\0') {

				lsm_svg_str_skip_spaces (&end_ptr);

				name = end_ptr;

				while (*end_ptr != '\0' && *end_ptr != ':' && !g_ascii_isspace (*end_ptr))
					end_ptr++;

				if (*end_ptr != '\0') {
					*end_ptr = '\0';
					end_ptr++;

					lsm_svg_str_skip_colon_and_spaces (&end_ptr);

					if (*end_ptr != '\0') {
						char old_char;

						value = end_ptr;

						while (*end_ptr != ';' && *end_ptr != '\0')
							end_ptr++;

						old_char = *end_ptr;
						*end_ptr = '\0';

						lsm_dom_debug ("[LsmSvgGraphic::update] inline_style %s = %s",
							    name, value);

						lsm_dom_attribute_map_set_css_attribute (s_element_class->attributes,
										      self,
										      name,
										      value,
										      LSM_DOM_CSS_TYPE_AUTHOR);

						*end_ptr = old_char;

						lsm_svg_str_skip_char (&end_ptr, ';');
					}
				}
			}
			g_free (inline_style);
		}
	}

	if (graphic->fill != NULL) {
		lsm_dom_debug ("[LsmSvgGraphic::update] fill");

		lsm_svg_paint_attribute_parse (&graphic->fill->paint, &parent_style->fill.paint);
		lsm_svg_fill_rule_attribute_parse (&graphic->fill->rule, &parent_style->fill.rule);
		lsm_dom_double_attribute_parse (&graphic->fill->opacity, &parent_style->fill.opacity);
	}

	if (graphic->stroke != NULL) {
		lsm_dom_debug ("[LsmSvgGraphic::update] stroke");

		lsm_svg_paint_attribute_parse (&graphic->stroke->paint, &parent_style->stroke.paint);
		lsm_svg_length_attribute_parse (&graphic->stroke->width, &parent_style->stroke.width, 0.0);
		lsm_dom_double_attribute_parse (&graphic->stroke->opacity, &parent_style->stroke.opacity);
	}

	if (graphic->transform != NULL) {
		lsm_svg_transform_attribute_parse (&graphic->transform->transform);
	}

	if (graphic->text != NULL) {
		lsm_dom_string_attribute_parse (&graphic->text->font_family, &parent_style->text.font_family);
		lsm_svg_length_attribute_parse (&graphic->text->font_size, &parent_style->text.font_size, 0.0);
	}

	if (graphic->stop != NULL) {
		lsm_svg_color_attribute_parse (&graphic->stop->color, &parent_style->stop.color);
		lsm_dom_double_attribute_parse (&graphic->stop->opacity, &parent_style->stop.opacity);
	}
}

static void
_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmDomNode *node;

	lsm_dom_debug ("[LsmSvgGraphic::_graphic_render");

	for (node = LSM_DOM_NODE (self)->first_child; node != NULL; node = node->next_sibling) {
		if (LSM_SVG_IS_ELEMENT (node))
		    lsm_svg_element_render (LSM_SVG_ELEMENT (node), view);
	}
}

static void
lsm_svg_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgGraphic *graphic = LSM_SVG_GRAPHIC (self);

	if (graphic->fill != NULL)
		lsm_svg_view_push_fill_attributes (view, graphic->fill);
	if (graphic->stroke != NULL)
		lsm_svg_view_push_stroke_attributes (view, graphic->stroke);
	if (graphic->text != NULL)
		lsm_svg_view_push_text_attributes (view, graphic->text);
	if (graphic->transform != NULL)
		lsm_svg_view_push_transform (view, &graphic->transform->transform.matrix);

	LSM_SVG_GRAPHIC_GET_CLASS (graphic)->graphic_render (self, view);

	if (graphic->transform != NULL)
		lsm_svg_view_pop_transform (view);
	if (graphic->text != NULL)
		lsm_svg_view_pop_text_attributes (view);
	if (graphic->stroke != NULL)
		lsm_svg_view_pop_stroke_attributes (view);
	if (graphic->fill != NULL)
		lsm_svg_view_pop_fill_attributes (view);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_graphic_init (LsmSvgGraphic *self)
{
}

static void
lsm_svg_graphic_finalize (GObject *object)
{
}

/* LsmSvgGraphic class */

static void
lsm_svg_graphic_class_init (LsmSvgGraphicClass *s_graphic_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_graphic_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_graphic_class);

	parent_class = g_type_class_peek_parent (s_graphic_class);

	object_class->finalize = lsm_svg_graphic_finalize;

	s_element_class->update = lsm_svg_graphic_update;
	s_element_class->render = lsm_svg_graphic_render;

	s_graphic_class->graphic_render = _graphic_render;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "class",
					  offsetof (LsmSvgGraphic, class_name));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "style",
					  offsetof (LsmSvgGraphic, style));

	lsm_dom_attribute_map_add_fill_attribute_bag (s_element_class->attributes, offsetof (LsmSvgGraphic, fill));
	lsm_dom_attribute_map_add_stroke_attribute_bag (s_element_class->attributes, offsetof (LsmSvgGraphic, stroke));
	lsm_dom_attribute_map_add_transform_attribute_bag (s_element_class->attributes, offsetof (LsmSvgGraphic, transform));
	lsm_dom_attribute_map_add_text_attribute_bag (s_element_class->attributes, offsetof (LsmSvgGraphic, text));
	lsm_dom_attribute_map_add_stop_attribute_bag (s_element_class->attributes, offsetof (LsmSvgGraphic, stop));
}

G_DEFINE_ABSTRACT_TYPE (LsmSvgGraphic, lsm_svg_graphic, LSM_SVG_TYPE_ELEMENT)

