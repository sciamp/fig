/* fig-init.c
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

#include "fig-command.h"
#include "fig-command-info.h"
#include "fig-template.h"

static void
render_template (const gchar *name,
                 const gchar *directory)
{
   FigTemplate *tmpl;
   GError *error = NULL;
   GFile *dst;
   gchar *path;

   tmpl = fig_template_new (name);
   g_assert (tmpl);

   path = g_build_filename (directory, name, NULL);
   dst = g_file_new_for_path (path);
   g_free (path);

   if (!fig_template_render (tmpl, dst, &error)) {
      g_printerr ("%s: %s\n", name, error->message);
      g_clear_error (&error);
   }

   g_object_unref (tmpl);
   g_object_unref (dst);
}

static gint
run_init (FigCommand   *command,
          const gchar  *directory,
          gint          argc,
          gchar       **argv,
          gpointer      user_data)
{
   g_return_val_if_fail (FIG_IS_COMMAND (command), -1);

   render_template ("autogen.sh", directory);
   render_template ("configure.ac", directory);
   render_template ("README", directory);
   render_template ("NEWS", directory);

   /* git init */
   /* tmpl autogen.sh . */
   /* tmpl README */
   /* tmpl configure.ac */
   /* tmpl COPYING-GPL3 */
   /* tmpl Makefile.am */
   /* tmpl NEWS */
   /* tmpl build/autotools/m4 */
   /* tmpl build/autotools/PrintConfiguration.m4 */
   /* git commit */

   g_print ("Initializing...\n");

   return -1;
}

static FigCommand *
create_command (FigCommandInfo *command_info,
                gpointer        user_data)
{
   FigCommand *command;

   command = g_object_new (FIG_TYPE_COMMAND, NULL);
   g_signal_connect (command, "run", G_CALLBACK (run_init), NULL);
   return command;
}

void
fig_plugin_load (void)
{
   FigCommandInfo *info;

   info = g_object_new (FIG_TYPE_COMMAND_INFO,
                        "name", "init",
                        "description", N_("Initialize a new automake project."),
                        NULL);
   fig_command_info_set_factory (info, create_command, NULL);
   fig_command_info_register (info);
}
