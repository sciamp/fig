/* fig-am-target.h
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

#ifndef FIG_AM_TARGET_H
#define FIG_AM_TARGET_H

#include <glib-object.h>

G_BEGIN_DECLS

#define FIG_TYPE_AM_TARGET            (fig_am_target_get_type())
#define FIG_AM_TARGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_AM_TARGET, FigAmTarget))
#define FIG_AM_TARGET_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_AM_TARGET, FigAmTarget const))
#define FIG_AM_TARGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_AM_TARGET, FigAmTargetClass))
#define FIG_IS_AM_TARGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_AM_TARGET))
#define FIG_IS_AM_TARGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_AM_TARGET))
#define FIG_AM_TARGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_AM_TARGET, FigAmTargetClass))

typedef struct _FigAmTarget        FigAmTarget;
typedef struct _FigAmTargetClass   FigAmTargetClass;
typedef struct _FigAmTargetPrivate FigAmTargetPrivate;

struct _FigAmTarget
{
   GObject parent;

   /*< private >*/
   FigAmTargetPrivate *priv;
};

struct _FigAmTargetClass
{
   GObjectClass parent_class;
};

GType fig_am_target_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* FIG_AM_TARGET_H */
