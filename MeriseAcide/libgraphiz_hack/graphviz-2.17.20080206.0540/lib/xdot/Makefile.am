# $Id: Makefile.am,v 1.1 2008/01/23 20:52:43 ellson Exp $ $Revision: 1.1 $
# ## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	-I$(top_srcdir)/lib/utilities \
	-I$(top_srcdir)/cmd/smyrna \
	$(GTK_CFLAGS) $(GTKGL_CFLAGS) $(GTKGLEXT_CFLAGS) $(GLADE_CFLAGS) $(FREETYPE2_CFLAGS)

if WITH_SMYRNA
noinst_HEADERS = xdot.h
noinst_LTLIBRARIES = libxdot_C.la
endif

libxdot_C_la_SOURCES = xdot.c
