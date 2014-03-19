/* fig-command.h
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

#ifndef FIG_COMMAND_H
#define FIG_COMMAND_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define FIG_TYPE_COMMAND            (fig_command_get_type())
#define FIG_COMMAND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND, FigCommand))
#define FIG_COMMAND_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_COMMAND, FigCommand const))
#define FIG_COMMAND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_COMMAND, FigCommandClass))
#define FIG_IS_COMMAND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_COMMAND))
#define FIG_IS_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_COMMAND))
#define FIG_COMMAND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_COMMAND, FigCommandClass))

typedef struct _FigCommand        FigCommand;
typedef struct _FigCommandClass   FigCommandClass;
typedef struct _FigCommandPrivate FigCommandPrivate;

struct _FigCommand
{
   GObject parent;

   /*< private >*/
   FigCommandPrivate *priv;
};

struct _FigCommandClass
{
   GObjectClass parent_class;

   gint (*run) (FigCommand  *command,
                gint         argc,
                gchar      **argv);
};

GType          fig_command_get_type          (void) G_GNUC_CONST;
GOutputStream *fig_command_get_stderr_stream (FigCommand *command);
GOutputStream *fig_command_get_stdout_stream (FigCommand *command);
GFile         *fig_command_get_project_dir   (FigCommand  *command);
gint           fig_command_run               (FigCommand  *command,
                                              gint         argc,
                                              gchar      **argv);

G_END_DECLS

#endif /* FIG_COMMAND_H */
