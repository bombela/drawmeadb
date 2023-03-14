# $Id: Makefile.am,v 1.2 2008/01/24 03:11:45 ellson Exp $ $Revision: 1.2 $
# ## Process this file with automake to produce Makefile.in
#
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/utilities \
	-I$(top_srcdir)/lib/gui \
	-I$(top_srcdir)/lib/topfish \
	-I$(top_srcdir)/cmd/smyrna \
	-I$(top_srcdir)/lib/xdot \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(GTK_CFLAGS) $(GTKGL_CFLAGS) $(GTKGLEXT_CFLAGS) $(GLADE_CFLAGS) $(FREETYPE2_CFLAGS)

if WITH_SMYRNA
noinst_HEADERS = \
	btree.h filter.h 

noinst_LTLIBRARIES = libfilter_C.la
endif

libfilter_C_la_SOURCES = \
	Filter.c btree.c

