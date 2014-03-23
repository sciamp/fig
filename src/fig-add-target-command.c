/* fig-add-target-command.c
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

#include "fig-add-target-command.h"

struct _FigAddTargetCommandPrivate
{
   gchar         *name;
   gchar         *directory;
   FigTargetType  target_type;
};

enum
{
   PROP_0,
   PROP_DIRECTORY,
   PROP_NAME,
   PROP_TARGET_TYPE,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigAddTargetCommand,
                            fig_add_target_command,
                            FIG_TYPE_COMMAND)

static GParamSpec *gParamSpecs [LAST_PROP];

const gchar *
fig_add_target_command_get_directory (FigAddTargetCommand *command)
{
   g_return_val_if_fail (FIG_IS_ADD_TARGET_COMMAND (command), NULL);

   return command->priv->directory;
}

void
fig_add_target_command_set_directory (FigAddTargetCommand *command,
                                      const gchar         *directory)
{
   g_return_if_fail (FIG_IS_ADD_TARGET_COMMAND (command));

   g_free (command->priv->directory);
   command->priv->directory = g_strdup (directory);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_DIRECTORY]);
}

const gchar *
fig_add_target_command_get_name (FigAddTargetCommand *command)
{
   g_return_val_if_fail (FIG_IS_ADD_TARGET_COMMAND (command), NULL);

   return command->priv->name;
}

void
fig_add_target_command_set_name (FigAddTargetCommand *command,
                                 const gchar         *name)
{
   g_return_if_fail (FIG_IS_ADD_TARGET_COMMAND (command));

   g_free (command->priv->name);
   command->priv->name = g_strdup (name);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_NAME]);
}

FigTargetType
fig_add_target_command_get_target_type (FigAddTargetCommand *command)
{
   g_return_val_if_fail (FIG_IS_ADD_TARGET_COMMAND (command), 0);

   return command->priv->target_type;
}

void
fig_add_target_command_set_target_type (FigAddTargetCommand *command,
                                        FigTargetType        target_type)
{
   g_return_if_fail (FIG_IS_ADD_TARGET_COMMAND (command));
   command->priv->target_type = target_type;
   g_object_notify_by_pspec (G_OBJECT (command),
                             gParamSpecs [PROP_TARGET_TYPE]);
}

static gint
fig_add_target_command_run (FigCommand  *command,
                            gint         argc,
                            gchar      **argv)
{
   FigAddTargetCommandPrivate *priv;

   g_assert (FIG_IS_ADD_TARGET_COMMAND (command));

   priv = FIG_ADD_TARGET_COMMAND (command)->priv;

   if (!priv->name) {
      fig_command_printerr (command,
                            "Please specify target name with --name=\n\n");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

static void
fig_add_target_command_finalize (GObject *object)
{
   FigAddTargetCommandPrivate *priv;

   priv = FIG_ADD_TARGET_COMMAND (object)->priv;

   g_clear_pointer (&priv->name, g_free);
   g_clear_pointer (&priv->directory, g_free);

   G_OBJECT_CLASS (fig_add_target_command_parent_class)->finalize (object);
}

static void
fig_add_target_command_get_property (GObject    *object,
                                     guint       prop_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
   FigAddTargetCommand *command = FIG_ADD_TARGET_COMMAND (object);

   switch (prop_id) {
   case PROP_DIRECTORY:
      g_value_set_string (value,
                          fig_add_target_command_get_directory (command));
      break;
   case PROP_NAME:
      g_value_set_string (value, fig_add_target_command_get_name (command));
      break;
   case PROP_TARGET_TYPE:
      g_value_set_enum (value,
                        fig_add_target_command_get_target_type (command));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_add_target_command_set_property (GObject      *object,
                                     guint         prop_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
   FigAddTargetCommand *command = FIG_ADD_TARGET_COMMAND (object);

   switch (prop_id) {
   case PROP_DIRECTORY:
      fig_add_target_command_set_directory (command,
                                            g_value_get_string (value));
      break;
   case PROP_NAME:
      fig_add_target_command_set_name (command, g_value_get_string (value));
      break;
   case PROP_TARGET_TYPE:
      fig_add_target_command_set_target_type (command,
                                              g_value_get_enum (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_add_target_command_class_init (FigAddTargetCommandClass *klass)
{
   GObjectClass *object_class;
   FigCommandClass *command_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_add_target_command_finalize;
   object_class->get_property = fig_add_target_command_get_property;
   object_class->set_property = fig_add_target_command_set_property;

   command_class = FIG_COMMAND_CLASS (klass);
   command_class->run = fig_add_target_command_run;

   gParamSpecs [PROP_NAME] =
      g_param_spec_string ("name",
                           _("Name"),
                           _("The name of the target."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_NAME,
                                    gParamSpecs [PROP_NAME]);

   gParamSpecs [PROP_DIRECTORY] =
      g_param_spec_string ("directory",
                           _("Directory"),
                           _("The directory for the target."),
                           "src",
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_DIRECTORY,
                                    gParamSpecs [PROP_DIRECTORY]);

   gParamSpecs [PROP_TARGET_TYPE] =
      g_param_spec_enum ("target-type",
                         _("Target Type"),
                         _("The type of the target to add."),
                         FIG_TYPE_TARGET_TYPE,
                         FIG_TARGET_PROGRAM,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_TARGET_TYPE,
                                    gParamSpecs [PROP_TARGET_TYPE]);
}

static void
fig_add_target_command_init (FigAddTargetCommand *command)
{
   command->priv = fig_add_target_command_get_instance_private (command);
   command->priv->target_type = FIG_TARGET_PROGRAM;
   command->priv->directory = g_strdup ("src");
}
