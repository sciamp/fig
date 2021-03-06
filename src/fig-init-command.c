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
#include <stdio.h>
#include <stdlib.h>

#include "fig-init-command.h"

struct _FigInitCommandPrivate
{
   gchar    *name;
   gchar    *version;
   gchar    *license;
   gboolean  gtk_doc;
   gint      version_major;
   gint      version_minor;
   gint      version_micro;
};

enum
{
   PROP_0,
   PROP_GTK_DOC,
   PROP_LICENSE,
   PROP_NAME,
   PROP_VERSION,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigInitCommand, fig_init_command, FIG_TYPE_COMMAND)

static GParamSpec *gParamSpecs [LAST_PROP];

gboolean
fig_init_command_get_gtk_doc (FigInitCommand *command)
{
   g_return_val_if_fail (FIG_IS_INIT_COMMAND (command), FALSE);

   return command->priv->gtk_doc;
}

void
fig_init_command_set_gtk_doc (FigInitCommand *command,
                              gboolean        gtk_doc)
{
   g_return_if_fail (FIG_IS_INIT_COMMAND (command));

   command->priv->gtk_doc = gtk_doc;
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_GTK_DOC]);
}

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
   FigInitCommandPrivate *priv;

   g_return_if_fail (FIG_IS_INIT_COMMAND (command));

   priv = command->priv;

   g_free (priv->version);
   priv->version = g_strdup (version ? version : "0.1.0");
   sscanf (priv->version, "%u.%u.%u",
           &priv->version_major,
           &priv->version_minor,
           &priv->version_micro);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_VERSION]);
}

const gchar *
fig_init_command_get_license (FigInitCommand *command)
{
   g_return_val_if_fail (FIG_IS_INIT_COMMAND (command), NULL);

   return command->priv->license;
}

void
fig_init_command_set_license (FigInitCommand *command,
                              const gchar    *license)
{
   g_return_if_fail (FIG_IS_INIT_COMMAND (command));

   if (license && strstr (license, "/")) {
      g_printerr ("license cannot be a path.\n");
      return;
   }

   g_free (command->priv->license);
   command->priv->license = g_strdup (license);
   g_object_notify_by_pspec (G_OBJECT (command), gParamSpecs [PROP_LICENSE]);
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

   fig_command_log (command, "GEN", target_name ? target_name : name);

   if (!fig_template_render (tmpl, dst, &error)) {
      g_printerr ("%s: %s\n", name, error->message);
      g_clear_error (&error);
   }

   g_object_unref (tmpl);
   g_object_unref (dst);
}

static void
render_project_info (FigInitCommand *command)
{
   const char *name = command->priv->name;
   GString *str;
   GFile *parent;
   GFile *file;
   gchar *project_dir_name = NULL;

   parent = fig_command_get_project_dir (FIG_COMMAND (command));

   if (!name) {
      project_dir_name = g_file_get_basename (parent);
      if (0 == g_strcmp0 (project_dir_name, ".")) {
         name = "foo";
      } else {
         name = project_dir_name;
      }
   }

   str = g_string_new (NULL);
   g_string_append_printf (str, "m4_define([project_name],[%s])\n", name);
   g_string_append_printf (str, "m4_define([project_major_version],[%d])\n",
                           command->priv->version_major);
   g_string_append_printf (str, "m4_define([project_minor_version],[%d])\n",
                           command->priv->version_minor);
   g_string_append_printf (str, "m4_define([project_micro_version],[%d])\n",
                           command->priv->version_micro);
   g_string_append (str, "m4_define([project_interface_age],[0])\n");
   g_string_append (str, "m4_define([project_bugreport_url],[http://])\n");

   file = g_file_get_child (parent, "build/autotools/project_info.m4");

   fig_command_log (FIG_COMMAND (command), "GEN", "build/autotools/project_info.m4");
   g_file_replace_contents (file, str->str, str->len, NULL, FALSE,
                            G_FILE_CREATE_NONE, NULL, NULL, NULL);

   g_object_unref (file);
   g_string_free (str, TRUE);
   g_free (project_dir_name);
}

static gboolean
fig_init_command_parse (FigInitCommand  *command,
                        gint             argc,
                        gchar          **argv)
{
   gchar *name = NULL;
   gchar *version = NULL;
   gchar *license = NULL;
   gboolean gtkdoc = FALSE;
   GOptionContext *context;
   GOptionEntry entries[] = {
      { "license", 'l', 0, G_OPTION_ARG_STRING, &license,
        _("The license for the project."), _("gpl-3.0") },
      { "name", 'n', 0, G_OPTION_ARG_STRING, &name,
        _("The name of the project."), _("foo") },
      { "version", 'V', 0, G_OPTION_ARG_STRING, &version,
        _("The starting version of the project."), _("0.1.0") },
      { "gtk-doc", 0, 0, G_OPTION_ARG_NONE, &gtkdoc,
        _("Add support for gtk-doc documentation.") },
      { NULL }
   };
   gboolean ret = FALSE;
   GError *error = NULL;
   gchar **argv_copy;

   g_set_prgname ("fig init");

   argv_copy = g_strdupv (argv);
   context = g_option_context_new (NULL);
   g_option_context_add_main_entries (context, entries, NULL);
   g_option_context_set_summary (
         context,
         _("Initialize a new autotools project."));

   if (!g_option_context_parse (context, &argc, &argv_copy, &error)) {
      goto cleanup;
   }

   fig_init_command_set_license (command, license);
   fig_init_command_set_name (command, name);
   fig_init_command_set_version (command, version);
   fig_init_command_set_gtk_doc (command, gtkdoc);

   ret = TRUE;

cleanup:
   g_option_context_free (context);
   g_strfreev (argv_copy);
   g_clear_error (&error);
   g_free (name);
   g_free (version);

   return ret;
}

