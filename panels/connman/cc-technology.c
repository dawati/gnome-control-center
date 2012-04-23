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

#include "cc-technology.h"
#include "cc-connman-enum-types.h"

G_DEFINE_TYPE (CcTechnology, cc_technology, CC_TYPE_CONNMAN_TECHNOLOGY_PROXY)

#define TECHNOLOGY_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CC_TYPE_TECHNOLOGY, CcTechnologyPrivate))

enum {
	PROP_0,
	PROP_KIND
};

struct _CcTechnologyPrivate {
	CcTechnologyKind kind;
};

static void
cc_technology_get_property (GObject    *object,
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
cc_technology_set_property (GObject      *object,
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
cc_technology_dispose (GObject *object)
{
  G_OBJECT_CLASS (cc_technology_parent_class)->dispose (object);
}

static void
cc_technology_finalize (GObject *object)
{
  G_OBJECT_CLASS (cc_technology_parent_class)->finalize (object);
}

static void
cc_technology_class_init (CcTechnologyClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (CcTechnologyPrivate));

  object_class->get_property = cc_technology_get_property;
  object_class->set_property = cc_technology_set_property;
  object_class->dispose = cc_technology_dispose;
  object_class->finalize = cc_technology_finalize;

  pspec = g_param_spec_enum ("kind",
			     "Kind",
			     "Type of technology",
			     CC_TYPE_TECHNOLOGY_KIND,
			     CC_TECHNOLOGY_KIND_UNKNOWN,
			     G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_KIND, pspec);
}

static void
cc_technology_init (CcTechnology *self)
{
  self->priv = TECHNOLOGY_PRIVATE (self);
}

CcTechnology *
cc_technology_new (const gchar *object_path)
{
	GInitable *ret;

	ret = g_initable_new (CC_TYPE_TECHNOLOGY,
			      NULL, /* cancellable */
			      NULL, /* error */
			      "g-flags", G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
			      "g-name", "net.connman",
			      "g-bus-type", G_BUS_TYPE_SYSTEM,
			      "g-object-path", object_path,
			      "g-interface-name", "net.connman.Technology",
			      NULL);

	if (ret != NULL)
		return CC_TECHNOLOGY (ret);
	else
		return NULL;
}

CcTechnologyKind
cc_technology_get_kind (CcTechnology *technology)
{
	g_return_val_if_fail (CC_IS_TECHNOLOGY (technology),
			      CC_TECHNOLOGY_KIND_UNKNOWN);

	return technology->priv->kind;
}

/*
 * This function returns a string that can be used to sort the different
 * technologies. The order is defined as:
 *   * Ethernet
 *   * Wifi
 *   * WiMax
 *   * Bt
 *   * 3G
 *   * ...
 */
const gchar *
cc_technolgy_to_sort_string (CcTechnology *technology)
{
	CcTechnologyPrivate *priv = technology->priv;
	gchar *value;

	g_return_val_if_fail (CC_IS_TECHNOLOGY (technology), "99");

        switch (priv->kind) {
        case CC_TECHNOLOGY_KIND_ETHERNET:
                value = "1";
                break;
        case CC_TECHNOLOGY_KIND_WIFI:
                value = "2";
                break;
        case CC_TECHNOLOGY_KIND_WIMAX:
                value = "3";
                break;
        case CC_TECHNOLOGY_KIND_BLUETOOTH:
                value = "4";
                break;
        case CC_TECHNOLOGY_KIND_CELLULAR:
                value = "5";
                break;
        default:
                value = "6";
                break;
        }
        return value;
}
