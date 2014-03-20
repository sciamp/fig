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

#include "fig-add-target-command.h"

struct _FigAddTargetCommandPrivate
{
   gchar *name;
};

enum
{
   PROP_0,
   PROP_NAME,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigAddTargetCommand,
                            fig_add_target_command,
                            FIG_TYPE_COMMAND)

static GParamSpec *gParamSpecs [LAST_PROP];

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

static void
fig_add_target_command_finalize (GObject *object)
{
   FigAddTargetCommandPrivate *priv;

   priv = FIG_ADD_TARGET_COMMAND (object)->priv;

   g_clear_object (&priv->name);

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
   case PROP_NAME:
      g_value_set_string (value, fig_add_target_command_get_name (command));
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
   case PROP_NAME:
      fig_add_target_command_set_name (command, g_value_get_string (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_add_target_command_class_init (FigAddTargetCommandClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_add_target_command_finalize;
   object_class->get_property = fig_add_target_command_get_property;
   object_class->set_property = fig_add_target_command_set_property;

   gParamSpecs [PROP_NAME] =
      g_param_spec_string ("name",
                           _("Name"),
                           _("The name of the target."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_NAME,
                                    gParamSpecs [PROP_NAME]);
}

static void
fig_add_target_command_init (FigAddTargetCommand *command)
{
   command->priv = fig_add_target_command_get_instance_private (command);
}
