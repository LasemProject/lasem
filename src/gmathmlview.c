/* gmathmlview.c
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

#include <gmathmlview.h>
#include <gmathmldocument.h>
#include <gmathmlelement.h>

static GObjectClass *parent_class;

struct _GMathmlViewPrivate {

	GMathmlDocument *document;

	PangoLayout *pango_layout;
	PangoContext *pango_context;

	/* cairo != NULL only during rendering */
	cairo_t *cairo;
};

void
gmathml_view_measure_text (GMathmlView *view, char const *text, GMathmlBbox *bbox)
{
	PangoRectangle rect;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	if (text == NULL)
		return;

	g_return_if_fail (bbox != NULL);

	pango_layout_set_text (view->priv->pango_layout, text, -1);
	pango_layout_get_extents (view->priv->pango_layout, NULL, &rect);

	bbox->width = pango_units_to_double (rect.width);
	bbox->ascent = pango_units_to_double (PANGO_ASCENT (rect));
	bbox->descent = pango_units_to_double (PANGO_DESCENT (rect));
}

void
gmathml_view_show_text (GMathmlView *view, char const *text)
{
	PangoLayoutLine *line;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	g_message ("View: show_text %s", text);

	pango_layout_set_text (view->priv->pango_layout, text, -1);
	line = pango_layout_get_line_readonly (view->priv->pango_layout, 0);
	pango_cairo_show_layout_line (view->priv->cairo, line);
}

void
gmathml_view_rel_move_to (GMathmlView *view, double dx, double dy)
{
	g_return_if_fail (GMATHML_IS_VIEW (view));

	g_message ("View: move_to %g, %g", dx, dy);

	cairo_rel_move_to (view->priv->cairo, dx, dy);
}

void
gmathml_view_render (GMathmlView *view, cairo_t *cr)
{
	GDomElement *root;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->priv->document));
	g_return_if_fail (GMATHML_IS_ELEMENT (root));

	view->priv->cairo = cr;

	gmathml_element_measure (GMATHML_ELEMENT (root), view);

	gmathml_element_layout (GMATHML_ELEMENT (root), view);

	gmathml_element_render (GMATHML_ELEMENT (root), view);

	view->priv->cairo = NULL;
}

GMathmlView *
gmathml_view_new (GMathmlDocument *document)
{
	GMathmlView *view;

	g_return_val_if_fail (GMATHML_IS_DOCUMENT (document), NULL);

	view = g_object_new (GMATHML_TYPE_VIEW, NULL);

	g_object_ref (document);
	view->priv->document = document;

	return view;
}

static void
gmathml_view_init (GMathmlView *view)
{
	PangoCairoFontMap *fontmap;

	view->priv = G_TYPE_INSTANCE_GET_PRIVATE (view, GMATHML_TYPE_VIEW, GMathmlViewPrivate);

	fontmap = PANGO_CAIRO_FONT_MAP (pango_cairo_font_map_get_default ());
	view->priv->pango_context = pango_cairo_font_map_create_context (fontmap);
	view->priv->pango_layout = pango_layout_new (view->priv->pango_context);
}

static void
gmathml_view_finalize (GObject *object)
{
	GMathmlView *view = GMATHML_VIEW (object);

	g_object_unref (view->priv->document);
	g_object_unref (view->priv->pango_layout);
	g_object_unref (view->priv->pango_context);

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
