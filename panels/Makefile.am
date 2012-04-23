SUBDIRS= \
	common \
	background \
	screen \
	power \
	display \
	mouse \
	online-accounts \
	region \
	info \
	sound \
	keyboard \
	universal-access \
	user-accounts \
	datetime

if BUILD_COLOR
SUBDIRS += color
endif

if BUILD_PRINTERS
SUBDIRS += printers
endif

if BUILD_NETWORK
SUBDIRS += network
endif

if BUILD_CONNMAN_PANEL
SUBDIRS += connman
endif

if BUILD_BLUETOOTH
SUBDIRS += bluetooth
endif

-include $(top_srcdir)/git.mk
