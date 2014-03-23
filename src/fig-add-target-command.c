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

static void
create_mk_file_program (FigAddTargetCommand *self,
                        GFile               *file)
{
}

static void
create_mk_file_library (FigAddTargetCommand *self,
                        GFile               *file)
{
}

static void
create_mk_file (FigAddTargetCommand *self)
{
   FigAddTargetCommandPrivate *priv;
   const gchar *directory = NULL;
   GFile *project_dir = NULL;
   GFile *dir = NULL;
   GFile *mkfile = NULL;
   gchar *mkname = NULL;

   g_assert (FIG_IS_ADD_TARGET_COMMAND (self));

   priv = self->priv;

   mkname = g_strdup_printf ("%s.mk", priv->name);
   g_strdelimit (mkname, " ", '_');

   directory = priv->directory ? priv->directory : ".";
   project_dir = fig_command_get_project_dir (FIG_COMMAND (self));
   dir = g_file_get_child (project_dir, directory);
   mkfile = g_file_get_child (dir, mkname);

   if (priv->target_type == FIG_TARGET_PROGRAM) {
      create_mk_file_program (self, mkfile);
   } else if (priv->target_type == FIG_TARGET_LIBRARY) {
      create_mk_file_library (self, mkfile);
   } else {
      g_assert_not_reached ();
   }

   g_free (mkname);
   g_clear_object (&mkfile);
   g_clear_object (&dir);
}

static void
add_mk_file_to_toplevel (FigAddTargetCommand *self)
{
}

static gint
fig_add_target_command_run (FigCommand  *command,
                            gint         argc,
                            gchar      **argv)
{
   FigAddTargetCommandPrivate *priv;
   FigAddTargetCommand *add_command = (FigAddTargetCommand *)command;
   GOptionContext *context;
   GError *error = NULL;
   gchar **argv_copy;
   gchar *name = NULL;
   gchar *directory = NULL;
   gboolean library = FALSE;
   gboolean program = FALSE;
   GOptionEntry entries [] = {
      { "name", 'n', 0, G_OPTION_ARG_STRING, &name,
        _("The name of the target."), _("NAME") },
      { "library", 0, 0, G_OPTION_ARG_NONE, &library,
        _("If the target should be a library.") },
      { "program", 0, 0, G_OPTION_ARG_NONE, &program,
        _("If the target should be a program.") },
      { "directory", 0, 0, G_OPTION_ARG_NONE, &directory,
        _("The directory for the target. Defaults to current."), "DIR" },
      { NULL }
   };

   g_assert (FIG_IS_ADD_TARGET_COMMAND (command));

   priv = FIG_ADD_TARGET_COMMAND (command)->priv;

   g_set_prgname ("fig add-target");

   argv_copy = g_strdupv (argv);
   context = g_option_context_new (NULL);
   g_option_context_add_main_entries (context, entries, NULL);

   if (!g_option_context_parse (context, &argc, &argv_copy, &error)) {
      fig_command_printerr (command, "%s\n\n", error->message);
      g_error_free (error);
      g_option_context_free (context);
      return EXIT_FAILURE;
   }

   g_option_context_free (context);

   if (name) {
      fig_add_target_command_set_name (add_command, name);
      g_free (name);
   }

   if (directory) {
      fig_add_target_command_set_directory (add_command, name);
      g_free (directory);
   }

   if (program && library) {
      fig_command_printerr (command,
                            "Please specify either --program or --library, "
                            "not both.\n\n");
      return EXIT_FAILURE;
   } else if (library) {
      priv->target_type = FIG_TARGET_LIBRARY;
   } else {
      priv->target_type = FIG_TARGET_PROGRAM;
   }

   if (!priv->name) {
      fig_command_printerr (command,
                            "Please specify target name with --name=\n\n");
      return EXIT_FAILURE;
   }

   create_mk_file (add_command);
   add_mk_file_to_toplevel (add_command);

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
