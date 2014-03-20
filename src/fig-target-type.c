/* fig-target-type.c
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

#include "fig-target-type.h"

GType
fig_target_type_get_type (void)
{
   static const GEnumValue values [] = {
      { FIG_TARGET_LIBRARY, "FIG_TARGET_LIBRARY", "LIBRARY" },
      { FIG_TARGET_PROGRAM, "FIG_TARGET_PROGRAM", "PROGRAM" },
      { 0 }
   };
   static gsize initialized;
   static GType type_id;

   if (g_once_init_enter (&initialized)) {
      type_id = g_enum_register_static ("FigTargetType", values);
      g_once_init_leave (&initialized, TRUE);
   }

   return type_id;
}
