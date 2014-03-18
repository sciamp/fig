/* fig-init-command.h
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

#ifndef FIG_INIT_COMMAND_H
#define FIG_INIT_COMMAND_H

#include "fig.h"

G_BEGIN_DECLS

#define FIG_TYPE_INIT_COMMAND            (fig_init_command_get_type())
#define FIG_INIT_COMMAND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_INIT_COMMAND, FigInitCommand))
#define FIG_INIT_COMMAND_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_INIT_COMMAND, FigInitCommand const))
#define FIG_INIT_COMMAND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_INIT_COMMAND, FigInitCommandClass))
#define FIG_IS_INIT_COMMAND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_INIT_COMMAND))
#define FIG_IS_INIT_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_INIT_COMMAND))
#define FIG_INIT_COMMAND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_INIT_COMMAND, FigInitCommandClass))

typedef struct _FigInitCommand        FigInitCommand;
typedef struct _FigInitCommandClass   FigInitCommandClass;
typedef struct _FigInitCommandPrivate FigInitCommandPrivate;

struct _FigInitCommand
{
   FigCommand parent;

   /*< private >*/
   FigInitCommandPrivate *priv;
};

struct _FigInitCommandClass
{
   FigCommandClass parent_class;
};

GType fig_init_command_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* FIG_INIT_COMMAND_H */
