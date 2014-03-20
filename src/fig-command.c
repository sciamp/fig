/* fig-command.c
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

#include <gio/gunixoutputstream.h>
#include <glib/gi18n.h>
#include <stdlib.h>

#include "fig-command.h"

struct _FigCommandPrivate
{
   GFile         *project_dir;
   GOutputStream *stderr_stream;
   GOutputStream *stdout_stream;
};

enum
{
   PROP_0,
   PROP_PROJECT_DIR,
   PROP_STDERR_STREAM,
   PROP_STDOUT_STREAM,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigCommand, fig_command, G_TYPE_OBJECT)

static GParamSpec *gParamSpecs [LAST_PROP];

GOutputStream *
fig_command_get_stderr_stream (FigCommand *command)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), NULL);

   return command->priv->stderr_stream;
}

void
fig_command_set_stderr_stream (FigCommand    *command,
                               GOutputStream *stderr_stream)
{
   FigCommandPrivate *priv;

   g_return_if_fail (FIG_IS_COMMAND (command));
   g_return_if_fail (!stderr_stream || G_IS_OUTPUT_STREAM (stderr_stream));

   priv = command->priv;

   if (stderr_stream != priv->stderr_stream) {
      g_clear_object (&priv->stderr_stream);
      if (stderr_stream) {
         priv->stderr_stream = g_object_ref (stderr_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (command),
                                gParamSpecs [PROP_STDERR_STREAM]);
   }
}

GOutputStream *
fig_command_get_stdout_stream (FigCommand *command)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), NULL);

   return command->priv->stderr_stream;
}

void
fig_command_set_stdout_stream (FigCommand    *command,
                               GOutputStream *stdout_stream)
{
   FigCommandPrivate *priv;

   g_return_if_fail (FIG_IS_COMMAND (command));
   g_return_if_fail (!stdout_stream || G_IS_OUTPUT_STREAM (stdout_stream));

   priv = command->priv;

   if (stdout_stream != priv->stdout_stream) {
      g_clear_object (&priv->stdout_stream);
      if (stdout_stream) {
         priv->stdout_stream = g_object_ref (stdout_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (command),
                                gParamSpecs [PROP_STDOUT_STREAM]);
   }
}

void
fig_command_log (FigCommand  *command,
                 const gchar *program,
                 const gchar *format,
                 ...)
{
   va_list args;
   gchar *message;
   gchar *full;

   va_start (args, format);
   message = g_strdup_vprintf (format, args);
   va_end (args);

   full = g_strdup_printf ("  %-10s %s\n", program, format);
   g_free (message);

   g_output_stream_write_all (command->priv->stdout_stream,
                              full, strlen (full), NULL, NULL, NULL);
   g_free (full);
}

gint
fig_command_run (FigCommand  *command,
                 gint         argc,
                 gchar      **argv)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), -1);

   if (FIG_COMMAND_GET_CLASS (command)->run) {
      return FIG_COMMAND_GET_CLASS (command)->run (command, argc, argv);
   }

   return EXIT_SUCCESS;
}

GFile *
fig_command_get_project_dir (FigCommand *command)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), NULL);

   return command->priv->project_dir;
}

void
fig_command_set_project_dir (FigCommand *command,
                             GFile      *project_dir)
{
   FigCommandPrivate *priv;
   GFile *tmp;

   g_return_if_fail (FIG_IS_COMMAND (command));
   g_return_if_fail (G_IS_OBJECT (command));
   g_return_if_fail (!project_dir || G_IS_FILE (project_dir));

   priv = command->priv;

   tmp = priv->project_dir;
   priv->project_dir = project_dir ? g_object_ref (project_dir) : NULL;
   if (!priv->project_dir) {
      priv->project_dir = g_file_new_for_path (".");
   }
   g_object_notify_by_pspec (G_OBJECT (command),
                             gParamSpecs [PROP_PROJECT_DIR]);
   g_clear_object (&tmp);
}

static void
fig_command_finalize (GObject *object)
{
   FigCommandPrivate *priv = FIG_COMMAND (object)->priv;

   g_clear_object (&priv->project_dir);
   g_clear_object (&priv->stderr_stream);
   g_clear_object (&priv->stdout_stream);

   G_OBJECT_CLASS (fig_command_parent_class)->finalize (object);
}

static void
fig_command_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
   FigCommand *command = FIG_COMMAND(object);

   switch (prop_id) {
   case PROP_PROJECT_DIR:
      g_value_set_object (value, fig_command_get_project_dir (command));
      break;
   case PROP_STDERR_STREAM:
      g_value_set_object (value, fig_command_get_stderr_stream (command));
      break;
   case PROP_STDOUT_STREAM:
      g_value_set_object (value, fig_command_get_stdout_stream (command));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_command_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
   FigCommand *command = FIG_COMMAND(object);

   switch (prop_id) {
   case PROP_PROJECT_DIR:
      fig_command_set_project_dir (command, g_value_get_object (value));
      break;
   case PROP_STDERR_STREAM:
      fig_command_set_stderr_stream (command, g_value_get_object (value));
      break;
   case PROP_STDOUT_STREAM:
      fig_command_set_stdout_stream (command, g_value_get_object (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_command_class_init (FigCommandClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_command_finalize;
   object_class->get_property = fig_command_get_property;
   object_class->set_property = fig_command_set_property;

   gParamSpecs [PROP_PROJECT_DIR] =
      g_param_spec_object ("project-dir",
                           _("Project Dir"),
                           _("The directory containing the project."),
                           G_TYPE_FILE,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS |
                            G_PARAM_CONSTRUCT_ONLY));
   g_object_class_install_property (object_class, PROP_PROJECT_DIR,
                                    gParamSpecs [PROP_PROJECT_DIR]);

   gParamSpecs [PROP_STDERR_STREAM] =
      g_param_spec_object ("stderr-stream",
                           _("Stderr Stream"),
                           _("The stream to write stderr content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS |
                            G_PARAM_CONSTRUCT_ONLY));
   g_object_class_install_property (object_class, PROP_STDERR_STREAM,
                                    gParamSpecs [PROP_STDERR_STREAM]);

   gParamSpecs [PROP_STDOUT_STREAM] =
      g_param_spec_object ("stdout-stream",
                           _("Stdout Stream"),
                           _("The stream to write stdout content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS |
                            G_PARAM_CONSTRUCT_ONLY));
   g_object_class_install_property (object_class, PROP_STDOUT_STREAM,
                                    gParamSpecs [PROP_STDOUT_STREAM]);
}

static void
fig_command_init (FigCommand *command)
{
   FigCommandPrivate *priv;

   priv = command->priv = fig_command_get_instance_private (command);

   priv->project_dir = g_file_new_for_path (".");
   priv->stderr_stream = g_unix_output_stream_new (STDERR_FILENO, FALSE);
   priv->stdout_stream = g_unix_output_stream_new (STDOUT_FILENO, FALSE);
}
