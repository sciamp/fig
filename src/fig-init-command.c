/* fig-init-command.c
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

#include "fig-init-command.h"

G_DEFINE_TYPE (FigInitCommand, fig_init_command, FIG_TYPE_COMMAND)

struct _FigInitCommandPrivate
{
   gchar *name;
   gchar *version;
};

enum
{
   PROP_0,
   PROP_NAME,
   PROP_VERSION,
   LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

const gchar *
fig_init_command_get_name (FigInitCommand *command)
{
   FigInitCommandPrivate *priv;
   GFile *file;

   g_return_val_if_fail (FIG_IS_INIT_COMMAND (command), NULL);

   priv = command->priv;

   if (!priv->name) {
      file = fig_command_get_project_dir (FIG_COMMAND (command));
      if (file) {
         priv->name = g_file_get_basename (file);
      }
   }

   return priv->name;
}

void
fig_init_command_set_name (FigInitCommand *command,
                           const gchar    *name)
{
   g_return_if_fail (FIG_IS_INIT_COMMAND (command));

   g_free (command->priv->name);
   command->priv->name = g_strdup (name);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_NAME]);
}

const gchar *
fig_init_command_get_version (FigInitCommand *command)
{
   FigInitCommandPrivate *priv;

   g_return_val_if_fail (FIG_IS_INIT_COMMAND (command), NULL);

   priv = command->priv;

   if (!priv->version) {
      return "0.1.0";
   }

   return priv->version;
}

void
fig_init_command_set_version (FigInitCommand *command,
                           const gchar    *version)
{
   g_return_if_fail (FIG_IS_INIT_COMMAND (command));

   g_free (command->priv->version);
   command->priv->version = g_strdup (version);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_VERSION]);
}

static void
render_template (FigCommand  *command,
                 const gchar *name,
                 const gchar *target_name)
{
   FigTemplate *tmpl;
   GError *error = NULL;
   GFile *directory;
   GFile *dst;

   tmpl = fig_template_new (name);
   g_assert (tmpl);

   directory = fig_command_get_project_dir (command);
   g_assert (directory);
   g_assert (G_IS_FILE (directory));

   dst = g_file_get_child (directory, target_name ? target_name : name);

   if (!fig_template_render (tmpl, dst, &error)) {
      g_printerr ("%s: %s\n", name, error->message);
      g_clear_error (&error);
   }

   g_object_unref (tmpl);
   g_object_unref (dst);
}

static gint
fig_init_command_run (FigCommand  *command,
                      gint         argc,
                      gchar      **argv,
                      GError     **error)
{
   const gchar *license = "gpl-3.0";
   gchar *license_path;

   g_return_val_if_fail (FIG_IS_COMMAND (command), -1);

   license_path = g_strdup_printf ("licenses/%s.txt", license);

   render_template (command, "AUTHORS", NULL);
   render_template (command, license_path, "COPYING");
   render_template (command, "Makefile.am", NULL);
   render_template (command, "NEWS", NULL);
   render_template (command, "README", NULL);
   render_template (command, "autogen.sh", NULL);
   render_template (command, "build/autotools/AutomakeDocs.mk", NULL);
   render_template (command, "build/autotools/ChangeLog.mk", NULL);
   render_template (command, "build/autotools/Defaults.mk", NULL);
   render_template (command, "build/autotools/m4/.gitignore", NULL);
   render_template (command, "configure.ac", NULL);

   g_free (license_path);

   return 0;
}

static void
fig_init_command_finalize (GObject *object)
{
   G_OBJECT_CLASS (fig_init_command_parent_class)->finalize (object);
}

static void
fig_init_command_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
   FigInitCommand *command = FIG_INIT_COMMAND (object);

   switch (prop_id) {
   case PROP_NAME:
      g_value_set_string (value, fig_init_command_get_name (command));
      break;
   case PROP_VERSION:
      g_value_set_string (value, fig_init_command_get_version (command));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      return;
   }

   g_object_notify_by_pspec (object, pspec);
}

static void
fig_init_command_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
   FigInitCommand *command = FIG_INIT_COMMAND (object);

   switch (prop_id) {
   case PROP_NAME:
      fig_init_command_set_name (command, g_value_get_string (value));
      break;
   case PROP_VERSION:
      fig_init_command_set_version (command, g_value_get_string (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_init_command_class_init (FigInitCommandClass *klass)
{
   FigCommandClass *command_class;
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = fig_init_command_finalize;
   object_class->get_property = fig_init_command_get_property;
   object_class->set_property = fig_init_command_set_property;
   g_type_class_add_private (object_class, sizeof (FigInitCommandPrivate));

   command_class = FIG_COMMAND_CLASS (klass);
   command_class->run = fig_init_command_run;

   gParamSpecs [PROP_NAME] =
      g_param_spec_string ("name",
                           _("Name"),
                           _("The name of the project."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_NAME,
                                    gParamSpecs [PROP_NAME]);

   gParamSpecs [PROP_VERSION] =
      g_param_spec_string ("version",
                           _("Version"),
                           _("The starting version triplet (0.1.0)"),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_VERSION,
                                    gParamSpecs [PROP_VERSION]);
}

static void
fig_init_command_init (FigInitCommand *command)
{
   command->priv = G_TYPE_INSTANCE_GET_PRIVATE (command,
                                                FIG_TYPE_INIT_COMMAND,
                                                FigInitCommandPrivate);
}
