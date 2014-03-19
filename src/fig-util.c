/* fig-util.c
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

#include <string.h>

#include "fig-util.h"

const gchar *
fig_util_get_command_name (gchar **argv)
{
   gint i;

   g_return_val_if_fail (argv, NULL);

   /*
    * Discover the command name after skipping known internal parameters
    * to the `fig' command line program.
    */

   for (i = 1; argv [i]; i++) {
      if (g_str_equal (argv [i], "--help")) {
         return NULL;
      } else if (0 == strncmp (argv [i], "--", 2)) {
         if (0 == strcmp (argv [i], "--project-dir")) {
            if (!strchr (argv [i], '=')) {
               i++;
            }
         }
      } else {
         return argv [i];
      }
   }

   return NULL;
}
