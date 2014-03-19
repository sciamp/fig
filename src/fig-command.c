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
#include <stdlib.h>

#include "fig-command.h"

G_DEFINE_TYPE (FigCommand, fig_command, G_TYPE_OBJECT)

struct _FigCommandPrivate
{
   GFile *project_dir;
};

enum
{
   PROP_0,
   PROP_PROJECT_DIR,
   LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

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
   g_type_class_add_private (object_class, sizeof (FigCommandPrivate));

   gParamSpecs [PROP_PROJECT_DIR] =
      g_param_spec_object ("project-dir",
                           _("Project Dir"),
                           _("The directory containing the project."),
                           G_TYPE_FILE,
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_PROJECT_DIR,
                                    gParamSpecs [PROP_PROJECT_DIR]);
}

static void
fig_command_init (FigCommand *command)
{
   command->priv = G_TYPE_INSTANCE_GET_PRIVATE (command,
                                                FIG_TYPE_COMMAND,
                                                FigCommandPrivate);
   command->priv->project_dir = g_file_new_for_path (".");
}
