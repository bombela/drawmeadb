# $Id: Makefile.am,v 1.2 2006/04/07 18:50:37 ellson Exp $ $Revision: 1.2 $
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = no-dependencies foreign

if INSTALL_LTDL
include_HEADERS = ltdl.h
lib_LTLIBRARIES = libltdl.la
else
noinst_HEADERS = ltdl.h
endif

if CONVENIENCE_LTDL
noinst_LTLIBRARIES = libltdlc.la
endif

## Make sure these will be cleaned even when they're not built by
## default.
CLEANFILES = libltdl.la libltdlc.la

libltdl_la_SOURCES = ltdl.c
libltdl_la_LDFLAGS = -no-undefined -version-info 4:0:1
libltdl_la_LIBADD = $(LIBADD_DL)

libltdlc_la_SOURCES = ltdl.c
libltdlc_la_LIBADD = $(LIBADD_DL)

## Because we do not have automatic dependency tracking:
ltdl.lo: ltdl.h config.h

$(libltdl_la_OBJECTS) $(libltdlc_la_OBJECTS): libtool
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

## This allows us to install libltdl without using ln and without creating
## a world writeable directory.
## FIXME:  Remove this rule once automake can do this properly by itself.
local-install-files: $(DISTFILES)
	-rm -rf $(DESTDIR)$(datadir)/libtool/libltdl
	$(mkinstalldirs) $(DESTDIR)$(datadir)/libtool/libltdl
	@for file in $(DISTFILES); do \
	  case $$file in \
	    $(srcdir)/*) file=`echo "$$file" | sed "s|^$(srcdir)/||"`;; \
	  esac; \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -r $$d/$$file $(DESTDIR)$(datadir)/libtool/libltdl/$$file; \
	  else \
	    test -f $(DESTDIR)$(datadir)/libtool/libltdl/$$file \
	    || cp $$d/$$file $(DESTDIR)$(datadir)/libtool/libltdl/$$file || :; \
	  fi; \
	done
