#include "fig.h"
#include "fig-util.h"

static void
test_fig_util_get_command_name (void)
{
   const gchar *args1[] = { "dummy", "--some", "--value", "--help", "blah", NULL };
   const gchar *args2[] = { "dummy", "--some", "init", "--help", NULL };
   const gchar *args3[] = { "dummy", "--some=", "init", "--help", NULL };
   const gchar *args4[] = { "dummy", "--some=abc", "init", "--help", NULL };
   const gchar *args5[] = { "dummy", "--project-dir", "init", "--help", NULL };
   const gchar *args6[] = { "dummy", "--project-dir=", "init", "--help", NULL };
   const gchar *args7[] = { "dummy", "--project-dir=abc", "init", "--help", NULL };
   const gchar *ret;

   ret = fig_util_get_command_name ((gchar **)args1);
   g_assert (!ret);

   ret = fig_util_get_command_name ((gchar **)args2);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name ((gchar **)args3);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name ((gchar **)args4);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name ((gchar **)args5);
   g_assert (!ret);

   ret = fig_util_get_command_name ((gchar **)args6);
   g_assert_cmpstr (ret, ==, "init");

   ret = fig_util_get_command_name ((gchar **)args7);
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
