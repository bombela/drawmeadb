# $Id: Makefile.am,v 1.6 2007/10/11 18:14:10 ellson Exp $ $Revision: 1.6 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
        -I$(top_srcdir)/lib/common \
        -I$(top_srcdir)/lib/pathplan \
        -I$(top_srcdir)/lib/gvc \
        -I$(top_srcdir)/lib/graph \
        -I$(top_srcdir)/lib/cdt \
	$(GTK_CFLAGS) $(GNOMEUI_CFLAGS)

if WITH_PANGOCAIRO
if WITH_GTK
noinst_HEADERS = support.h interface.h callbacks.h
noinst_LTLIBRARIES = libgvplugin_gtk_C.la
pkglib_LTLIBRARIES = libgvplugin_gtk.la
endif
endif

libgvplugin_gtk_C_la_SOURCES = \
	gvplugin_gtk.c \
	gvdevice_gtk.c \
	support.c \
        interface.c \
        callbacks.c 

libgvplugin_gtk_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_gtk_la_SOURCES = $(libgvplugin_gtk_C_la_SOURCES)
libgvplugin_gtk_la_LIBADD = $(GTK_LIBS) $(GNOMEUI_LIBS)

EXTRA_DIST = gtk.glade gtk.gladep 
