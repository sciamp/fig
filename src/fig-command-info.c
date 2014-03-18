/* fig-command-info.c
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

#include "fig-command-info.h"

GType
fig_command_info_get_type (void)
{
   static gsize initialized;
   static GType gtype;

   if (g_once_init_enter (&initialized)) {
      gtype = g_boxed_type_register_static ("FigCommandInfo",
                                            (GBoxedCopyFunc)fig_command_info_copy,
                                            (GBoxedFreeFunc)fig_command_info_free);
      g_once_init_leave (&initialized, 1);
   }

   return gtype;
}

FigCommandInfo *
fig_command_info_copy (const FigCommandInfo *command_info)
{
   FigCommandInfo *copy;

   g_return_val_if_fail (command_info, NULL);

   copy = g_new0 (FigCommandInfo, 1);
   copy->name = g_strdup (command_info->name);
   copy->description = g_strdup (command_info->description);
   copy->command_type = command_info->command_type;

   return copy;
}

void
fig_command_info_free (FigCommandInfo *command_info)
{
   if (command_info) {
      g_free (command_info->name);
      g_free (command_info->description);
      g_free (command_info);
   }
}
