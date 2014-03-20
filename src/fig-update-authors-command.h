/* fig-update-authors-command.h
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

#ifndef FIG_UPDATE_AUTHORS_COMMAND_H
#define FIG_UPDATE_AUTHORS_COMMAND_H

#include "fig-command.h"

G_BEGIN_DECLS

#define FIG_TYPE_UPDATE_AUTHORS_COMMAND            (fig_update_authors_command_get_type())
#define FIG_UPDATE_AUTHORS_COMMAND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_UPDATE_AUTHORS_COMMAND, FigUpdateAuthorsCommand))
#define FIG_UPDATE_AUTHORS_COMMAND_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_UPDATE_AUTHORS_COMMAND, FigUpdateAuthorsCommand const))
#define FIG_UPDATE_AUTHORS_COMMAND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_UPDATE_AUTHORS_COMMAND, FigUpdateAuthorsCommandClass))
#define FIG_IS_UPDATE_AUTHORS_COMMAND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_UPDATE_AUTHORS_COMMAND))
#define FIG_IS_UPDATE_AUTHORS_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_UPDATE_AUTHORS_COMMAND))
#define FIG_UPDATE_AUTHORS_COMMAND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_UPDATE_AUTHORS_COMMAND, FigUpdateAuthorsCommandClass))

typedef struct _FigUpdateAuthorsCommand        FigUpdateAuthorsCommand;
typedef struct _FigUpdateAuthorsCommandClass   FigUpdateAuthorsCommandClass;
typedef struct _FigUpdateAuthorsCommandPrivate FigUpdateAuthorsCommandPrivate;

struct _FigUpdateAuthorsCommand
{
   FigCommand parent;

   /*< private >*/
   FigUpdateAuthorsCommandPrivate *priv;
};

struct _FigUpdateAuthorsCommandClass
{
   FigCommandClass parent_class;
};

GType fig_update_authors_command_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* FIG_UPDATE_AUTHORS_COMMAND_H */
