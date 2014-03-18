/* fig-command-manager.h
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

#ifndef FIG_COMMAND_MANAGER_H
#define FIG_COMMAND_MANAGER_H

#include <glib-object.h>

#include "fig-command-info.h"

G_BEGIN_DECLS

#define FIG_TYPE_COMMAND_MANAGER            (fig_command_manager_get_type())
#define FIG_COMMAND_MANAGER_DEFAULT         (fig_command_manager_get_default())
#define FIG_COMMAND_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND_MANAGER, FigCommandManager))
#define FIG_COMMAND_MANAGER_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND_MANAGER, FigCommandManager const))
#define FIG_COMMAND_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_COMMAND_MANAGER, FigCommandManagerClass))
#define FIG_IS_COMMAND_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_COMMAND_MANAGER))
#define FIG_IS_COMMAND_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_COMMAND_MANAGER))
#define FIG_COMMAND_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_COMMAND_MANAGER, FigCommandManagerClass))

typedef struct _FigCommandManager        FigCommandManager;
typedef struct _FigCommandManagerClass   FigCommandManagerClass;
typedef struct _FigCommandManagerPrivate FigCommandManagerPrivate;

struct _FigCommandManager
{
   GObject parent;

   /*< private >*/
   FigCommandManagerPrivate *priv;
};

struct _FigCommandManagerClass
{
   GObjectClass parent_class;
};

GType              fig_command_manager_get_type     (void) G_GNUC_CONST;
FigCommandManager *fig_command_manager_get_default  (void);
GList             *fig_command_manager_get_commands (FigCommandManager    *manager);
FigCommandInfo    *fig_command_manager_lookup       (FigCommandManager    *manager,
                                                     const gchar          *command);
void               fig_command_manager_register     (FigCommandManager    *manager,
                                                     const FigCommandInfo *command_info);

G_END_DECLS

#endif /* FIG_COMMAND_MANAGER_H */
