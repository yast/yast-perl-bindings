#
# Makefile.am for qt/src
#

AM_CXXFLAGS = -DY2LOG=\"Perl\"

serversdir = $(execcompdir)/servers

# plugin, libtool forces 'lib' prefix
plugin_LTLIBRARIES = libpy2perl.la
lib_LTLIBRARIES = liby2perl.la
###	noinst_LTLIBRARIES = liby2perl.la

libpy2perl_la_LDFLAGS = -version-info 2:0
liby2perl_la_LDFLAGS = -version-info 2:0

libpy2perl_la_LIBADD = $(PERL_LDFLAGS)
liby2perl_la_LIBADD  = $(PERL_LDFLAGS)

pkgincludedir = $(includedir)

pkginclude_HEADERS =				\
	YPerl.h					\
						\
	Y2PerlComponent.h


liby2perl_la_SOURCES = 				\
	YPerl.cc


# the plugin adds the liby2 component interface

libpy2perl_la_SOURCES =				\
	$(liby2perl_la_SOURCES)			\
	Y2CCPerl.cc				\
	Y2PerlComponent.cc


CLEANFILES = y2perl.kdoc y2ui.kdoc

INCLUDES = -I$(srcdir)/include -I$(includedir)
