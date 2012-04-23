cappletname = connman

CLEANFILES =
DISTCLEANFILES =
BUILT_SOURCES =
EXTRA_DIST =

glib_enum_c = cc-connman-enum-types.c
glib_enum_h = cc-connman-enum-types.h
glib_enum_headers = cc-technology.h

include Makefile.am.enums

INCLUDES = 						\
	$(PANEL_CFLAGS)					\
	$(CONNMAN_PANEL_CFLAGS)				\
	-DGNOMECC_UI_DIR="\"$(uidir)\""			\
	-DGNOMELOCALEDIR="\"$(datadir)/locale\""	\
	-DGNOMECC_DATA_DIR="\"$(pkgdatadir)\""		\
	$(NULL)

ccpanelsdir = $(PANELS_DIR)
ccpanels_LTLIBRARIES = libconnman.la

gdbus_sources = 					\
	cc-connman-manager.xml				\
	cc-connman-technology.xml			\
	cc-connman-service.xml

gdbus_generated =					\
	cc-connman-generated.h				\
	cc-connman-generated.c

BUILT_SOURCES += $(gdbus_generated)

$(gdbus_generated): $(gdbus_sources) Makefile
	$(AM_V_GEN)gdbus-codegen 			\
		--interface-prefix net.			\
		--c-namespace Cc			\
		--generate-c-code cc-connman-generated	\
		$(gdbus_sources)

libconnman_la_SOURCES =					\
	connman-module.c				\
	$(glib_enum_c)					\
	$(glib_enum_h)					\
	$(glib_enum_headers)				\
	cc-connman-panel.c				\
	cc-connman-panel.h				\
	cc-technology.c

nodist_libconnman_la_SOURCES = $(gdbus_generated)

libconnman_la_LIBADD = $(PANEL_LIBS) $(CONNMAN_LIBS)
libconnman_la_LDFLAGS = $(PANEL_LDFLAGS)

uidir = $(pkgdatadir)/ui
dist_ui_DATA = connman.ui

@INTLTOOL_DESKTOP_RULE@

desktopdir = $(datadir)/applications
desktop_in_files = gnome-connman-panel.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

CLEANFILES += $(desktop_in_files) $(desktop_DATA) $(gdbus_generated)

-include $(top_srcdir)/git.mk
