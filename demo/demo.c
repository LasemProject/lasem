#include <gtk/gtk.h>

#include <lsm.h>
#include <lsmdom.h>
#include <cairo.h>

// Subclass GTK Window so we can store demo state

#define DEMO_TYPE_WINDOW (demo_window_get_type())
G_DECLARE_FINAL_TYPE (DemoWindow, demo_window, DEMO, WINDOW, GtkApplicationWindow)

struct _DemoWindow
{
    GtkApplicationWindow parent_instance;

    GtkWidget *lasem_view;
    GtkWidget *text_view;

    LsmDomDocument *document;
    LsmDomView *view;
    GError *error;
};

G_DEFINE_FINAL_TYPE (DemoWindow, demo_window, GTK_TYPE_APPLICATION_WINDOW)

/* see test.mml */
static const char *mml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n  <mrow>\r\n    <msub>\r\n      <mi>a</mi>\r\n      <mn>0</mn>\r\n    </msub>\r\n    <mo>+</mo>\r\n    <mfrac>\r\n      <mn>1</mn>\r\n      <mrow>\r\n        <msub>\r\n          <mi>a</mi>\r\n          <mn>1</mn>\r\n        </msub>\r\n        <mo>+</mo>\r\n        <mfrac>\r\n          <mn>1</mn>\r\n          <mrow>\r\n            <msub>\r\n              <mi>a</mi>\r\n              <mn>2</mn>\r\n            </msub>\r\n            <mo>+</mo>\r\n            <mfrac>\r\n              <mn>1</mn>\r\n\r\n              <mrow>\r\n                <msub>\r\n                  <mi>a</mi>\r\n                  <mn>3</mn>\r\n                </msub>\r\n                <mo>+</mo>\r\n                <mfrac>\r\n                  <mn>1</mn>\r\n                  <mrow>\r\n                    <msub>\r\n                      <mi>a</mi>\r\n                      <mn>4</mn>\r\n                    </msub>\r\n                  </mrow>\r\n                </mfrac>\r\n              </mrow>\r\n\r\n            </mfrac>\r\n          </mrow>\r\n        </mfrac>\r\n      </mrow>\r\n    </mfrac>\r\n  </mrow>\r\n</math>\r\n\0";

static GtkWidget *
demo_window_new (GtkApplication *app)
{
    return GTK_WIDGET (g_object_new (DEMO_TYPE_WINDOW,
                                     "application", app,
                                     NULL));
}

static void
demo_window_finalize (GObject *object)
{
    DemoWindow *self = (DemoWindow *)object;

    g_clear_pointer (&self->view, g_object_unref);
    g_clear_pointer (&self->document, g_object_unref);

    G_OBJECT_CLASS (demo_window_parent_class)->finalize (object);
}

static void
demo_window_class_init (DemoWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = demo_window_finalize;
}

static void
demo_window_render (DemoWindow *window)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (window->text_view));

    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter (buffer, &start_iter);
    gtk_text_buffer_get_end_iter (buffer, &end_iter);

    char *text = gtk_text_buffer_get_text (buffer, &start_iter, &end_iter, false);

    if (window->view)
    {
        g_clear_pointer (&window->view, g_object_unref);
        g_clear_pointer (&window->document, g_object_unref);
        g_clear_pointer (&window->error, g_object_unref);
    }

    GError *error = NULL;
    window->document = lsm_dom_document_new_from_memory (text, -1, &error);

    if (error != NULL)
    {
        GtkWidget *dlg = gtk_message_dialog_new (GTK_WINDOW (window),
                                                 GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Could not read MathML: %s\n",
                                                 error->message);

        gtk_window_present (GTK_WINDOW (dlg));
        g_signal_connect (dlg, "response", G_CALLBACK (gtk_window_destroy), NULL);

        g_error_free (error);
    }
    else
    {
        window->view = lsm_dom_document_create_view (window->document);
    }

    gtk_widget_queue_draw (window->lasem_view);
}

static void
lasem_view_draw (GtkDrawingArea *drawing_area,
                 cairo_t        *cr,
                 int             width,
                 int             height,
                 DemoWindow     *window)
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
demo_window_init (DemoWindow *self)
{
    gtk_window_set_title (GTK_WINDOW (self), "Lasem Demo");
    gtk_window_set_default_size (GTK_WINDOW (self), 800, 600);

    GtkWidget *header_bar = gtk_header_bar_new ();
    gtk_window_set_titlebar (GTK_WINDOW (self), header_bar);

    GtkWidget *render_btn = gtk_button_new_with_label ("Render");
    gtk_widget_add_css_class (render_btn, "suggested-action");
    g_signal_connect_swapped (render_btn, "clicked", G_CALLBACK (demo_window_render), self);
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), render_btn);

    GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous (GTK_BOX (hbox), TRUE);
    gtk_window_set_child (GTK_WINDOW (self), hbox);

    GtkWidget *text_view = gtk_text_view_new ();
    self->text_view = text_view;

    // Set placeholder contents
    gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view)), mml, -1);

    GtkWidget *scroll_area = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scroll_area), text_view);
    gtk_box_append (GTK_BOX (hbox), scroll_area);

    GtkWidget *lasem_view = gtk_drawing_area_new ();
    gtk_box_append (GTK_BOX (hbox), lasem_view);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (lasem_view),
                                    (GtkDrawingAreaDrawFunc) lasem_view_draw,
                                    self, NULL);
    self->lasem_view = lasem_view;

    demo_window_render (self);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    // Create and show a new DemoWindow instance
    GtkWidget *window = demo_window_new(app);
    gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("io.github.LasemProject.LasemDemo", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
