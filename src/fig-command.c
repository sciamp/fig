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

#include <glib/gi18n.h>

#include "fig-command.h"

G_DEFINE_TYPE (FigCommand, fig_command, G_TYPE_OBJECT)

struct _FigCommandPrivate
{
   GFile *project_dir;
};

enum
{
   RUN,
   LAST_SIGNAL
};

enum
{
   PROP_0,
   PROP_PROJECT_DIR,
   LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];
static guint       gSignals [LAST_SIGNAL];

gint
fig_command_run (FigCommand   *command,
                 const gchar  *project_dir,
                 gint          argc,
                 gchar       **argv)
{
   gint ret = -1;

   g_return_val_if_fail (FIG_IS_COMMAND(command), -1);

   g_signal_emit (command, gSignals [RUN], 0, project_dir, argc, argv, &ret);

   return ret;
}

GFile *
fig_command_get_project_dir (FigCommand *command)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), NULL);

   return command->priv->project_dir;
}

void
fig_command_set_project_dir (FigCommand *command,
                             GFile      *file)
{
   FigCommandPrivate *priv;

   g_return_if_fail (FIG_IS_COMMAND (command));

   priv = command->priv;

   if (file != priv->project_dir) {
      g_clear_object (&priv->project_dir);
      if (file) {
         priv->project_dir = g_object_ref (file);
      } else {
         priv->project_dir = g_file_new_for_path (".");
      }
      g_object_notify_by_pspec (G_OBJECT (command),
                                gParamSpecs [PROP_PROJECT_DIR]);
   }
}

static void
fig_command_finalize (GObject *object)
{
   FigCommandPrivate *priv = FIG_COMMAND (object)->priv;

   g_clear_object (&priv->project_dir);

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
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property(object_class, PROP_PROJECT_DIR,
                                   gParamSpecs[PROP_PROJECT_DIR]);

   gSignals [RUN] = g_signal_new ("run",
                                  FIG_TYPE_COMMAND,
                                  G_SIGNAL_RUN_LAST,
                                  G_STRUCT_OFFSET (FigCommandClass, run),
                                  g_signal_accumulator_first_wins,
                                  NULL,
                                  g_cclosure_marshal_generic,
                                  G_TYPE_INT,
                                  3,
                                  G_TYPE_STRING,
                                  G_TYPE_INT,
                                  G_TYPE_POINTER);
}

static void
fig_command_init (FigCommand *command)
{
   command->priv = G_TYPE_INSTANCE_GET_PRIVATE (command,
                                                FIG_TYPE_COMMAND,
                                                FigCommandPrivate);

   command->priv->project_dir = g_file_new_for_path (".");
}
