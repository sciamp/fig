/* fig-command-info.h
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

#ifndef FIG_COMMAND_INFO_H
#define FIG_COMMAND_INFO_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct
{
   gchar *name;
   gchar *description;
   GType  command_type;
} FigCommandInfo;

GType           fig_command_info_get_type (void) G_GNUC_CONST;
FigCommandInfo *fig_command_info_copy     (const FigCommandInfo *command_info);
void            fig_command_info_free     (FigCommandInfo       *command_info);

G_END_DECLS

#endif /* FIG_COMMAND_INFO_H */
