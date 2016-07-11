#include <lsmsvgruler.h>
#include <lsmsvgmatrix.h>
#include <lsmutils.h>

struct _LsmSvgRuler {
	double resolution_ppi;

	LsmSvgStyle *style;
	LsmSvgMatrix *matrix;
	LsmSvgViewbox *viewbox;

	GSList *style_stack;
	GSList *matrix_stack;
	GSList *viewbox_stack;

	volatile gint ref_count;
};

LsmSvgRuler *
lsm_svg_ruler_new (double resolution_ppi)
{
	LsmSvgRuler *ruler;

	ruler = g_slice_new0 (LsmSvgRuler);
	ruler->ref_count = 1;
	ruler->resolution_ppi = resolution_ppi;
	ruler->style = NULL;
	ruler->matrix = g_slice_new (LsmSvgMatrix);
	lsm_svg_matrix_init_identity (ruler->matrix);

	return ruler;
}

LsmSvgRuler *
lsm_svg_ruler_ref (LsmSvgRuler *ruler)
{
	g_return_val_if_fail (ruler != NULL, NULL);
	g_return_val_if_fail (g_atomic_int_get (&ruler->ref_count) > 0, ruler);
	g_atomic_int_inc (&ruler->ref_count);

	return ruler;
}

static void
_free_style (gpointer data, gpointer user_data)
{
	if (data != NULL)
		lsm_svg_style_unref (data);
}

static void
_free_matrix (gpointer data, gpointer user_data)
{
	if (data != NULL)
		g_slice_free (LsmSvgMatrix, data);
}

static void
_free_viewbox (gpointer data, gpointer user_data)
{
	if (data != NULL)
		lsm_svg_viewbox_free (data);
}

void
lsm_svg_ruler_unref (LsmSvgRuler *ruler)
{
	g_return_if_fail (g_atomic_int_get (&ruler->ref_count) > 0);
	if (g_atomic_int_dec_and_test (&ruler->ref_count)) {
		if (ruler->style_stack != NULL) {
			g_warning ("Dangling style in ruler stack");
			g_slist_foreach (ruler->style_stack, _free_style, NULL);
			g_clear_pointer (&ruler->style_stack, g_slist_free);
		}
		if (ruler->matrix_stack != NULL) {
			g_warning ("Dangling matrix in ruler stack");
			g_slist_foreach (ruler->matrix_stack, _free_matrix, NULL);
			g_clear_pointer (&ruler->matrix_stack, g_slist_free);
		}
		if (ruler->viewbox_stack != NULL) {
			g_warning ("Dangling viewbow in ruler stack");
			g_slist_foreach (ruler->viewbox_stack, _free_viewbox, NULL);
			g_clear_pointer (&ruler->viewbox_stack, g_slist_free);
		}
		_free_style (ruler->style, NULL);
		_free_matrix (ruler->matrix, NULL);
		_free_viewbox (ruler->viewbox, NULL);
		g_slice_free (LsmSvgRuler, ruler);
	}
}