static gint
fig_init_command_run (FigCommand  *command,
                      gint         argc,
                      gchar      **argv)
{
   FigInitCommandPrivate *priv;
   const gchar *license = "gpl-3.0";
   gchar *license_path;

   g_return_val_if_fail (FIG_IS_COMMAND (command), -1);

   priv = FIG_INIT_COMMAND (command)->priv;

   if (!fig_init_command_parse (FIG_INIT_COMMAND (command), argc, argv)) {
      return EXIT_FAILURE;
   }

   if (priv->license) {
      license = priv->license;
   }

   license_path = g_strdup_printf ("licenses/%s.txt", license);

   render_template (command, "AUTHORS", NULL);
   render_template (command, license_path, "COPYING");
   render_template (command, "Makefile.am", NULL);
   render_template (command, "NEWS", NULL);
   render_template (command, "README", NULL);
   render_template (command, "autogen.sh", NULL);
   render_template (command, "configure.ac", NULL);
   render_template (command, ".gitignore", NULL);

   render_template (command, "build/autotools/.gitignore", NULL);

   render_template (command, "build/autotools/autogen.d/git-submodule.sh", NULL);

   if (priv->gtk_doc) {
      render_template (command, "build/autotools/autogen.d/gtk-doc.sh", NULL);
   }

   render_template (command, "build/autotools/automake/AutomakeDocs.mk", NULL);
   render_template (command, "build/autotools/automake/ChangeLog.mk", NULL);
   render_template (command, "build/autotools/automake/Defaults.mk", NULL);

   render_template (command, "build/autotools/autoconf.d/setup_libtool.m4", NULL);
   render_template (command, "build/autotools/autoconf.d/print_config.m4", NULL);
   render_template (command, "build/autotools/autoconf.d/20_debug.post-config", NULL);
   render_template (command, "build/autotools/autoconf.d/30_command_line_options.pre-am", NULL);
   render_template (command, "build/autotools/autoconf.d/50_ltversion.post-lt", NULL);
   render_template (command, "build/autotools/autoconf.d/50_progs.post-am", NULL);
   render_template (command, "build/autotools/autoconf.d/50_silent_rules.post-am", NULL);
   render_template (command, "build/autotools/autoconf.d/50_versions.pre-ac", NULL);
   render_template (command, "build/autotools/autoconf.d/.gitignore", NULL);

   render_template (command, "build/autotools/m4/.gitignore", NULL);

   render_project_info (FIG_INIT_COMMAND (command));

   g_free (license_path);

   fig_command_log (command, NULL,
                    "\n"
                    "Run ./autogen.sh to bootstrap your project.\n"
                    "\n");

   return EXIT_SUCCESS;
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
   case PROP_GTK_DOC:
      g_value_set_boolean (value, fig_init_command_get_gtk_doc (command));
      break;
   case PROP_LICENSE:
      g_value_set_string (value, fig_init_command_get_license (command));
      break;
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
   case PROP_GTK_DOC:
      fig_init_command_set_gtk_doc (command, g_value_get_boolean (value));
      break;
   case PROP_LICENSE:
      fig_init_command_set_license (command, g_value_get_string (value));
      break;
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

   command_class = FIG_COMMAND_CLASS (klass);
   command_class->run = fig_init_command_run;

   gParamSpecs [PROP_GTK_DOC] =
      g_param_spec_boolean ("gtk-doc",
                           _("Gtk Doc"),
                           _("If gtk-doc should be enabled."),
                           FALSE,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_GTK_DOC,
                                    gParamSpecs [PROP_GTK_DOC]);

   gParamSpecs [PROP_LICENSE] =
      g_param_spec_string ("license",
                           _("License"),
                           _("The license for the project."),
                           "gpl-3.0",
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_LICENSE,
                                    gParamSpecs [PROP_LICENSE]);

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
   command->priv = fig_init_command_get_instance_private (command);

   command->priv->name = g_strdup ("foo");

   command->priv->license = g_strdup ("gpl-3.0");

   command->priv->version = g_strdup ("0.1.0");
   command->priv->version_minor = 1;
}
