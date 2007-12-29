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

void
gmathml_view_render (GMathmlView *view, cairo_t *cr)
{
	GDomElement *root;

	g_return_if_fail (GMATHML_IS_VIEW (view));

	root = gdom_document_get_document_element (GDOM_DOCUMENT (view->document));
	g_return_if_fail (GMATHML_IS_ELEMENT (root));

	gmathml_element_layout (GMATHML_ELEMENT (root), view);

	gmathml_element_render (GMATHML_ELEMENT (root), view);
}

GMathmlView *
gmathml_view_new (GMathmlDocument *document)
{
	GMathmlView *view;

	g_return_val_if_fail (GMATHML_IS_DOCUMENT (document), NULL);

	view = g_object_new (GMATHML_TYPE_VIEW, NULL);

	g_object_ref (document);
	view->document = document;

	return view;
}

static void
gmathml_view_init (GMathmlView *view)
{
	PangoFontMap *fontmap;

	fontmap = pango_cairo_font_map_get_default ();
	view->pango_context = pango_cairo_font_map_create_context (fontmap);
	view->pango_layout = pango_layout_new (view->pango_context);
}

static void
gmathml_view_finalize (GObject *object)
{
	GMathmlView *view = GMATHML_VIEW (object);

	g_object_unref (view->document);
	g_object_unref (view->pango_layout);
	g_object_unref (view->pango_context);

	parent_class->finalize (object);
}

static void
gmathml_view_class_init (GMathmlViewClass *view_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (view_class);

	parent_class = g_type_class_peek_parent (view_class);

	object_class->finalize = gmathml_view_finalize;
}

G_DEFINE_TYPE (GMathmlView, gmathml_view, G_TYPE_OBJECT)
