# $Id: Makefile.am,v 1.1 2008/01/23 20:52:43 ellson Exp $ $Revision: 1.1 $
# ## Process this file with automake to produce Makefile.in
#
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/cmd/smyrna \
	-I$(top_srcdir)/lib/utilities \
	-I$(top_srcdir)/lib/xdot \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(GTK_CFLAGS) $(GTKGL_CFLAGS) $(GTKGLEXT_CFLAGS) $(GLADE_CFLAGS) $(FREETYPE2_CFLAGS)

if WITH_SMYRNA
noinst_HEADERS = \
	abstring.h glTexFontDefs.h glTexFont.h glTexFontInclude.h \
	glTexFontTGA.h opengl_pango.h selection.h 

noinst_LTLIBRARIES = libutilities_C.la
endif

libutilities_C_la_SOURCES = \
	abstring.c glTexFont.c glTexFontColor.c glTexFontTGA.c \
	opengl_pango.c selection.c
