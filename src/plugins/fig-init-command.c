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
   LAST_PROP
};

//static GParamSpec *gParamSpecs[LAST_PROP];

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
fig_init_command_run (FigCommand   *command,
                      GError      **error)
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
   //FigInitCommand *command = FIG_INIT_COMMAND (object);

   switch (prop_id) {
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_init_command_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
   //FigInitCommand *command = FIG_INIT_COMMAND (object);

   switch (prop_id) {
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
}

static void
fig_init_command_init (FigInitCommand *command)
{
   command->priv = G_TYPE_INSTANCE_GET_PRIVATE (command,
                                                FIG_TYPE_INIT_COMMAND,
                                                FigInitCommandPrivate);
}

FIG_PLUGIN_REGISTER ("init", "Initialize a new project.", FIG_TYPE_INIT_COMMAND)
