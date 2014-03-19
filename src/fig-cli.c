/* fig-cli.c
 *
 * Copyright (C) 2014 Christian Hergert <christian@hergert.me>
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gio/gio.h>
#include <gio/gunixoutputstream.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fig-cli.h"
#include "fig-command.h"
#include "fig-command-info.h"
#include "fig-command-manager.h"
#include "fig-util.h"

struct _FigCliPrivate
{
   GOutputStream *stderr_stream;
   GOutputStream *stdout_stream;
};

enum
{
   PROP_0,
   PROP_STDERR_STREAM,
   PROP_STDOUT_STREAM,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigCli, fig_cli, G_TYPE_OBJECT)

static GParamSpec *gParamSpecs [LAST_PROP];

static gboolean
strv_contains (gchar       **strv,
               const gchar  *needle)
{
   gint i;

   g_return_val_if_fail (strv, FALSE);
   g_return_val_if_fail (needle, FALSE);

   for (i = 0; strv [i]; i++) {
      if (0 == strcmp (strv [i], needle)) {
         return TRUE;
      }
   }

   return FALSE;
}

FigCli *
fig_cli_new (void)
{
   return g_object_new (FIG_TYPE_CLI, NULL);
}

GOutputStream *
fig_cli_get_stderr_stream (FigCli *cli)
{
   g_return_val_if_fail (FIG_IS_CLI (cli), NULL);

   return cli->priv->stderr_stream;
}

void
fig_cli_set_stderr_stream (FigCli        *cli,
                           GOutputStream *stderr_stream)
{
   FigCliPrivate *priv;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (!stderr_stream || G_IS_OUTPUT_STREAM (stderr_stream));

   priv = cli->priv;

   if (stderr_stream != priv->stderr_stream) {
      g_clear_object (&priv->stderr_stream);
      if (stderr_stream) {
         priv->stderr_stream = g_object_ref (stderr_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (cli),
                                gParamSpecs [PROP_STDERR_STREAM]);
   }
}

GOutputStream *
fig_cli_get_stdout_stream (FigCli *cli)
{
   g_return_val_if_fail (FIG_IS_CLI (cli), NULL);

   return cli->priv->stderr_stream;
}

void
fig_cli_set_stdout_stream (FigCli        *cli,
                           GOutputStream *stdout_stream)
{
   FigCliPrivate *priv;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (!stdout_stream || G_IS_OUTPUT_STREAM (stdout_stream));

   priv = cli->priv;

   if (stdout_stream != priv->stdout_stream) {
      g_clear_object (&priv->stdout_stream);
      if (stdout_stream) {
         priv->stdout_stream = g_object_ref (stdout_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (cli),
                                gParamSpecs [PROP_STDOUT_STREAM]);
   }
}

static void
fig_cli_print_help (FigCli        *cli,
                    const gchar   *prgname,
                    GOutputStream *stream)
{
   FigCommandInfo *info;
   GString *str;
   GList *list;
   GList *iter;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (G_IS_OUTPUT_STREAM (stream));

   str = g_string_new ("Usage:\n");
   g_string_append_printf (str, "  %s ", prgname);
   g_string_append (str, "[OPTION...] <command> <args>\n"
                         "\n"
                         "Fig is a tool to help manage autotools projects.\n"
                         "\n"
                         "Help Options:\n"
                         "  --help                 Show help options\n"
                         "\n"
                         "Commands:\n");

   list = fig_command_manager_get_commands (FIG_COMMAND_MANAGER_DEFAULT);

   for (iter = list; iter; iter = iter->next) {
      info = iter->data;
      g_string_append_printf (str, "  %-22s %s\n",
                              info->name,
                              info->description);
   }

   g_string_append (str, "\n"
                         "Examples:\n"
                         "  fig --project-dir=foo init\n"
                         "\n");

   g_output_stream_write_all (stream, str->str, str->len, NULL, NULL, NULL);
   g_string_free (str, TRUE);
}

static void
fig_cli_print_stderr (FigCli      *cli,
                      const gchar *format,
                      ...) G_GNUC_PRINTF (2, 3);

static void
fig_cli_print_stderr (FigCli      *cli,
                      const gchar *format,
                      ...)
{
   va_list args;
   gchar *str;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (format);

   va_start (args, format);
   str = g_strdup_vprintf (format, args);
   va_end (args);

   if (str) {
      g_output_stream_write_all (cli->priv->stderr_stream, str,
                                 strlen (str), NULL, NULL, NULL);
   }

   g_free (str);
}

gint
fig_cli_run (FigCli  *cli,
             gint     argc,
             gchar  **argv)
{
   FigCommandInfo *info = NULL;
   FigCliPrivate *priv;
   const gchar *command_name;
   FigCommand *command = NULL;
   gboolean contains_help;
   gchar **strv;
   gchar **cmd_argv;
   gint command_index = -1;
   gint ret = EXIT_SUCCESS;
   gint i;

   g_return_val_if_fail (FIG_IS_CLI (cli), -1);
   g_return_val_if_fail (argc, -1);
   g_return_val_if_fail (argv, -1);

   priv = cli->priv;

   /*
    * Convert argc/argv into a GStrv.
    */
   strv = g_new (gchar *, argc + 1);
   for (i = 0; i < argc; i++) {
      strv [i] = g_strdup (argv [i]);
   }
   strv [argc] = NULL;

   contains_help = strv_contains (strv, "--help");

   /*
    * Check to see if a command was specified. If not we need to print out
    * an error. Otherwise, pass execution to the command.
    */
   command_name = fig_util_get_command_name (strv, &command_index);
   if (!command_name) {
      if (contains_help) {
         fig_cli_print_help (cli, strv [0], priv->stdout_stream);
         goto cleanup;
      } else {
         fig_cli_print_help (cli, strv [0], priv->stderr_stream);
         ret = EXIT_FAILURE;
         goto cleanup;
      }
   }

   /*
    * Lookup to see if we have the command.
    */
   info = fig_command_manager_lookup (FIG_COMMAND_MANAGER_DEFAULT,
                                      command_name);
   if (!info) {
      fig_cli_print_stderr (cli,
                            "fig: '%s' is not a fig command. "
                            "See 'fig --help'.\n",
                            command_name);
      ret = EXIT_FAILURE;
      goto cleanup;
   }

   /*
    * Create the command from our command info.
    */
   command = g_object_new (info->command_type,
                           //"project-dir", project_dir,
                           "stderr-stream", priv->stderr_stream,
                           "stdout-stream", priv->stdout_stream,
                           NULL);
   if (!command) {
      fig_cli_print_stderr (cli,
                            "fig: '%s' command failed to initialize. "
                            "Please check your installation.\n",
                            command_name);
      ret = EXIT_FAILURE;
      goto cleanup;
   }

   /*
    * Create our arguments for the command. We simply bump up the argv
    * so that it appears as the command-name is argv[0].
    */
   g_assert (command_index > 0);
   cmd_argv = &strv [command_index];


   /*
    * Execute the command.
    */
   ret = fig_command_run (command, g_strv_length (cmd_argv), cmd_argv);

