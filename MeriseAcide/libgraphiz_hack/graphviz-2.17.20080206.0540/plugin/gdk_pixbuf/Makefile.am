# $Id: Makefile.am,v 1.7 2007/10/26 12:10:50 ellson Exp $ $Revision: 1.7 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(GDK_PIXBUF_CFLAGS)

if WITH_GDK_PIXBUF
noinst_LTLIBRARIES = libgvplugin_gdk_pixbuf_C.la
pkglib_LTLIBRARIES = libgvplugin_gdk_pixbuf.la
endif

libgvplugin_gdk_pixbuf_C_la_SOURCES = \
	gvplugin_gdk_pixbuf.c \
	gvdevice_gdk_pixbuf.c

libgvplugin_gdk_pixbuf_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_gdk_pixbuf_la_SOURCES = $(libgvplugin_gdk_pixbuf_C_la_SOURCES)
libgvplugin_gdk_pixbuf_la_LIBADD = \
	$(top_builddir)/lib/gvc/libgvc.la @GDK_PIXBUF_LIBS@

EXTRA_DIST = Makefile.old

