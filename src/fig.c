/* fig.c
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

#include <glib.h>
#include <glib/gi18n.h>
#include <stdlib.h>

#include "fig.h"

static gchar *gDirectory;
static GList *gPlugins;
static GOptionEntry gEntries [] = {
   { "directory", 'd', 0, G_OPTION_ARG_FILENAME, &gDirectory,
     "Root directory of the project.", "DIR" },
   { NULL }
};

static void
load_plugins (void)
{
   const gchar *plugins_dir = FIG_PLUGINS_DIR;
   const gchar *str;
   FigPlugin *plugin;
   GError *error = NULL;
   gchar **dirs;
   gchar *path;
   GDir *dir;
   gint i;

   g_debug ("Loading plugins.");

   if ((str = g_getenv ("FIG_PLUGINS_DIR"))) {
      if (g_file_test (str, G_FILE_TEST_IS_DIR)) {
         plugins_dir = str;
      }
   }

   dirs = g_strsplit (plugins_dir, ":", 0);

   for (i = 0; dirs [i]; i++) {
      if (!g_file_test (dirs [i], G_FILE_TEST_IS_DIR)) {
         continue;
      }

      if (!(dir = g_dir_open (dirs [i], 0, &error))) {
         g_printerr ("%s\n", error->message);
         g_clear_error (&error);
         continue;
      }

      while ((str = g_dir_read_name (dir))) {
         if (!g_str_has_suffix (str, "."G_MODULE_SUFFIX)) {
            continue;
         }

         path = g_build_filename (dirs [i], str, NULL);
         plugin = fig_plugin_new_for_path (path);
         g_free (path);

         g_debug ("Loading %s", path);

         if (!fig_plugin_load (plugin, &error)) {
            g_printerr ("%s\n", error->message);
            g_clear_error (&error);
            g_object_unref (plugin);
            continue;
         }

         gPlugins = g_list_prepend (gPlugins, plugin);
      }

      g_dir_close (dir);
   }

   g_strfreev (dirs);
}

static void
unload_plugins (void)
{
   GError *error = NULL;
   GList *iter;

   g_debug ("Unloading plugins.");

   for (iter = gPlugins; iter; iter = iter->next) {
      if (!fig_plugin_unload (iter->data, &error)) {
         g_printerr ("%s\n", error->message);
      }
   }

   iter = gPlugins;
   gPlugins = NULL;

   g_list_foreach (iter, (GFunc)g_object_unref, NULL);
   g_list_free (iter);
}

int
main (int   argc,
      char *argv[])
{
   GOptionContext *context;
   FigCommandInfo *info;
   const gchar *command_name;
   FigCommand *command;
   GError *error = NULL;
   gchar *str;
   int ret = EXIT_FAILURE;

   context = g_option_context_new ("- Scrappy autotools manager");
   g_option_context_add_main_entries (context, gEntries, NULL);
   if (!g_option_context_parse (context, &argc, &argv, &error)) {
      g_printerr ("%s\n", error->message);
      g_error_free (error);
      goto failure;
   }

   if (argc < 2) {
      str = g_option_context_get_help (context, TRUE, NULL);
      g_printerr ("%s", str);
      g_free (str);
      goto failure;
   }

   if (!gDirectory) {
      gDirectory = g_strdup (".");
   }

   load_plugins ();

   command_name = argv [1];

   if (!(info = fig_command_info_lookup (command_name))) {
      g_printerr ("fig: '%s' is not a command. See 'fig --help'.\n\n",
                  command_name);
   } else {
      if (!(command = fig_command_info_create (info))) {
         g_printerr ("fig: There is a problem with your installation of the "
                     "'%s' command.\n", command_name);
      } else {
         ret = fig_command_run (command, gDirectory, argc, argv);
         g_object_unref (command);
      }
   }

failure:
   unload_plugins ();
   g_option_context_free (context);
   g_free (gDirectory);

   return ret;
}
