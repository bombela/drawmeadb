# $Id: Makefile.am,v 1.8 2007/10/11 18:14:10 ellson Exp $ $Revision: 1.8 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(PANGOCAIRO_CFLAGS)

if WITH_PANGOCAIRO
if WITH_X
noinst_LTLIBRARIES = libgvplugin_xlib_C.la
pkglib_LTLIBRARIES = libgvplugin_xlib.la
endif
endif

libgvplugin_xlib_C_la_SOURCES = \
	gvplugin_xlib.c \
	gvdevice_xlib.c

libgvplugin_xlib_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_xlib_la_SOURCES = $(libgvplugin_xlib_C_la_SOURCES)
libgvplugin_xlib_la_LIBADD = @PANGOCAIRO_LIBS@ @X_LIBS@ -lX11 @SOCKET_LIBS@ @MATH_LIBS@
