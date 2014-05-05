#include <glib.h>
#include <lsmstr.h>
#include <lsm.h>

static void
str_consolidate_test (void)
{
	char *string;

	string = g_strdup ("   consolidate \n \t   test   \t \r a b c \n ");
	lsm_str_consolidate (string);
	g_assert_cmpstr (string, ==, "consolidate test a b c");
	g_free (string);

	string = g_strdup ("    ");
	lsm_str_consolidate (string);
	g_assert_cmpstr (string, ==, "");
	g_free (string);

	string = g_strdup ("");
	lsm_str_consolidate (string);
	g_assert_cmpstr (string, ==, "");
	g_free (string);

	string = NULL;
	lsm_str_consolidate (string);
	g_assert (string == NULL);
}

int
main (int argc, char *argv[])
{
	int result;

	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/str/str-consolidate", str_consolidate_test);

#if !GLIB_CHECK_VERSION(2,36,0)
	g_type_init ();
#endif

	result = g_test_run();

	lsm_shutdown ();

	return result;
}