void
lsm_svg_ruler_push_style (LsmSvgRuler *ruler, LsmSvgStyle *style)
{
	g_return_if_fail (ruler != NULL);
	g_return_if_fail (style != NULL);

	lsm_log_ruler ("Push style");

	/* FIXME Don't store current font_size_pixel in style */
	if (ruler->style == NULL || (style->font_size != ruler->style->font_size)) {
		LsmSvgViewbox font_viewbox;
		LsmSvgViewbox *viewbox;
		double current_font_size_px;

		if (ruler->style != NULL)
			current_font_size_px = ruler->style->font_size_px;
		else
			current_font_size_px = 0.0;

		viewbox = ruler->viewbox;
		font_viewbox.resolution_ppi = viewbox->resolution_ppi;
		font_viewbox.viewbox.x = 0;
		font_viewbox.viewbox.y = 0;
		font_viewbox.viewbox.width = current_font_size_px;
		font_viewbox.viewbox.height = current_font_size_px;

		style->font_size_px = lsm_svg_length_normalize (&style->font_size->length, &font_viewbox,
								current_font_size_px, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

		if (style->font_size_px < 0.0)
			style->font_size_px = 0.0;
		lsm_log_ruler ("Font size = %g pixels", style->font_size_px);
	} else
		style->font_size_px = ruler->style->font_size_px;

	ruler->style_stack = g_slist_prepend (ruler->style_stack, ruler->style);
	ruler->style = lsm_svg_style_ref (style);
}

void
lsm_svg_ruler_pop_style (LsmSvgRuler *ruler)
{
	g_return_if_fail (ruler != NULL);
	g_return_if_fail (ruler->style_stack != NULL);

	lsm_log_ruler ("Pop style");

	lsm_svg_style_unref (ruler->style);
	ruler->style = ruler->style_stack->data;
	ruler->style_stack = g_slist_delete_link (ruler->style_stack, ruler->style_stack);
}

LsmSvgStyle *
lsm_svg_ruler_get_style (LsmSvgRuler *ruler)
{
	g_return_val_if_fail (ruler != NULL, NULL);

	return ruler->style;
}

gboolean
lsm_svg_ruler_push_matrix (LsmSvgRuler *ruler, const LsmSvgMatrix *matrix)
{
	gboolean is_invertible;

	g_return_val_if_fail (ruler != NULL, FALSE);
	g_return_val_if_fail (matrix != NULL, FALSE);

	ruler->matrix_stack = g_slist_prepend (ruler->matrix_stack, ruler->matrix);
	ruler->matrix = g_slice_new (LsmSvgMatrix);

	is_invertible = lsm_svg_matrix_is_invertible (matrix);
	if (is_invertible)
		lsm_svg_matrix_multiply (ruler->matrix, ruler->matrix, matrix); 
	else
		lsm_svg_matrix_init (ruler->matrix, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

	return is_invertible;
}

void
lsm_svg_ruler_pop_matrix (LsmSvgRuler *ruler)
{
	g_return_if_fail (ruler != NULL);
	g_return_if_fail (ruler->matrix_stack != NULL);

	g_slice_free (LsmSvgMatrix, ruler->matrix);
	ruler->matrix = ruler->matrix_stack->data;
	ruler->matrix_stack = g_slist_delete_link (ruler->matrix_stack, ruler->matrix_stack);
}

void
lsm_svg_ruler_push_viewbox (LsmSvgRuler *ruler, const LsmBox *viewbox)
{
	g_return_if_fail (ruler != NULL);
	g_return_if_fail (viewbox != NULL);

	lsm_debug_ruler ("Push viewbox = %g, %g, %g, %g",
			 viewbox->x, viewbox->y, viewbox->width, viewbox->height);

	ruler->viewbox_stack = g_slist_prepend (ruler->viewbox_stack, ruler->viewbox);
	ruler->viewbox = lsm_svg_viewbox_new (ruler->resolution_ppi, viewbox);
}

void
lsm_svg_ruler_pop_viewbox (LsmSvgRuler *ruler)
{
	g_return_if_fail (ruler != NULL);
	g_return_if_fail (ruler->viewbox_stack != NULL);

	lsm_debug_ruler ("Pop viewbox");

	lsm_svg_viewbox_free (ruler->viewbox);
	ruler->viewbox = ruler->viewbox_stack->data;
	ruler->viewbox_stack = g_slist_delete_link (ruler->viewbox_stack, ruler->viewbox_stack);
}

double
lsm_svg_ruler_normalize_length (LsmSvgRuler *ruler, const LsmSvgLength *length, LsmSvgLengthDirection direction)
{
	g_return_val_if_fail (ruler != NULL, 0.0);

	return lsm_svg_length_normalize (length, ruler->viewbox, ruler->style->font_size_px, direction);
}

double *
lsm_svg_ruler_normalize_length_list (LsmSvgRuler *ruler, const LsmSvgLengthList *list,
				     LsmSvgLengthDirection direction, unsigned int *n_data)
{
	double *data;
	unsigned int i;

	g_return_val_if_fail (n_data != NULL, NULL);
	*n_data = 0;
	g_return_val_if_fail (ruler != NULL, NULL);
	g_return_val_if_fail (list != NULL, NULL);

	if (list->n_lengths == 0)
		return NULL;

	*n_data = list->n_lengths;
	data = g_new (double, list->n_lengths);
	for (i = 0; i < list->n_lengths; i++)
		data[i] = lsm_svg_ruler_normalize_length (ruler, &list->lengths[i], direction);

	return data;
}

