#include <lsm.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
	LsmCss *css;

	printf ("%s\n", CSS_TEST_FILE);

	css = lsm_css_new_from_path (CSS_TEST_FILE, NULL);

	g_object_unref (css);

	return EXIT_SUCCESS;
}
