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
   gint i;

   ret = fig_util_get_command_name ((gchar **)args1, &i);
   g_assert (!ret);
   g_assert_cmpint (i, ==, -1);

   ret = fig_util_get_command_name ((gchar **)args2, &i);
   g_assert_cmpstr (ret, ==, "init");
   g_assert_cmpint (i, ==, 2);

   ret = fig_util_get_command_name ((gchar **)args3, &i);
   g_assert_cmpstr (ret, ==, "init");
   g_assert_cmpint (i, ==, 2);

   ret = fig_util_get_command_name ((gchar **)args4, &i);
   g_assert_cmpstr (ret, ==, "init");
   g_assert_cmpint (i, ==, 2);

   ret = fig_util_get_command_name ((gchar **)args5, &i);
   g_assert (!ret);
   g_assert_cmpint (i, ==, -1);

   ret = fig_util_get_command_name ((gchar **)args6, &i);
   g_assert_cmpstr (ret, ==, "init");
   g_assert_cmpint (i, ==, 2);

   ret = fig_util_get_command_name ((gchar **)args7, &i);
   g_assert_cmpstr (ret, ==, "init");
   g_assert_cmpint (i, ==, 2);
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
