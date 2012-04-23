/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2012 Intel Coporation
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
 * Authors:
 *   Damien Lespiau <damien.lespiau@intel.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n.h>
#include <stdlib.h>

#include "cc-connman-generated.h"
#include "cc-connman-panel.h"

G_DEFINE_DYNAMIC_TYPE (CcConnmanPanel, cc_connman_panel, CC_TYPE_PANEL)

#define CONNMAN_PANEL_PRIVATE(o) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), CC_TYPE_CONNMAN_PANEL, CcConnmanPanelPrivate))

typedef enum {
        OPERATION_NULL,
        OPERATION_SHOW_DEVICE,
        OPERATION_CREATE_WIFI,
        OPERATION_CONNECT_HIDDEN,
        OPERATION_CONNECT_8021X,
        OPERATION_CONNECT_MOBILE
} CmdlineOperation;

struct _CcConnmanPanelPrivate
{
        GSettings        *proxy_settings;
        GtkBuilder       *builder;

        CmdlineOperation  arg_operation;
        gchar            *arg_device;
        gchar            *arg_access_point;

	CcConnmanManager *manager;
};

enum {
        PANEL_DEVICES_COLUMN_ICON,
        PANEL_DEVICES_COLUMN_TITLE,
        PANEL_DEVICES_COLUMN_SORT,
        PANEL_DEVICES_COLUMN_OBJECT,
        PANEL_DEVICES_COLUMN_LAST
};

enum {
        PANEL_WIRELESS_COLUMN_ID,
        PANEL_WIRELESS_COLUMN_TITLE,
        PANEL_WIRELESS_COLUMN_SORT,
        PANEL_WIRELESS_COLUMN_STRENGTH,
        PANEL_WIRELESS_COLUMN_MODE,
        PANEL_WIRELESS_COLUMN_SECURITY,
        PANEL_WIRELESS_COLUMN_LAST
};

enum {
        PROP_0,
        PROP_ARGV
};

static void
panel_add_technology (const gchar *object_path)
{
        g_print ("New technology added %s\n", object_path);
}

static void
on_get_technologies_done (GObject        *proxy,
			  GAsyncResult   *res,
			  gpointer data)
{
	CcConnmanPanel *panel = data;
	CcConnmanPanelPrivate *priv = panel->priv;
	GVariant *technologies, *technology;
	GVariantIter iter;
	GError *error = NULL;

	cc_connman_manager_call_get_technologies_finish (priv->manager,
							 &technologies,
							 res,
							 &error);
	if (error) {
		g_warning ("Could not retrieve technologies: %s",
			   error->message);
		g_error_free (error);
		return;
	}

	g_variant_iter_init (&iter, technologies);
	while ((technology = g_variant_iter_next_value (&iter))) {
		GVariant *object;
		const gchar *object_path;

		object = g_variant_get_child_value (technology, 0);
		object_path = g_variant_get_string (object, NULL);
		panel_add_technology (object_path);
	}
}

/*
 * GObject implementation
 */

static void
cc_connman_panel_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
        switch (property_id) {
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        }
}

static CmdlineOperation
cmdline_operation_from_string (const gchar *string)
{
        if (g_strcmp0 (string, "create-wifi") == 0)
                return OPERATION_CREATE_WIFI;
        if (g_strcmp0 (string, "connect-hidden-wifi") == 0)
                return OPERATION_CONNECT_HIDDEN;
        if (g_strcmp0 (string, "connect-8021x-wifi") == 0)
                return OPERATION_CONNECT_8021X;
        if (g_strcmp0 (string, "connect-3g") == 0)
                return OPERATION_CONNECT_MOBILE;
        if (g_strcmp0 (string, "show-device") == 0)
                return OPERATION_SHOW_DEVICE;

        g_warning ("Invalid additional argument %s", string);
        return OPERATION_NULL;
}

static void
cc_connman_panel_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
        CcConnmanPanel *self = CC_CONNMAN_PANEL (object);
        CcConnmanPanelPrivate *priv = self->priv;

        switch (property_id) {
        case PROP_ARGV: {
                gchar **args;

                priv->arg_operation = OPERATION_NULL;
                g_free (priv->arg_device);
                priv->arg_device = NULL;
                g_free (priv->arg_access_point);
                priv->arg_access_point = NULL;

                args = g_value_get_boxed (value);

                if (args) {
                        g_debug ("Invoked with operation %s", args[0]);

                        if (args[0])
                                priv->arg_operation = cmdline_operation_from_string (args[0]);
                        if (args[0] && args[1])
                                priv->arg_device = g_strdup (args[1]);
                        if (args[0] && args[1] && args[2])
                                priv->arg_access_point = g_strdup (args[2]);
                }
                break;
        }
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        }
}

static void
cc_connman_panel_dispose (GObject *object)
{
        CcConnmanPanelPrivate *priv = CC_CONNMAN_PANEL (object)->priv;

        g_clear_object (&priv->proxy_settings);
	g_clear_object (&priv->builder);

        G_OBJECT_CLASS (cc_connman_panel_parent_class)->dispose (object);
}

static void
cc_connman_panel_finalize (GObject *object)
{
        CcConnmanPanelPrivate *priv = CC_CONNMAN_PANEL (object)->priv;
        g_free (priv->arg_device);
        g_free (priv->arg_access_point);

        G_OBJECT_CLASS (cc_connman_panel_parent_class)->finalize (object);
}

static void
cc_connman_panel_class_init (CcConnmanPanelClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (CcConnmanPanelPrivate));

        object_class->get_property = cc_connman_panel_get_property;
        object_class->set_property = cc_connman_panel_set_property;
        object_class->dispose = cc_connman_panel_dispose;
        object_class->finalize = cc_connman_panel_finalize;

        g_object_class_override_property (object_class, PROP_ARGV, "argv");
}

static void
cc_connman_panel_class_finalize (CcConnmanPanelClass *klass)
{
}

static void
cc_connman_panel_init (CcConnmanPanel *panel)
{
	CcConnmanPanelPrivate *priv;
        GError *error = NULL;
        GtkWidget *widget;

        panel->priv = priv = CONNMAN_PANEL_PRIVATE (panel);

        priv->builder = gtk_builder_new ();
        gtk_builder_add_from_file (priv->builder,
                                   GNOMECC_UI_DIR "/connman.ui",
                                   &error);
        if (error != NULL) {
                g_warning ("Could not load interface file: %s", error->message);
                g_error_free (error);
                return;
        }

	/* Create the net.connman.Manager proxy */
	priv->manager =
		cc_connman_manager_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
							   G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
							   "net.connman",
							   "/",
							   NULL,
							   &error);
	if (error != NULL) {
		g_warning ("Could not connect net.connman.Manager object: %s",
			   error->message);
		g_error_free (error);
		return;
	}

	cc_connman_manager_call_get_technologies (priv->manager,
						  NULL,
						  on_get_technologies_done,
						  panel);

        /* hide implementation details */
        widget = GTK_WIDGET (gtk_builder_get_object (priv->builder,
                                                     "notebook_types"));
        gtk_notebook_set_show_tabs (GTK_NOTEBOOK (widget), FALSE);

        widget = GTK_WIDGET (gtk_builder_get_object (priv->builder,
                                                     "box2"));
        gtk_widget_reparent (widget, (GtkWidget *) panel);
}

void
cc_connman_panel_register (GIOModule *module)
{
        cc_connman_panel_register_type (G_TYPE_MODULE (module));
        g_io_extension_point_implement (CC_SHELL_PANEL_EXTENSION_POINT,
                                        CC_TYPE_CONNMAN_PANEL,
                                        "network", 0);
}
