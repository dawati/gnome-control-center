/*
 * Copyright (C) 2010 Intel, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Author: Thomas Wood <thomas.wood@intel.com>
 *
 */

#include "cc-keybindings-panel.h"

G_DEFINE_DYNAMIC_TYPE (CcKeybindingsPanel, cc_keybindings_panel, CC_TYPE_PANEL)

#define KEYBINDINGS_PANEL_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CC_TYPE_KEYBINDINGS_PANEL, CcKeybindingsPanelPrivate))

struct _CcKeybindingsPanelPrivate
{
  GtkBuilder *builder;
};


static void
cc_keybindings_panel_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
cc_keybindings_panel_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
cc_keybindings_panel_dispose (GObject *object)
{
  G_OBJECT_CLASS (cc_keybindings_panel_parent_class)->dispose (object);
}

static void
cc_keybindings_panel_finalize (GObject *object)
{
  G_OBJECT_CLASS (cc_keybindings_panel_parent_class)->finalize (object);
}

static void
cc_keybindings_panel_class_init (CcKeybindingsPanelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (CcKeybindingsPanelPrivate));

  object_class->get_property = cc_keybindings_panel_get_property;
  object_class->set_property = cc_keybindings_panel_set_property;
  object_class->dispose = cc_keybindings_panel_dispose;
  object_class->finalize = cc_keybindings_panel_finalize;
}

static void
cc_keybindings_panel_class_finalize (CcKeybindingsPanelClass *klass)
{
}

static void
cc_keybindings_panel_init (CcKeybindingsPanel *self)
{
  CcKeybindingsPanelPrivate *priv;
  GError *error = NULL;
  const gchar *uifile = GNOMECC_UI_DIR "/gnome-keybinding-properties.ui";

  priv = self->priv = KEYBINDINGS_PANEL_PRIVATE (self);


  priv->builder = gtk_builder_new ();

  if (gtk_builder_add_from_file (priv->builder, uifile, &error) == 0)
    {
      g_warning ("Could not load UI: %s", error->message);
      g_clear_error (&error);
      g_object_unref (priv->builder);
      priv->builder = NULL;
      return;
    }

  gnome_keybinding_properties_init (priv->builder);

  GtkWidget *widget;

  widget = (GtkWidget *) gtk_builder_get_object (priv->builder,
                                                 "vbox3");

  gtk_widget_reparent (widget, (GtkWidget *) self);
}

void
cc_keybindings_panel_register (GIOModule *module)
{
  cc_keybindings_panel_register_type (G_TYPE_MODULE (module));
  g_io_extension_point_implement (CC_SHELL_PANEL_EXTENSION_POINT,
                                  CC_TYPE_KEYBINDINGS_PANEL,
                                  "keybinding", 0);
}
