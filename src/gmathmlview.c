/* gmathmlview.c
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

#include <gmathmlview.h>
#include <gmathmldocument.h>
#include <gmathmlelement.h>
#include <gmathmlmathelement.h>
#include <gmathmlstyleelement.h>

#include <math.h>
#include <string.h>

#define GMATHML_LARGE_OP_SCALE	1.8

typedef enum {
	GMATHML_STRETCH_DIRECTION_NONE,
	GMATHML_STRETCH_DIRECTION_VERTICAL,
	GMATHML_STRETCH_DIRECTION_HORIZONTAL,
	GMATHML_STRETCH_DIRECTION_BOTH
} GMathmlStretchDirection;

typedef struct {
	const char *font;
	const char utf8[4];
} GMathmlGlyph;

typedef struct {
	const char *font;
	struct {
		const char utf8[4];
		const gboolean is_stretchy;
	} glyphs[6];
} GMathmlCompoundGlyph;

typedef struct {
	const char utf8[4];
	GMathmlStretchDirection	direction;
	GMathmlGlyph glyphs[6];
	GMathmlCompoundGlyph compound_glyph;
} GMathmlStretchyGlyph;

static const GMathmlStretchyGlyph latex_table[] = {
	{"(", GMATHML_STRETCH_DIRECTION_VERTICAL,
		{
			{"cmr10", 	"("} ,
			{"cmex10",	"\xc2\xa1"},
			{"cmex10",	"\xc2\xb3"},
			{"cmex10",	"\xc2\xb5"},
			{"cmex10",	"\xc3\x83"},
			{"",		""}
		},
		{"cmex10", {{"0", FALSE}, {"B", TRUE}, {"@", FALSE}, {"", FALSE}}}
	},
	{")", GMATHML_STRETCH_DIRECTION_VERTICAL,
		{
			{"cmr10", 	")"},
			{"cmex10",	"\xc2\xa2"},
			{"cmex10",	"\xc2\xb4"},
			{"cmex10",	"\xc2\xb6"},
			{"cmex10",	"!"},
			{"",		""}
		},
		{"cmex10", {{"1", FALSE}, {"C", TRUE}, {"A", FALSE}, {"", FALSE}}}
	},
	{"{", GMATHML_STRETCH_DIRECTION_VERTICAL,
		{
			{"cmsy10", 	"f"},
			{"cmex10",	"\xc2\xa9"},
			{"cmex10",	"n"},
			{"cmex10",	"\xc2\xbd"},
			{"cmex10",	"("},
			{"",		""}
		},
		{"cmex10", {{"8", FALSE}, {">", TRUE}, {"<", FALSE}, {">", TRUE}, {":", FALSE}, {"", FALSE}}}
	},
	{"}", GMATHML_STRETCH_DIRECTION_VERTICAL,
		{
			{"cmsy10",	"g"},
			{"cmex10",	"\xc2\xaa"},
			{"cmex10",	"o"},
			{"cmex10",	"\xc2\xbe"},
			{"cmex10",	")"},
			{"",		""}
		},
		{"cmex10", {{"9", FALSE}, {">", TRUE}, {"=", FALSE}, {">", TRUE}, {";", FALSE}, {"", FALSE}}}
	},

	/* Sum ∑ */

	{"\xe2\x88\x91", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmr10", 	"\xc2\xa7"},
			{"cmex10",	"P"},
			{"cmex10",	"X"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* Product ∏ */

	{"\xe2\x88\x8f", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmr10",	"\xc2\xa6"},
			{"cmex10",	"Q"},
			{"cmex10",	"Y"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* Coproduct ∐ */

	{"\xe2\x88\x90", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmsy10", 	"q"},
			{"cmex10",	"`"},
			{"cmex10",	"a"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* Integral ∫ */

	{"\xe2\x88\xab", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmsy10", 	"s"},
			{"cmex10",	"R"},
			{"cmex10",	"Z"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* Contour integral ∮ */

	{"\xe2\x88\xae", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmex10",	"H"},
			{"cmex10",	"I"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* DifferentialD d */

	{"\xe2\x85\x86", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"cmmi10",	"d"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* UnderBar */

	{"\xcc\xb2", GMATHML_STRETCH_DIRECTION_HORIZONTAL,
		{
			{"Sans",	"\xcc\xb2"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	/* OverBar */

	{"\xc2\xaf", GMATHML_STRETCH_DIRECTION_HORIZONTAL,
		{
			{"Sans",	"\xc2\xaf"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	{"-", GMATHML_STRETCH_DIRECTION_BOTH,
		{
			{"Sans",	"\xe2\x88\x92"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	},

	{"|", GMATHML_STRETCH_DIRECTION_VERTICAL,
		{
			{"Sans",	"|"},
			{"",		""}
		},
		{"", {{"", FALSE}}}
	}

	/*
	,
	{"^", GMATHML_STRETCH_DIRECTION_HORIZONTAL,
		{"b", "c", "d", ""},
		{{"", FALSE}}
	},
	{"~", GMATHML_STRETCH_DIRECTION_HORIZONTAL,
		{"e", "f", "g", ""},
		{{"", FALSE}}
	},
	{"\xe2\x88\x8f", GMATHML_STRETCH_DIRECTION_BOTH,
		{"Q", "Y", ""},
		{{"", FALSE}}
	}
	*/
};

static GObjectClass *parent_class;

struct _GMathmlViewPrivate {

	gboolean debug;

	GMathmlDocument *document;

	PangoLayout *pango_layout;
	PangoFontDescription *font_description;

	gboolean is_vector;

	/* rendering context */
	cairo_t *cairo;

	const GMathmlElement *current_element;
	GSList *elements;
};

double
gmathml_view_measure_length (GMathmlView *view, double length)
{
	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	if (view->priv->is_vector)
		return length;

	return floor (length + 0.5);
}

double
gmathml_view_measure_hairline (GMathmlView *view)
{
	return gmathml_view_measure_length (view, 1.0);
}

static void
gmathml_view_update_layout (GMathmlView *view, char const *text,
			    gboolean large,
			    PangoRectangle *ink_rect,
			    PangoRectangle *rect,
			    int *baseline)
{
	const GMathmlElement *element;

	element = view->priv->current_element;

	pango_font_description_set_family (view->priv->font_description, "Serif");
	pango_font_description_set_size (view->priv->font_description,
					 element->math_size * PANGO_SCALE * (large ? GMATHML_LARGE_OP_SCALE : 1.0));
	switch (element->math_variant) {
		case GMATHML_VARIANT_NORMAL:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			break;
		case GMATHML_VARIANT_ITALIC:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_ITALIC);
			break;
		default:
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			break;
	}
	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
	pango_layout_get_extents (view->priv->pango_layout, ink_rect, rect);

	if (baseline != NULL) {
		PangoLayoutIter *iter;

		iter = pango_layout_get_iter (view->priv->pango_layout);
		*baseline = pango_layout_iter_get_baseline (iter);
		pango_layout_iter_free (iter);
	}
}

static void
gmathml_view_show_layout (GMathmlView *view, double x, double y,
			  int baseline,
			  const PangoRectangle *ink_rect,
			  const PangoRectangle * rect)
{
	if (view->priv->debug) {
		cairo_set_line_width (view->priv->cairo, 0.1);
		cairo_set_source_rgb (view->priv->cairo, 1,0,0);
		cairo_rectangle (view->priv->cairo,
				 x + pango_units_to_double (rect->x)
				 - pango_units_to_double (ink_rect->x),
				 y + pango_units_to_double (rect->y) -
				 pango_units_to_double (baseline),
				 pango_units_to_double (rect->width),
				 pango_units_to_double (rect->height));
		cairo_stroke (view->priv->cairo);
		cairo_set_source_rgb (view->priv->cairo, 0,1,0);
		cairo_rectangle (view->priv->cairo,
				 x,
				 y + pango_units_to_double (ink_rect->y) -
				 pango_units_to_double (baseline),
				 pango_units_to_double (ink_rect->width),
				 pango_units_to_double (ink_rect->height));
		cairo_stroke (view->priv->cairo);
	}

}

double
gmathml_view_measure_axis_offset (GMathmlView *view, double math_size)
{
	PangoRectangle ink_rect;
	PangoLayoutIter *iter;
	double axis_offset;
	int baseline;

	g_return_val_if_fail (GMATHML_IS_VIEW (view), 0.0);

	pango_font_description_set_family (view->priv->font_description, "Serif");
	pango_font_description_set_size (view->priv->font_description, math_size * PANGO_SCALE);
	pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
	pango_layout_set_text (view->priv->pango_layout, "\xe2\x88\x92", -1);
	pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
	pango_layout_get_extents (view->priv->pango_layout, &ink_rect, NULL);

	iter = pango_layout_get_iter (view->priv->pango_layout);
	baseline = pango_layout_iter_get_baseline (iter);
	pango_layout_iter_free (iter);

	axis_offset = pango_units_to_double (- 0.5 * ink_rect.height - ink_rect.y + baseline);

	g_message ("[GMathmlView::measure_axis_offset] offset = %g (%g %%)",
		   axis_offset, axis_offset / view->priv->current_element->math_size);

	return axis_offset;
}

void
gmathml_view_measure_text (GMathmlView *view, char const *text, GMathmlBbox *bbox)
{
	const GMathmlElement *element;
	PangoRectangle ink_rect;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element));
	g_return_if_fail (bbox != NULL);

	element = view->priv->current_element;

	if (text == NULL) {
		*bbox = gmathml_bbox_null;
		return;
	}

	gmathml_view_update_layout (view, text, FALSE, &ink_rect, NULL, &baseline);

	bbox->width = pango_units_to_double (ink_rect.width);
	bbox->height = pango_units_to_double (baseline - ink_rect.y);
	bbox->depth = pango_units_to_double (ink_rect.height + ink_rect.y - baseline);
	bbox->is_defined = TRUE;
}

void
gmathml_view_show_text (GMathmlView *view, double x, double y, char const *text)
{
	const GMathmlElement *element;
	PangoRectangle rect, ink_rect;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element));

	element = view->priv->current_element;

	if (text == NULL)
		return;

/*        g_message ("View: show_text %s at %g, %g (size = %g) %s",*/
/*                   text, x, y, element->math_size,*/
/*                   gmathml_variant_to_string (element->math_variant));*/

	gmathml_view_update_layout (view, text, FALSE, &ink_rect, &rect, &baseline);
	gmathml_view_show_layout (view, x, y, baseline, &ink_rect, &rect);

	cairo_set_source_rgba (view->priv->cairo,
			       element->math_color.red,
			       element->math_color.green,
			       element->math_color.blue,
			       element->math_color.alpha);

	cairo_move_to (view->priv->cairo, x - pango_units_to_double (ink_rect.x), y - pango_units_to_double (baseline));
	pango_cairo_show_layout (view->priv->cairo, view->priv->pango_layout);
}

static const GMathmlStretchyGlyph *
gmathml_view_find_strecthy_glyph (const char *text)
{
	unsigned int i;

	for (i = 0; i < G_N_ELEMENTS (latex_table); i++)
		if (strcmp (text, latex_table[i].utf8) == 0) {
			g_message ("[GMathmlView::find_strecthy_glyph] found %s", text);
			return &latex_table[i];
		}

	return NULL;
}

void
gmathml_view_measure_operator (GMathmlView *view,
			       char const *text,
			       gboolean large,
			       gboolean symmetric,
			       double axis_offset,
			       GMathmlBbox const *stretch_bbox, GMathmlBbox *bbox)
{
	const GMathmlElement *element;
	const GMathmlStretchyGlyph *glyph;
	GMathmlStretchDirection direction;
	PangoRectangle ink_rect;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element));
	g_return_if_fail (bbox != NULL);
	g_return_if_fail (stretch_bbox != NULL);

	element = view->priv->current_element;

	if (text == NULL) {
		*bbox = gmathml_bbox_null;
		return;
	}

	if (stretch_bbox->is_defined)
		g_message ("[GMathmlView::measure_operator] Stretch bbox w = %g, h = %g, d = %g",
			   stretch_bbox->width, stretch_bbox->height, stretch_bbox->depth);

	glyph = gmathml_view_find_strecthy_glyph (text);
	if (glyph == NULL) {
		gmathml_view_update_layout (view, text, large, &ink_rect, NULL, &baseline);
		direction = GMATHML_STRETCH_DIRECTION_BOTH;

		g_message ("[GMathmlView::measure_operator] operator = %s", text);

	} else {
		PangoLayoutIter *iter;
		unsigned int i;
		gboolean found = FALSE;

		if (glyph->glyphs[1].utf8[0] == '\x0') {
			pango_font_description_set_size (view->priv->font_description,
							 element->math_size * PANGO_SCALE *
							 (large ? GMATHML_LARGE_OP_SCALE : 1.0));
			i = 0;
		} else {
			pango_font_description_set_size (view->priv->font_description,
							 element->math_size * PANGO_SCALE);
			i = large ? 1 : 0;
		}

		for (; glyph->glyphs[i].utf8[0] != '\x0' && !found; i++) {
			pango_font_description_set_family (view->priv->font_description, glyph->glyphs[i].font);
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			pango_layout_set_text (view->priv->pango_layout, glyph->glyphs[i].utf8, -1);
			pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
			pango_layout_get_extents (view->priv->pango_layout, &ink_rect, NULL);

			g_message ("Glyph #%i -> width = %g, height = %g", i,
				   pango_units_to_double (ink_rect.width),
				   pango_units_to_double (ink_rect.height));

			if (!stretch_bbox->is_defined)
				found = TRUE;
			else
				switch (glyph->direction) {
					case GMATHML_STRETCH_DIRECTION_VERTICAL:
						if (pango_units_to_double (ink_rect.height) >
						    (stretch_bbox->height + stretch_bbox->depth))
							found = TRUE;
						break;
					case GMATHML_STRETCH_DIRECTION_HORIZONTAL:
						if (pango_units_to_double (ink_rect.width) >
						    stretch_bbox->width)
							found = TRUE;
						break;
					default:
						if (pango_units_to_double (ink_rect.height) >
						    (stretch_bbox->height + stretch_bbox->depth) &&
						    pango_units_to_double (ink_rect.width) >
						    stretch_bbox->width)
							found = TRUE;
						break;
				}
		}

		if (found)
			g_message ("Found sized glyph #%i", i);

		iter = pango_layout_get_iter (view->priv->pango_layout);
		baseline = pango_layout_iter_get_baseline (iter);
		pango_layout_iter_free (iter);

		direction = glyph->direction;
	}

	if (direction == GMATHML_STRETCH_DIRECTION_BOTH &&
	    stretch_bbox->is_defined &&
	    pango_units_to_double (ink_rect.width) != 0.0 &&
	    pango_units_to_double (ink_rect.height) != 0.0) {
		double x_scale;
		double y_scale;

		x_scale = stretch_bbox->width / pango_units_to_double (ink_rect.width);
		y_scale = (stretch_bbox->height + stretch_bbox->depth) / pango_units_to_double (ink_rect.height);

		g_message ("x_scale = %g, y_scale = %g", x_scale, y_scale);

		*bbox = *stretch_bbox;

		if (x_scale > y_scale) {
			bbox->depth = pango_units_to_double (ink_rect.y - baseline) * x_scale;
			bbox->height = pango_units_to_double (ink_rect.height + ink_rect.y - baseline) * x_scale;

			bbox->depth = gmathml_view_measure_length (view, bbox->depth);
			bbox->height = gmathml_view_measure_length (view, bbox->height);
		} else {
			bbox->width = pango_units_to_double (ink_rect.width) * y_scale;

			bbox->width = gmathml_view_measure_length (view, bbox->width);
		}

		return;
	}

	if (stretch_bbox->is_defined && direction != GMATHML_STRETCH_DIRECTION_HORIZONTAL) {
		bbox->height = stretch_bbox->height;
		bbox->depth = stretch_bbox->depth;
	} else {
		bbox->height = pango_units_to_double (baseline - ink_rect.y);
		bbox->depth = pango_units_to_double (ink_rect.height + ink_rect.y - baseline);
	}
	if (stretch_bbox->is_defined && direction != GMATHML_STRETCH_DIRECTION_VERTICAL)
		bbox->width = stretch_bbox->width;
	else
		bbox->width = pango_units_to_double (ink_rect.width);

	if (stretch_bbox->is_defined && symmetric &&
	    (direction == GMATHML_STRETCH_DIRECTION_VERTICAL ||
	     direction == GMATHML_STRETCH_DIRECTION_BOTH)) {
		double length = MAX (axis_offset + bbox->depth, bbox->height - axis_offset);

		bbox->height = gmathml_view_measure_length (view, length + axis_offset);
		bbox->depth =  gmathml_view_measure_length (view, length - axis_offset);
	}

	bbox->is_defined = TRUE;
}

void
gmathml_view_show_operator (GMathmlView *view, double x, double y,
			    char const *text, gboolean large,
			    GMathmlBbox const *stretch_bbox)
{
	const GMathmlElement *element;
	const GMathmlStretchyGlyph *glyph;
	PangoRectangle rect, ink_rect;
	double scale_x, scale_y;
	int baseline;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element));
	g_return_if_fail (stretch_bbox != NULL);

	element = view->priv->current_element;

	if (text == NULL || !stretch_bbox->is_defined)
		return;

	if (stretch_bbox->is_defined)
		g_message ("[GMathmlView::show_operator] Stretch bbox w = %g, h = %g, d = %g",
			   stretch_bbox->width, stretch_bbox->height, stretch_bbox->depth);

	cairo_set_source_rgba (view->priv->cairo,
			       element->math_color.red,
			       element->math_color.green,
			       element->math_color.blue,
			       element->math_color.alpha);

	glyph = gmathml_view_find_strecthy_glyph (text);
	if (glyph == NULL) {
		gmathml_view_update_layout (view, text, large, &ink_rect, &rect, &baseline);
	} else {
		PangoLayoutIter *iter;
		unsigned int i;
		gboolean found = FALSE;

		pango_font_description_set_size (view->priv->font_description,
						 element->math_size * PANGO_SCALE *
						 (large ? GMATHML_LARGE_OP_SCALE : 1.0));

		for (i = 0; glyph->glyphs[i].utf8[0] != '\x0' && !found; i++) {
			pango_font_description_set_family (view->priv->font_description, glyph->glyphs[i].font);
			pango_font_description_set_style (view->priv->font_description, PANGO_STYLE_NORMAL);
			pango_layout_set_text (view->priv->pango_layout, glyph->glyphs[i].utf8, -1);
			pango_layout_set_font_description (view->priv->pango_layout, view->priv->font_description);
			pango_layout_get_extents (view->priv->pango_layout, &ink_rect, NULL);

			g_message ("Glyph #%i -> width = %g, height = %g", i,
				   pango_units_to_double (ink_rect.width),
				   pango_units_to_double (ink_rect.height));

			switch (glyph->direction) {
				case GMATHML_STRETCH_DIRECTION_VERTICAL:
					if (pango_units_to_double (ink_rect.height) >
					    stretch_bbox->height + stretch_bbox->depth)
						found = TRUE;
					break;
				case GMATHML_STRETCH_DIRECTION_HORIZONTAL:
					if (pango_units_to_double (ink_rect.width) > stretch_bbox->width)
						found = TRUE;
					break;
				default:
					if (pango_units_to_double (ink_rect.height) >
					    stretch_bbox->height + stretch_bbox->depth &&
					    pango_units_to_double (ink_rect.width) > stretch_bbox->width)
						found = TRUE;
					break;
			}
		}

		if (found)
			g_message ("Found sized glyph #%i", i);

		iter = pango_layout_get_iter (view->priv->pango_layout);
		baseline = pango_layout_iter_get_baseline (iter);
		pango_layout_iter_free (iter);

/*                cairo_set_source_rgba (view->priv->cairo, 1.0, 0.0, 0.0, 1.0);*/
	}

	if (ink_rect.width == 0 || ink_rect.height == 0)
		return;

	scale_x = stretch_bbox->width / pango_units_to_double (ink_rect.width);
	scale_y = (stretch_bbox->height + stretch_bbox->depth) / pango_units_to_double (ink_rect.height);

	g_message ("x_scale = %g, y_scale = %g", scale_x, scale_y);

	cairo_save (view->priv->cairo);
	cairo_move_to (view->priv->cairo, x , y);
	cairo_scale (view->priv->cairo, scale_x, scale_y);
	cairo_rel_move_to (view->priv->cairo,
			   - pango_units_to_double (ink_rect.x),
			   - pango_units_to_double (ink_rect.y));
	pango_cairo_show_layout (view->priv->cairo, view->priv->pango_layout);
	cairo_restore (view->priv->cairo);

	g_message ("Show operator %s (cr status = %d)", text, cairo_status (view->priv->cairo));
}

void
gmathml_view_show_background (GMathmlView *view, double x, double y, const GMathmlBbox *bbox)
{
	const GMathmlElement *element;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element));

	element = view->priv->current_element;

	cairo_set_source_rgba (view->priv->cairo,
			       element->math_background.red,
			       element->math_background.green,
			       element->math_background.blue,
			       element->math_background.alpha);
	cairo_rectangle (view->priv->cairo, x, y - bbox->height, bbox->width, bbox->depth + bbox->height);
	cairo_fill (view->priv->cairo);
}

void
gmathml_view_show_bbox (GMathmlView *view, double x, double y, const GMathmlBbox *bbox)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (view->priv->debug) {
		cairo_move_to (view->priv->cairo, x, y);
		cairo_set_source_rgba (view->priv->cairo, 0,0,1,0.1);
		cairo_rectangle (view->priv->cairo, x, y, bbox->width, -bbox->height);
		cairo_rectangle (view->priv->cairo, x, y, bbox->width, bbox->depth);
		cairo_fill (view->priv->cairo);
	}
}

static gboolean
_emit_stroke_attributes (GMathmlView *view, GMathmlLine line)
{
	const GMathmlElement *element;
	double dashes[2] = {3.0, 2.0};

	g_return_val_if_fail (GMATHML_IS_ELEMENT (view->priv->current_element), FALSE);

	element = view->priv->current_element;

	switch (line) {
		case GMATHML_LINE_DASHED:
			cairo_set_dash (view->priv->cairo, dashes, 2, 0.0);
			break;
		case GMATHML_LINE_SOLID:
			cairo_set_dash (view->priv->cairo, NULL, 0, 0.0);
			break;
		default:
			return FALSE;
	}

	cairo_set_line_width (view->priv->cairo, 1.0);
	cairo_set_source_rgba (view->priv->cairo,
			       element->math_color.red,
			       element->math_color.green,
			       element->math_color.blue,
			       element->math_color.alpha);

	return TRUE;
}

void
gmathml_view_show_rectangle (GMathmlView *view,
			     double x, double y, double width, double height,
			     GMathmlLine line)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (_emit_stroke_attributes (view, line)) {
		cairo_rectangle (view->priv->cairo, x, y, width, height);
		cairo_stroke (view->priv->cairo);
	}
}

void
gmathml_view_show_line (GMathmlView *view,
			double x0, double y0, double x1, double y1,
			GMathmlLine line)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (_emit_stroke_attributes (view, line)) {
		cairo_move_to (view->priv->cairo, x0, y0);
		cairo_line_to (view->priv->cairo, x1, y1);
		cairo_stroke (view->priv->cairo);
	}
}

void
gmathml_view_push_element (GMathmlView *view, const GMathmlElement *element)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (GMATHML_IS_ELEMENT (element));

	view->priv->elements = g_slist_prepend (view->priv->elements, (void *) element);
	view->priv->current_element = element;
}

void
gmathml_view_pop_element (GMathmlView *view)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (view->priv->elements != NULL);

	view->priv->elements = g_slist_remove (view->priv->elements, view->priv->elements->data);

	if (view->priv->elements != NULL)
		view->priv->current_element = view->priv->elements->data;
	else
		view->priv->current_element = NULL;
}

void
gmathml_view_draw_fraction_line (GMathmlView *view,
				 double x, double y,
				 double width, double thickness,
				 GMathmlColor *color)
{
	cairo_t *cairo;
	int rounded_thickness;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	cairo = view->priv->cairo;

	if (view->priv->is_vector)
		rounded_thickness = thickness;
	else {
		rounded_thickness = thickness + 0.5;
		if ((rounded_thickness % 2) == 0) {
			y = (int) (y + 0.5);
			x = (int) (x + 0.5);
		} else {
			y = +0.5 + (int ) y;
			x = +0.5 + (int ) x;
		}
	}

	g_message ("[View::draw_fraction_line] y = %g, thickness = %d", y, rounded_thickness);

	cairo_set_line_width (cairo, rounded_thickness);
	cairo_set_source_rgba (cairo, color->red, color->green, color->blue,
			       thickness < 1.0 ? thickness : color->alpha);
	cairo_move_to (cairo, x, y);
	cairo_line_to (cairo, x + width, y);
	cairo_stroke (cairo);
}

void
gmathml_view_draw_root (GMathmlView *view,
			double x, double y,
			double width, double height,
			double top_width,
			double thickness,
			GMathmlColor *color)
{
	cairo_t *cairo;
	int rounded_thickness;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	cairo = view->priv->cairo;

	rounded_thickness = thickness + 0.5;
	if (rounded_thickness <= 0)
		rounded_thickness = 1;

	cairo_save (cairo);
	cairo_set_line_cap (cairo, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_width (cairo, 2.0 * thickness);

	cairo_set_source_rgba (cairo, color->red, color->green, color->blue, color->alpha);
	cairo_move_to (cairo, x, y - height * 0.5);
	cairo_line_to (cairo, x + width * 0.3, y);
	cairo_stroke (cairo);

	cairo_set_line_width (cairo, thickness);
	cairo_line_to (cairo, x + width * 0.3, y);
	cairo_line_to (cairo, x + width, y - height);
	cairo_stroke (cairo);

	cairo_set_line_width (cairo, rounded_thickness);
	if (rounded_thickness % 2 == 0) {
		cairo_move_to (cairo, x + width, (int) (y - height - rounded_thickness * 0.5 + 0.5));
		cairo_line_to (cairo, x + width + top_width, (int) (y - height -rounded_thickness * 0.5 + 0.5));
	} else {
		cairo_move_to (cairo, x + width, +0.5 + (int) (y - height - rounded_thickness * 0.5));
		cairo_line_to (cairo, x + width + top_width, +0.5 + (int) (y - height -rounded_thickness * 0.5));
	}
	cairo_stroke (cairo);

	cairo_restore (cairo);
}

void
gmathml_view_measure (GMathmlView *view, double *width, double *height)
{
	GDomElement *root;
	const GMathmlBbox *bbox;

	if (width != NULL)
		*width = 0.0;
	if (height != NULL)
		*height = 0.0;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_MATH_ELEMENT (root));

	gmathml_element_update (GMATHML_ELEMENT (root),
				gmathml_math_element_get_default_style (GMATHML_MATH_ELEMENT (root)));

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view, NULL);

	if (bbox != NULL) {
		if (width != NULL)
			*width = bbox->width;
		if (height != NULL)
			*height = bbox->height + bbox->depth;
	}
}

void
gmathml_view_render (GMathmlView *view)
{
	GDomElement *root;
	cairo_t *cairo;
	const GMathmlBbox *bbox;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_MATH_ELEMENT (root));

	cairo = view->priv->cairo;

	view->priv->current_element = NULL;
	view->priv->elements = NULL;

	gmathml_element_update (GMATHML_ELEMENT (root),
				gmathml_math_element_get_default_style (GMATHML_MATH_ELEMENT (root)));

	bbox = gmathml_element_measure (GMATHML_ELEMENT (root), view, NULL);

	if (bbox->is_defined)
		g_message ("[View::render] bbox = %g, %g, %g", bbox->width, bbox->height, bbox->depth);
	else
		g_message ("[View::render] bbox not defined");

	gmathml_element_layout (GMATHML_ELEMENT (root), view, 0, 0, bbox);

	cairo_save (cairo);
	cairo_translate (cairo, 0, bbox->height);
	gmathml_element_render (GMATHML_ELEMENT (root), view);
	cairo_restore (cairo);

	if (view->priv->elements != NULL) {
		g_warning ("[View::render] dangling elements (check push/pop element calls)");
		g_slist_free (view->priv->elements);
		view->priv->elements = NULL;
	}
}

