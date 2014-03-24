/* fig-am-script.h
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

#ifndef FIG_AM_SCRIPT_H
#define FIG_AM_SCRIPT_H

#include <gio/gio.h>

#include "fig-am-target.h"

G_BEGIN_DECLS

#define FIG_TYPE_AM_SCRIPT            (fig_am_script_get_type())
#define FIG_AM_SCRIPT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_AM_SCRIPT, FigAmScript))
#define FIG_AM_SCRIPT_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_AM_SCRIPT, FigAmScript const))
#define FIG_AM_SCRIPT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_AM_SCRIPT, FigAmScriptClass))
#define FIG_IS_AM_SCRIPT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_AM_SCRIPT))
#define FIG_IS_AM_SCRIPT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_AM_SCRIPT))
#define FIG_AM_SCRIPT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_AM_SCRIPT, FigAmScriptClass))

typedef struct _FigAmScript        FigAmScript;
typedef struct _FigAmScriptClass   FigAmScriptClass;
typedef struct _FigAmScriptPrivate FigAmScriptPrivate;

struct _FigAmScript
{
   GObject parent;

   /*< private >*/
   FigAmScriptPrivate *priv;
};

struct _FigAmScriptClass
{
   GObjectClass parent_class;
};

GType        fig_am_script_get_type      (void) G_GNUC_CONST;
FigAmScript *fig_am_script_new           (GFile         *file);
gboolean     fig_am_script_add_target    (FigAmScript   *script,
                                          FigAmTarget   *target,
                                          GError       **error);
gboolean     fig_am_script_modify_target (FigAmScript   *script,
                                          FigAmTarget   *target,
                                          GError       **error);
gboolean     fig_am_script_remove_target (FigAmScript   *script,
                                          FigAmTarget   *target,
                                          GError       **error);
gboolean     fig_am_script_add_include   (FigAmScript   *script,
                                          GFile         *include,
                                          GError       **error);
gboolean     fig_am_script_save          (FigAmScript   *script,
                                          GCancellable  *cancellable,
                                          GError       **error);

G_END_DECLS

#endif /* FIG_AM_SCRIPT_H */
