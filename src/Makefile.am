#
# Makefile.am for qt/src
#

AM_CXXFLAGS = -DY2LOG=\"Perl\"

serversdir = $(execcompdir)/servers

# plugin, libtool forces 'lib' prefix
plugin_LTLIBRARIES = libpy2perl.la
noinst_LTLIBRARIES = liby2perl.la

libpy2perl_la_LDFLAGS = -version-info 2:0
liby2perl_la_LDFLAGS = -version-info 2:0

libpy2perl_la_LIBADD = $(PERL_LDFLAGS)
liby2perl_la_LIBADD  = $(PERL_LDFLAGS)


liby2perl_la_SOURCES = 				\
	YPerl.cc				\
						\
	perlxsi.c				

# Auto-generated stub for dynamic loading of Perl modules.
#
# This results in a linker warning:
# *** Warning: Linking the shared library libpy2perl.la against the
# *** static library /usr/lib/perl5/.../DynaLoader.a is not portable!
# 
# According to mls@suse.de this warning can safely be disregarded:
# The SuSE DynaLoader is compiled with -fPIC for just this situation.
#
# -- sh@suse.de 2003-07-24
perlxsi.c:
	perl -MExtUtils::Embed -e xsinit -- -o perlxsi.c


# the plugin adds the liby2 component interface

libpy2perl_la_SOURCES =				\
	$(liby2perl_la_SOURCES)			\
	Y2CCPerl.cc				\
	Y2PerlComponent.cc


CLEANFILES = y2perl.kdoc y2ui.kdoc		\
	perlxsi.c

INCLUDES = -I$(srcdir)/include -I$(includedir)
