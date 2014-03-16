/* fig-command.c
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

#include "fig-command.h"

G_DEFINE_TYPE (FigCommand, fig_command, G_TYPE_OBJECT)

enum
{
   RUN,
   LAST_SIGNAL
};

static guint gSignals [LAST_SIGNAL];

gint
fig_command_run (FigCommand   *command,
                 const gchar  *project_dir,
                 gint          argc,
                 gchar       **argv)
{
   gint ret = -1;

   g_return_val_if_fail (FIG_IS_COMMAND(command), -1);

   g_signal_emit (command, gSignals [RUN], 0, project_dir, argc, argv, &ret);

   return ret;
}

static void
fig_command_class_init (FigCommandClass *klass)
{
   gSignals [RUN] = g_signal_new ("run",
                                  FIG_TYPE_COMMAND,
                                  G_SIGNAL_RUN_LAST,
                                  G_STRUCT_OFFSET (FigCommandClass, run),
                                  g_signal_accumulator_first_wins,
                                  NULL,
                                  g_cclosure_marshal_generic,
                                  G_TYPE_INT,
                                  3,
                                  G_TYPE_STRING,
                                  G_TYPE_INT,
                                  G_TYPE_POINTER);
}

static void
fig_command_init (FigCommand *command)
{
   command->priv = G_TYPE_INSTANCE_GET_PRIVATE (command,
                                                FIG_TYPE_COMMAND,
                                                FigCommandPrivate);
}
