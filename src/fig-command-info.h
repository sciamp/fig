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

#include "fig-command.h"

G_BEGIN_DECLS

#define FIG_TYPE_COMMAND_INFO            (fig_command_info_get_type())
#define FIG_COMMAND_INFO(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND_INFO, FigCommandInfo))
#define FIG_COMMAND_INFO_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND_INFO, FigCommandInfo const))
#define FIG_COMMAND_INFO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_COMMAND_INFO, FigCommandInfoClass))
#define FIG_IS_COMMAND_INFO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_COMMAND_INFO))
#define FIG_IS_COMMAND_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_COMMAND_INFO))
#define FIG_COMMAND_INFO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_COMMAND_INFO, FigCommandInfoClass))

typedef struct _FigCommandInfo        FigCommandInfo;
typedef struct _FigCommandInfoClass   FigCommandInfoClass;
typedef struct _FigCommandInfoPrivate FigCommandInfoPrivate;

typedef FigCommand *(*FigCommandFactory) (FigCommandInfo *info,
                                          gpointer        user_info);

struct _FigCommandInfo
{
   GInitiallyUnowned parent;

   /*< private >*/
   FigCommandInfoPrivate *priv;
};

struct _FigCommandInfoClass
{
   GInitiallyUnownedClass parent_class;

   FigCommand *(*create) (FigCommandInfo *info);
};

FigCommand     *fig_command_info_create           (FigCommandInfo *info);
GType           fig_command_info_get_type         (void) G_GNUC_CONST;
GType           fig_command_info_get_command_type (FigCommandInfo    *info);
const gchar    *fig_command_info_get_description  (FigCommandInfo    *info);
const gchar    *fig_command_info_get_name         (FigCommandInfo    *info);
void            fig_command_info_set_factory      (FigCommandInfo    *info,
                                                   FigCommandFactory  factory,
                                                   gpointer           user_data);
void            fig_command_info_register         (FigCommandInfo    *info);
FigCommandInfo *fig_command_info_lookup           (const gchar       *name);

G_END_DECLS

#endif /* FIG_COMMAND_INFO_H */
