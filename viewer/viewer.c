/* Lasem - SVG and Mathml library
 *
 * Copyright © 2010 Emmanuel Pacaud
 * Copyright © 2023 Matthew Jakeman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Matthew Jakeman <mattjakemandev@gmail.com>
 */

#include <gtk/gtk.h>

#include <lsm.h>
#include <lsmdom.h>
#include <cairo.h>

#define VIEWER_TYPE_WINDOW (viewer_window_get_type())
G_DECLARE_FINAL_TYPE (ViewerWindow, viewer_window, VIEWER, WINDOW, GtkApplicationWindow)

struct _ViewerWindow
{
    GtkApplicationWindow parent_instance;

    GtkWidget *lasem_view;

    LsmDomDocument *document;
    LsmDomView *view;
    GError *error;
};

G_DEFINE_FINAL_TYPE (ViewerWindow, viewer_window, GTK_TYPE_APPLICATION_WINDOW)

static GtkWidget *
viewer_window_new (GtkApplication *app)
{
    return GTK_WIDGET (g_object_new (VIEWER_TYPE_WINDOW,
                                     "application", app,
                                     NULL));
}

static void
viewer_window_finalize (GObject *object)
{
    ViewerWindow *self = (ViewerWindow *)object;

    g_clear_pointer (&self->view, g_object_unref);
    g_clear_pointer (&self->document, g_object_unref);

    G_OBJECT_CLASS (viewer_window_parent_class)->finalize (object);
}

static void
viewer_window_class_init (ViewerWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = viewer_window_finalize;
}

static void
display_error (ViewerWindow *window, GError *error, const char *error_text)
{
    char *message;

    message = (error != NULL)
        ? error->message
        : "Undetermined Error";

    GtkWidget *dlg = gtk_message_dialog_new (GTK_WINDOW (window),
                                             GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_OK,
                                             "%s: %s\n",
                                             error_text,
                                             message);

    gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
    gtk_window_set_transient_for (GTK_WINDOW (dlg), GTK_WINDOW (window));
    g_signal_connect (dlg, "response", G_CALLBACK (gtk_window_destroy), NULL);

    gtk_window_present (GTK_WINDOW (dlg));
}

static void
viewer_window_load_from_file (ViewerWindow *window, const char *path)
{
    GError *error = NULL;

    gchar *contents;
    gsize length;

    if (!g_file_get_contents (path, &contents, &length, &error))
    {
        display_error (window, error, "Could not read file");

        if (error != NULL)
            g_error_free (error);

        return;
    }

    if (window->view)
    {
        g_clear_pointer (&window->view, g_object_unref);
        g_clear_pointer (&window->document, g_object_unref);
        g_clear_pointer (&window->error, g_object_unref);
    }

    window->document = lsm_dom_document_new_from_memory (contents, length, &error);

    if (!LSM_IS_DOM_DOCUMENT (window->document))
    {
        display_error (window, error, "Could not read MathML/SVG");

        if (error != NULL)
            g_error_free (error);

        return;
    }

    window->view = lsm_dom_document_create_view (window->document);
    gtk_widget_queue_draw (window->lasem_view);
}

static void
lasem_view_draw (GtkDrawingArea *drawing_area,
                 cairo_t        *cr,
                 int             width,
                 int             height,
                 ViewerWindow     *window)
{
    if (window->view == NULL)
        return;

    int margin = 25;

    guint content_width = 0;
    guint content_height = 0;
    guint baseline = 0;
    lsm_dom_view_get_size_pixels (window->view, &content_width, &content_height, &baseline);

    float multiplier = ((float)width - (2 * margin)) / content_width;
    lsm_dom_view_set_resolution (window->view, multiplier * LSM_DOM_VIEW_DEFAULT_RESOLUTION);

    lsm_dom_view_render (window->view, cr, margin, margin);
}

static void
viewer_on_file_open (GtkFileDialog *dialog,
                     GAsyncResult  *result,
                     ViewerWindow  *self)
{
    GFile *file;
    GError *error = NULL;

    file = gtk_file_dialog_open_finish (dialog, result, &error);

    if (G_IS_FILE (file))
    {
        char *path;

        path = g_file_get_path (file);
        viewer_window_load_from_file (self, path);
        g_free (path);
    }
    else
    {
        display_error (GTK_WINDOW (self), error, "File Chooser error");

        if (error != NULL)
            g_error_free (error);
    }
}

static void
viewer_show_file_chooser (ViewerWindow *self)
{
    GtkFileDialog *dialog;
    GtkFileFilter *filter;

    filter = gtk_file_filter_new ();
    gtk_file_filter_add_mime_type (filter, "text/mathml");
    gtk_file_filter_add_mime_type (filter, "image/svg+xml");
    gtk_file_filter_set_name (filter, "MathML/SVG Files");

    dialog = gtk_file_dialog_new ();
    gtk_file_dialog_set_modal (dialog, TRUE);
    gtk_file_dialog_set_default_filter (dialog, filter);

    gtk_file_dialog_open (dialog, GTK_WINDOW (self),
                          NULL, (GAsyncReadyCallback) viewer_on_file_open,
                          self);
}

static void
viewer_window_init (ViewerWindow *self)
{
    gtk_window_set_title (GTK_WINDOW (self), "Lasem Viewer");
    gtk_window_set_default_size (GTK_WINDOW (self), 800, 600);
    gtk_window_set_resizable (GTK_WINDOW (self), FALSE);

    GtkWidget *header_bar = gtk_header_bar_new ();
    gtk_window_set_titlebar (GTK_WINDOW (self), header_bar);

    GtkWidget *open_btn = gtk_button_new_with_label ("Open");
    gtk_widget_add_css_class (open_btn, "suggested-action");
    g_signal_connect_swapped (open_btn, "clicked", G_CALLBACK (viewer_show_file_chooser), self);
    gtk_header_bar_pack_start (header_bar, open_btn);

    GtkWidget *lasem_view = gtk_drawing_area_new ();
    gtk_window_set_child (GTK_WINDOW (self), lasem_view);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (lasem_view),
                                    (GtkDrawingAreaDrawFunc) lasem_view_draw,
                                    self, NULL);
    self->lasem_view = lasem_view;

    viewer_show_file_chooser (self);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    // Create and show a new ViewerWindow instance
    GtkWidget *window = viewer_window_new(app);
    gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.lasem.Viewer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
