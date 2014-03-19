/* fig-cli.h
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

#ifndef FIG_CLI_H
#define FIG_CLI_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define FIG_TYPE_CLI            (fig_cli_get_type())
#define FIG_CLI(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_CLI, FigCli))
#define FIG_CLI_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_CLI, FigCli const))
#define FIG_CLI_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_CLI, FigCliClass))
#define FIG_IS_CLI(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_CLI))
#define FIG_IS_CLI_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_CLI))
#define FIG_CLI_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_CLI, FigCliClass))

typedef struct _FigCli        FigCli;
typedef struct _FigCliClass   FigCliClass;
typedef struct _FigCliPrivate FigCliPrivate;

struct _FigCli
{
   GObject parent;

   /*< private >*/
   FigCliPrivate *priv;
};

struct _FigCliClass
{
   GObjectClass parent_class;
};

GType          fig_cli_get_type          (void) G_GNUC_CONST;
GOutputStream *fig_cli_get_stderr_stream (FigCli        *cli);
GOutputStream *fig_cli_get_stdout_stream (FigCli        *cli);
FigCli        *fig_cli_new               (void);
gint           fig_cli_run               (FigCli         *cli,
                                          gint            argc,
                                          gchar         **argv);
void           fig_cli_set_stderr_stream (FigCli         *cli,
                                          GOutputStream  *stderr_stream);
void           fig_cli_set_stdout_stream (FigCli         *cli,
                                          GOutputStream  *stderr_stream);

G_END_DECLS

#endif /* FIG_CLI_H */
