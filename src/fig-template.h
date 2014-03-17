/* fig-template.h
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

#ifndef FIG_TEMPLATE_H
#define FIG_TEMPLATE_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define FIG_TYPE_TEMPLATE            (fig_template_get_type())
#define FIG_TEMPLATE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_TEMPLATE, FigTemplate))
#define FIG_TEMPLATE_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_TEMPLATE, FigTemplate const))
#define FIG_TEMPLATE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_TEMPLATE, FigTemplateClass))
#define FIG_IS_TEMPLATE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_TEMPLATE))
#define FIG_IS_TEMPLATE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_TEMPLATE))
#define FIG_TEMPLATE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_TEMPLATE, FigTemplateClass))

typedef struct _FigTemplate        FigTemplate;
typedef struct _FigTemplateClass   FigTemplateClass;
typedef struct _FigTemplatePrivate FigTemplatePrivate;

struct _FigTemplate
{
   GObject parent;

   /*< private >*/
   FigTemplatePrivate *priv;
};

struct _FigTemplateClass
{
   GObjectClass parent_class;
};

GType        fig_template_get_type (void) G_GNUC_CONST;
FigTemplate *fig_template_new      (const gchar    *name);
gboolean     fig_template_render   (FigTemplate    *templ,
                                    GFile          *file,
                                    GError        **error);

G_END_DECLS

#endif /* FIG_TEMPLATE_H */
