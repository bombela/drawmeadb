# $Id: Makefile.am,v 1.1 2007/12/28 21:27:36 ellson Exp $ $Revision: 1.1 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(RSVG_CFLAGS) $(PANGOCAIRO_CFLAGS)

if WITH_RSVG
noinst_LTLIBRARIES = libgvplugin_rsvg_C.la
pkglib_LTLIBRARIES = libgvplugin_rsvg.la
endif

libgvplugin_rsvg_C_la_SOURCES = \
	gvplugin_rsvg.c \
	gvloadimage_rsvg.c

libgvplugin_rsvg_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_rsvg_la_SOURCES = $(libgvplugin_rsvg_C_la_SOURCES)
libgvplugin_rsvg_la_LIBADD = \
	$(top_builddir)/lib/gvc/libgvc.la @RSVG_LIBS@ @PANGOCAIRO_LIBS@

EXTRA_DIST = Makefile.old

