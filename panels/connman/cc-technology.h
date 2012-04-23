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

#ifndef __CC_TECHNOLOGY_H__
#define __CC_TECHNOLOGY_H__

#include <glib-object.h>

#include "cc-connman-generated.h"

G_BEGIN_DECLS

#define CC_TYPE_TECHNOLOGY cc_technology_get_type()

#define CC_TECHNOLOGY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CC_TYPE_TECHNOLOGY, CcTechnology))

#define CC_TECHNOLOGY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CC_TYPE_TECHNOLOGY, CcTechnologyClass))

#define CC_IS_TECHNOLOGY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CC_TYPE_TECHNOLOGY))

#define CC_IS_TECHNOLOGY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CC_TYPE_TECHNOLOGY))

#define CC_TECHNOLOGY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CC_TYPE_TECHNOLOGY, CcTechnologyClass))

typedef struct _CcTechnology CcTechnology;
typedef struct _CcTechnologyClass CcTechnologyClass;
typedef struct _CcTechnologyPrivate CcTechnologyPrivate;

typedef enum _CcTechnologyKind
{
	CC_TECHNOLOGY_KIND_UNKNOWN,
	CC_TECHNOLOGY_KIND_ETHERNET,
	CC_TECHNOLOGY_KIND_WIFI,
	CC_TECHNOLOGY_KIND_WIMAX,
	CC_TECHNOLOGY_KIND_BLUETOOTH,
	CC_TECHNOLOGY_KIND_CELLULAR
} CcTechnologyKind;

struct _CcTechnology
{
  CcConnmanTechnologyProxy parent;

  CcTechnologyPrivate *priv;
};

struct _CcTechnologyClass
{
  CcTechnologyProxyClass parent_class;
};

GType cc_technology_get_type (void) G_GNUC_CONST;

CcTechnology *		cc_technology_new 		(const gchar *object_path);
CcTechnologyKind 	cc_technology_get_kind		(CcTechnology *technology);
const gchar * 		cc_technolgy_to_sort_string 	(CcTechnology *technology);

G_END_DECLS

#endif /* __CC_TECHNOLOGY_H__ */