void
gmathml_view_set_debug (GMathmlView *view, gboolean debug)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	view->priv->debug = debug;
}

void
gmathml_view_set_cairo (GMathmlView *view, cairo_t *cairo)
{
	PangoContext *context;
	cairo_font_options_t *font_options;
	cairo_surface_t *surface;

	g_return_if_fail (GMATHML_IS_VIEW (view));
	g_return_if_fail (cairo != NULL);

	if (view->priv->cairo != NULL) {
		cairo_destroy (view->priv->cairo);
		g_object_unref (view->priv->pango_layout);
	}

	cairo_reference (cairo);
	view->priv->cairo = cairo;
	view->priv->pango_layout = pango_cairo_create_layout (cairo);
	view->priv->font_description = pango_font_description_new();

	surface = cairo_get_target (cairo);

	view->priv->is_vector = cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_SVG;

	context = pango_layout_get_context (view->priv->pango_layout);

	font_options = cairo_font_options_create ();

	cairo_font_options_set_antialias (font_options, CAIRO_ANTIALIAS_GRAY);

	pango_cairo_context_set_font_options (context, font_options);

	cairo_font_options_destroy (font_options);
}

GMathmlView *
gmathml_view_new (GMathmlDocument *document, cairo_t *cairo)
{
	GMathmlView *view;

	g_return_val_if_fail (GMATHML_IS_DOCUMENT (document), NULL);

	view = g_object_new (GMATHML_TYPE_VIEW, NULL);

	g_object_ref (document);
	view->priv->document = document;

	gmathml_view_set_cairo (view, cairo);

	return view;
}

static void
gmathml_view_init (GMathmlView *view)
{
	view->priv = G_TYPE_INSTANCE_GET_PRIVATE (view, GMATHML_TYPE_VIEW, GMathmlViewPrivate);
}

static void
gmathml_view_finalize (GObject *object)
{
	GMathmlView *view = GMATHML_VIEW (object);

	g_object_unref (view->priv->document);
	g_object_unref (view->priv->pango_layout);
/*        g_object_unref (view->priv->font_description);*/
	cairo_destroy (view->priv->cairo);

	parent_class->finalize (object);
}

static void
gmathml_view_class_init (GMathmlViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = gmathml_view_finalize;

	g_type_class_add_private (object_class, sizeof (GMathmlViewPrivate));
}

G_DEFINE_TYPE (GMathmlView, gmathml_view, G_TYPE_OBJECT)
