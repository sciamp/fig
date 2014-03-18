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
#include "fig-util.h"

static void
print_help (void)
{
   FigCommandManager *commands;
   FigCommandInfo *info;
   GList *list, *iter;

   commands = fig_command_manager_get_default ();
   list = fig_command_manager_get_commands (commands);

   g_printerr ("Usage:\n"
               "  fig [--project-dir=DIR] <command> <args> - "
               "manage autotools projects\n"
               "\n"
               "Options:\n"
               "  --project-dir=DIR    Specify the project directory.\n"
               "                       Defaults to current directory.\n"
               "\n"
               "Commands:\n");

   for (iter = list; iter; iter = iter->next) {
      info = iter->data;

      g_printerr ("  %-20s %s\n", info->name, info->description);
   }

   g_printerr ("\n"
               "Examples:\n"
               "  # initialize a new project in ~/foo\n"
               "  fig --project-dir=~/foo init\n"
               "\n");

   g_list_foreach (list, (GFunc)fig_command_info_free, NULL);
   g_list_free (list);
}

int
main (int   argc,
      char *argv[])
{
   FigCommandManager *commands;
   FigCommandInfo *command_info;
   const gchar *command_name;
   FigCommand *command;

   if (!(command_name = fig_util_get_command_name (argc, argv))) {
      print_help ();
      return EXIT_FAILURE;
   }

   commands = fig_command_manager_get_default ();

   if (!(command_info = fig_command_manager_lookup (commands, command_name))) {
      g_printerr ("fig: '%s' is not a fig command. See 'fig --help'.\n",
                  command_name);
      return EXIT_FAILURE;
   }

   if (!(command = g_object_new (command_info->command_type, NULL))) {
      g_printerr ("fig: '%s' command was not installed properly.\n",
                  command_name);
      return EXIT_FAILURE;
   }

   /*
    * TODO: remove command name from argv.
    *       get option group from command.
    *       parse whole thing with one option context.
    */

   g_clear_object (&command);
   fig_command_info_free (command_info);
   g_clear_object (&commands);

   return EXIT_SUCCESS;
}

#if 0
int
main (int   argc,
      char *argv[])
{
   GOptionContext *context;
   FigCommandInfo *info;
   const gchar *command_name;
   FigCommand *command;
   GError *error = NULL;
   GFile *dir;
   gchar *str;
   int i;
   int ret = EXIT_FAILURE;

   context = g_option_context_new ("- Scrappy autotools manager");
   g_option_context_add_main_entries (context, gEntries, NULL);
   g_option_context_set_help_enabled (context, FALSE);
   g_option_context_set_ignore_unknown_options (context, TRUE);

   if (!g_option_context_parse (context, &argc, &argv, &error)) {
      g_printerr ("%s\n", error->message);
      g_clear_error (&error);
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
      for (i = 0; i < argc; i++) {
         if (0 == g_strcmp0 (argv [i], "--help")) {
            str = g_option_context_get_help (context, TRUE, NULL);
            g_printerr ("%s", str);
            g_free (str);
            ret = EXIT_SUCCESS;
            goto failure;
         }
      }

      g_printerr ("fig: '%s' is not a command. See 'fig --help'.\n\n",
                  command_name);
   } else {
      if (!(command = fig_command_info_create (info))) {
         g_printerr ("fig: There is a problem with your installation of the "
                     "'%s' command.\n", command_name);
      } else {
         dir = g_file_new_for_path (gDirectory);
         g_assert (G_IS_FILE (dir));
         fig_command_set_project_dir (command, dir);
         g_clear_object (&dir);

         if (!fig_command_parse (command, argc, argv, &error)) {
            g_printerr ("%s\n", error->message);
            g_clear_error (&error);
            g_clear_object (&command);
            goto failure;
         }
         ret = fig_command_run (command, &error);
         if (ret != 0 && error) {
            g_printerr ("%s\n", error->message);
            g_clear_error (&error);
         }
         g_clear_object (&command);
      }
   }

failure:
   unload_plugins ();
   g_option_context_free (context);
   g_free (gDirectory);

   return ret;
}
#endif