cleanup:
   g_clear_object (&command);
   fig_command_info_free (info);
   g_strfreev (strv);

   return ret;
}

static void
fig_cli_finalize (GObject *object)
{
   FigCliPrivate *priv;

   priv = FIG_CLI (object)->priv;

   g_clear_object (&priv->stderr_stream);
   g_clear_object (&priv->stdout_stream);

   G_OBJECT_CLASS (fig_cli_parent_class)->finalize (object);
}

static void
fig_cli_get_property (GObject    *object,
                      guint       prop_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
   FigCli *cli = FIG_CLI (object);

   switch (prop_id) {
   case PROP_STDERR_STREAM:
      g_value_set_object (value, fig_cli_get_stderr_stream (cli));
      break;
   case PROP_STDOUT_STREAM:
      g_value_set_object (value, fig_cli_get_stdout_stream (cli));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_cli_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
   FigCli *cli = FIG_CLI (object);

   switch (prop_id) {
   case PROP_STDERR_STREAM:
      fig_cli_set_stderr_stream (cli, g_value_get_object (value));
      break;
   case PROP_STDOUT_STREAM:
      fig_cli_set_stdout_stream (cli, g_value_get_object (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_cli_class_init (FigCliClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_cli_finalize;
   object_class->get_property = fig_cli_get_property;
   object_class->set_property = fig_cli_set_property;

   gParamSpecs [PROP_STDERR_STREAM] =
      g_param_spec_object ("stderr-stream",
                           _("Stderr Stream"),
                           _("The stream to write stderr content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_STDERR_STREAM,
                                    gParamSpecs [PROP_STDERR_STREAM]);

   gParamSpecs [PROP_STDOUT_STREAM] =
      g_param_spec_object ("stdout-stream",
                           _("Stdout Stream"),
                           _("The stream to write stdout content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_STDOUT_STREAM,
                                    gParamSpecs [PROP_STDOUT_STREAM]);
}

static void
fig_cli_init (FigCli *cli)
{
   cli->priv = fig_cli_get_instance_private (cli);
   cli->priv->stderr_stream = g_unix_output_stream_new (STDERR_FILENO, FALSE);
   cli->priv->stdout_stream = g_unix_output_stream_new (STDOUT_FILENO, FALSE);
}
