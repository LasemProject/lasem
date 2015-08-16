#include <glib.h>
#include <lsmsvgfiltersurface.h>

static void
surface (void)
{
	LsmSvgFilterSurface *surface;
	const LsmBox *subregion;

	surface = lsm_svg_filter_surface_new ("test", 320, 200, NULL);

	g_assert (surface != NULL);
	g_assert_cmpstr (lsm_svg_filter_surface_get_name (surface),  ==, "test");

	subregion = lsm_svg_filter_surface_get_subregion (surface);

	g_assert (subregion != NULL);

	lsm_svg_filter_surface_ref (surface);
	lsm_svg_filter_surface_unref (surface);

	lsm_svg_filter_surface_unref (surface);

	g_test_expect_message ("Lasem", G_LOG_LEVEL_CRITICAL, "*assertion*ref_count*failed");

	lsm_svg_filter_surface_unref (surface);

	g_test_assert_expected_messages ();
}

static void
similar (void)
{
	LsmSvgFilterSurface *surface_a;
	LsmSvgFilterSurface *surface_b;

	surface_a = lsm_svg_filter_surface_new ("test_a", 320, 200, NULL);
	surface_b = lsm_svg_filter_surface_new_similar ("test_b", surface_a, NULL);

	g_assert_cmpstr (lsm_svg_filter_surface_get_name (surface_a),  ==, "test_a");
	g_assert_cmpstr (lsm_svg_filter_surface_get_name (surface_b),  ==, "test_b");

	lsm_svg_filter_surface_unref (surface_a);
	lsm_svg_filter_surface_unref (surface_b);
}

static void
operations (LsmSvgFilterSurface *input_1, LsmSvgFilterSurface *input_2, LsmSvgFilterSurface *output)
{
	cairo_matrix_t transform;

	cairo_matrix_init_identity (&transform);

	lsm_svg_filter_surface_alpha (input_1, output);
	lsm_svg_filter_surface_blend (input_1, input_2, output, LSM_SVG_BLENDING_MODE_XOR);
	lsm_svg_filter_surface_blur (input_1, output, 0.0, 0.0);
	lsm_svg_filter_surface_blur (input_1, output, 1.0, 1.0);
	lsm_svg_filter_surface_blur (input_1, output, 10.0, 10.0);
	lsm_svg_filter_surface_blur (input_1, output, 1000.0, 1000.0);
	lsm_svg_filter_surface_color_matrix (input_1, output, LSM_SVG_COLOR_FILTER_TYPE_HUE_ROTATE, 0, NULL);
	lsm_svg_filter_surface_convolve_matrix (input_1, output, 0, 0, 0, NULL, 1.0, 0.0, 0, 0, LSM_SVG_EDGE_MODE_NONE, TRUE);
	lsm_svg_filter_surface_displacement_map (input_1, input_2, output, 2.0, 2.0,
						 LSM_SVG_CHANNEL_SELECTOR_RED, LSM_SVG_CHANNEL_SELECTOR_GREEN);
	lsm_svg_filter_surface_displacement_map (input_1, input_2, output, 2.0, 3.0,
						 LSM_SVG_CHANNEL_SELECTOR_BLUE, LSM_SVG_CHANNEL_SELECTOR_ALPHA);
	lsm_svg_filter_surface_displacement_map (input_1, input_2, output, 0.0, 0.0,
						 LSM_SVG_CHANNEL_SELECTOR_ALPHA, LSM_SVG_CHANNEL_SELECTOR_ALPHA);
	lsm_svg_filter_surface_flood (output, 1.0, 0.0, 0.5, 0.25);
	lsm_svg_filter_surface_merge (input_1, output);
	lsm_svg_filter_surface_morphology (input_1, output, LSM_SVG_MORPHOLOGY_OPERATOR_ERODE, 1, 1);
	lsm_svg_filter_surface_morphology (input_1, output, LSM_SVG_MORPHOLOGY_OPERATOR_DILATE, 1, 1);
	lsm_svg_filter_surface_offset (input_1, output, 10, 10);
	lsm_svg_filter_surface_offset (input_1, output, -10, -10);
	lsm_svg_filter_surface_offset (input_1, output, -1000, -1000);
	lsm_svg_filter_surface_tile (input_1, output);
	lsm_svg_filter_surface_turbulence (output, 10.0, 10.0, 2, 1.0, LSM_SVG_STITCH_TILES_STITCH, LSM_SVG_TURBULENCE_TYPE_FRACTAL_NOISE,
					   &transform);
}

static void
processing (void)
{
	LsmSvgFilterSurface *input_1;
	LsmSvgFilterSurface *input_2;
	LsmSvgFilterSurface *output;

	input_1 = lsm_svg_filter_surface_new ("input_1", 320, 200, NULL);
	input_2 = lsm_svg_filter_surface_new_similar ("input_2", input_1, NULL);
	output = lsm_svg_filter_surface_new_similar ("output", input_1, NULL);

	operations (input_1, input_2, output);

	lsm_svg_filter_surface_unref (input_1);
	lsm_svg_filter_surface_unref (input_2);
	lsm_svg_filter_surface_unref (output);
}

static void
processing_mismatch (void)
{
	LsmSvgFilterSurface *input_1;
	LsmSvgFilterSurface *input_2;
	LsmSvgFilterSurface *output;

	input_1 = lsm_svg_filter_surface_new ("input_1", 320, 200, NULL);
	input_2 = lsm_svg_filter_surface_new ("input_2", 100, 10, NULL);
	output = lsm_svg_filter_surface_new ("output", 10, 5, NULL);

	operations (input_1, input_2, output);

	lsm_svg_filter_surface_unref (input_1);
	lsm_svg_filter_surface_unref (input_2);
	lsm_svg_filter_surface_unref (output);
}

static void
processing_null (void)
{
	unsigned int i;

	for (i = 0; i < 20; i++)
		g_test_expect_message ("Lasem", G_LOG_LEVEL_CRITICAL, "*assertion*NULL*failed");

	operations (NULL, NULL, NULL);

	for (i = 0; i < 20; i++)
		g_test_assert_expected_messages ();
}

int
main (int argc, char *argv[])
{
	int result;

	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/filter/surface", surface);
	g_test_add_func ("/filter/similar", similar);
	g_test_add_func ("/filter/processing", processing);
	g_test_add_func ("/filter/processing_mismatch", processing_mismatch);
	g_test_add_func ("/filter/processing_null", processing_null);

#if !GLIB_CHECK_VERSION(2,36,0)
	g_type_init ();
#endif

	result = g_test_run ();

	lsm_shutdown ();

	return result;
}
