/* fig-update-authors-command.c
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

#include "fig-update-authors-command.h"

G_DEFINE_TYPE (FigUpdateAuthorsCommand,
               fig_update_authors_command,
               FIG_TYPE_COMMAND)

static gint
fig_update_authors_command_run (FigCommand  *command,
                                gint         argc,
                                gchar      **argv)
{
   GHashTableIter iter;
   const gchar *key;
   GHashTable *hash = NULL;
   GPtrArray *ptr;
   GString *contents;
   GError *error = NULL;
   GFile *project_dir;
   GFile *file = NULL;
   gchar *path = NULL;
   gchar **args = NULL;
   gchar *stdout_str = NULL;
   gchar **lines = NULL;
   gint status;
   gint i;

   project_dir = fig_command_get_project_dir (command);
   path = g_file_get_path (project_dir);
   hash = g_hash_table_new (g_str_hash, g_str_equal);
   contents = g_string_new (NULL);

   ptr = g_ptr_array_new ();
   g_ptr_array_add (ptr, "git");
   g_ptr_array_add (ptr, "log");
   g_ptr_array_add (ptr, "--format=%aN");
   g_ptr_array_add (ptr, NULL);
   args = (gchar **)g_ptr_array_free (ptr, FALSE);

   if (!g_spawn_sync (path, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
                      &stdout_str, NULL, &status, &error)) {
      g_printerr ("%s\n", error->message);
      goto cleanup;
   }

   g_strchomp (stdout_str);

   lines = g_strsplit (stdout_str, "\n", 0);

   for (i = 0; lines [i]; i++) {
      g_hash_table_insert (hash, lines [i], lines [i]);
   }

   g_hash_table_iter_init (&iter, hash);

   while (g_hash_table_iter_next (&iter, (gpointer *)&key, NULL)) {
      g_string_append (contents, key);
      g_string_append_c (contents, '\n');
   }

   file = g_file_get_child (project_dir, "AUTHORS");

   g_file_replace_contents (file,
                            contents->str,
                            contents->len,
                            NULL,
                            FALSE,
                            G_FILE_CREATE_NONE,
                            NULL,
                            NULL,
                            NULL);

cleanup:
   g_clear_object (&file);
   g_hash_table_unref (hash);
   g_string_free (contents, TRUE);
   g_strfreev (lines);
   g_clear_error (&error);
   g_free (stdout_str);
   g_free (args);
   g_free (path);

   return 0;
}

static void
fig_update_authors_command_class_init (FigUpdateAuthorsCommandClass *klass)
{
   FigCommandClass *command_class;

   command_class = FIG_COMMAND_CLASS (klass);
   command_class->run = fig_update_authors_command_run;
}

static void
fig_update_authors_command_init (FigUpdateAuthorsCommand *command)
{
}
