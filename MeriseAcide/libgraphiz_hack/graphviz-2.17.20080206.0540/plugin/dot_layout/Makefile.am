# $Id: Makefile.am,v 1.3 2007/10/11 18:14:10 ellson Exp $ $Revision: 1.3 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(LIBGVC_CFLAGS)

noinst_LTLIBRARIES = libgvplugin_dot_layout_C.la

pkglib_LTLIBRARIES = libgvplugin_dot_layout.la

libgvplugin_dot_layout_C_la_SOURCES = \
	gvplugin_dot_layout.c \
	gvlayout_dot_layout.c
libgvplugin_dot_layout_C_la_LIBADD = \
	$(top_builddir)/lib/dotgen/libdotgen_C.la

libgvplugin_dot_layout_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_dot_layout_la_SOURCES = $(libgvplugin_dot_layout_C_la_SOURCES)
libgvplugin_dot_layout_la_LIBADD = $(libgvplugin_dot_layout_C_la_LIBADD) \
	$(top_builddir)/lib/gvc/libgvc.la \
	$(top_builddir)/lib/pathplan/libpathplan.la

EXTRA_DIST = Makefile.old

