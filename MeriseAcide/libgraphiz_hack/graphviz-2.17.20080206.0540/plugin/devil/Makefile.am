# $Id: Makefile.am,v 1.6 2007/10/11 18:14:10 ellson Exp $ $Revision: 1.6 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(PANGOCAIRO_CFLAGS) $(DEVIL_CFLAGS)

if WITH_DEVIL
noinst_LTLIBRARIES = libgvplugin_devil_C.la
pkglib_LTLIBRARIES = libgvplugin_devil.la
endif

libgvplugin_devil_C_la_SOURCES = \
	gvplugin_devil.c \
	gvdevice_devil.c

libgvplugin_devil_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_devil_la_SOURCES = $(libgvplugin_devil_C_la_SOURCES)
libgvplugin_devil_la_LIBADD = @DEVIL_LIBS@

EXTRA_DIST = Makefile.old

