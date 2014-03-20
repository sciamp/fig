/* fig-add-target-command.h
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

#ifndef FIG_ADD_TARGET_COMMAND_H
#define FIG_ADD_TARGET_COMMAND_H

#include "fig-command.h"
#include "fig-target-type.h"

G_BEGIN_DECLS

#define FIG_TYPE_ADD_TARGET_COMMAND            (fig_add_target_command_get_type())
#define FIG_ADD_TARGET_COMMAND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_ADD_TARGET_COMMAND, FigAddTargetCommand))
#define FIG_ADD_TARGET_COMMAND_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_ADD_TARGET_COMMAND, FigAddTargetCommand const))
#define FIG_ADD_TARGET_COMMAND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_ADD_TARGET_COMMAND, FigAddTargetCommandClass))
#define FIG_IS_ADD_TARGET_COMMAND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_ADD_TARGET_COMMAND))
#define FIG_IS_ADD_TARGET_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_ADD_TARGET_COMMAND))
#define FIG_ADD_TARGET_COMMAND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_ADD_TARGET_COMMAND, FigAddTargetCommandClass))

typedef struct _FigAddTargetCommand        FigAddTargetCommand;
typedef struct _FigAddTargetCommandClass   FigAddTargetCommandClass;
typedef struct _FigAddTargetCommandPrivate FigAddTargetCommandPrivate;

struct _FigAddTargetCommand
{
   FigCommand parent;

   /*< private >*/
   FigAddTargetCommandPrivate *priv;
};

struct _FigAddTargetCommandClass
{
   FigCommandClass parent_class;
};

GType fig_add_target_command_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* FIG_ADD_TARGET_COMMAND_H */
