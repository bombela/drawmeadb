# $Id: Makefile.am,v 1.11 2007/10/25 13:48:20 ellson Exp $ $Revision: 1.11 $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt \
	$(LIBGVC_CFLAGS)

noinst_LTLIBRARIES = libgvplugin_core_C.la
pkglib_LTLIBRARIES = libgvplugin_core.la

libgvplugin_core_C_la_SOURCES = \
	gvplugin_core.c \
	gvrender_core_dot.c \
	gvrender_core_ps.c \
	gvrender_core_fig.c \
	gvrender_core_svg.c \
	gvrender_core_vml.c \
	gvrender_core_map.c \
	gvloadimage_core.c

libgvplugin_core_la_LDFLAGS = -version-info @GVPLUGIN_VERSION_INFO@
libgvplugin_core_la_SOURCES = $(libgvplugin_core_C_la_SOURCES)
libgvplugin_core_la_LIBADD = $(top_builddir)/lib/gvc/libgvc.la

EXTRA_DIST = Makefile.old

