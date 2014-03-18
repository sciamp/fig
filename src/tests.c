#include "fig.h"
#include "fig-util.h"

static void
test_fig_util_get_command_name (void)
{
   const gchar *args1[] = { "dummy", "--some", "--value", "--help", "blah" };
   const gchar *args2[] = { "dummy", "--some", "init", "--help" };
   const gchar *args3[] = { "dummy", "--some=", "init", "--help" };
   const gchar *args4[] = { "dummy", "--some=abc", "init", "--help" };
   const gchar *args5[] = { "dummy", "--project-dir", "init", "--help" };
   const gchar *args6[] = { "dummy", "--project-dir=", "init", "--help" };
   const gchar *args7[] = { "dummy", "--project-dir=abc", "init", "--help" };
   const gchar *ret;

   ret = fig_util_get_command_name (G_N_ELEMENTS (args1), (gchar **)args1);
   g_assert (!ret);

   ret = fig_util_get_command_name (G_N_ELEMENTS (args2), (gchar **)args2);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name (G_N_ELEMENTS (args3), (gchar **)args3);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name (G_N_ELEMENTS (args4), (gchar **)args4);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name (G_N_ELEMENTS (args5), (gchar **)args5);
   g_assert (!ret);

   ret = fig_util_get_command_name (G_N_ELEMENTS (args6), (gchar **)args6);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name (G_N_ELEMENTS (args7), (gchar **)args7);
   g_assert_cmpstr (ret, ==, "init");
}

gint
main (gint    argc,
      gchar **argv)
{
   g_test_init (&argc, &argv, NULL);
   g_test_add_func ("/fig/util/get_command_name",
                    test_fig_util_get_command_name);
   return g_test_run ();
}
