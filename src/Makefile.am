#
# Makefile.am for perl-bindings/src
#

AM_CXXFLAGS = -DY2LOG=\"Perl\" -DMODULEDIR=\"$(moduledir)\"
AM_LDFLAGS = -L$(libdir) -L$(plugindir)

perlpmdir = $(PERL_VENDORARCH)/YaST
perlsodir = $(PERL_VENDORARCH)/auto/YaST/YCP

perlpm_DATA = YCP.pm

# plugin, libtool forces 'lib' prefix
plugin_LTLIBRARIES = libpy2lang_perl.la
noinst_LTLIBRARIES = liby2lang_perl.la
perlso_LTLIBRARIES = libYCP.la

libYCP_la_LDFLAGS = -version-info 2:0
# binary part of the Perl module
libYCP_la_SOURCES =				\
	YCP.cc

# are there enough yast libraries?
# check with y2base, integrate them like y2pm does
libYCP_la_LIBADD = $(PERL_LDFLAGS)		\
		-lpy2plugin			\
		-lscr -lyui			\
		-lycp -ly2 -ly2pm -ly2util


libpy2lang_perl_la_LDFLAGS = -version-info 2:0
liby2lang_perl_la_LDFLAGS = -version-info 2:0

# the yast libraries are apparently necessary when we're loaded by perl.
libpy2lang_perl_la_LIBADD = $(PERL_LDFLAGS)	\
		-lycp -ly2 -ly2pm -ly2util
#libpy2lang_perl_la_LIBADD = $(PERL_LDFLAGS)

liby2lang_perl_la_LIBADD  = $(PERL_LDFLAGS)


liby2lang_perl_la_SOURCES =				\
	YPerl.cc YPerl.h			\
	perlxsi.c

# Auto-generated stub for dynamic loading of Perl modules.
# And also register the interface to the YCP module
## which is linked in already and won't be in the standard Perl location.
#
# This results in a linker warning:
# *** Warning: Linking the shared library libpy2lang_perl.la against the
# *** static library /usr/lib/perl5/.../DynaLoader.a is not portable!
#
# According to mls@suse.de this warning can safely be disregarded:
# The SuSE DynaLoader is compiled with -fPIC for just this situation.
#
# -- sh@suse.de 2003-07-24
perlxsi.c:
	perl -MExtUtils::Embed -e xsinit -- -o perlxsi.c -std
# See "man perlembed"


# the plugin adds the liby2 component interface

libpy2lang_perl_la_SOURCES =				\
	$(liby2lang_perl_la_SOURCES)			\
	Y2CCPerl.cc Y2CCPerl.h			\
	YPerlNamespace.cc YPerlNamespace.h	\
	Y2PerlComponent.cc Y2PerlComponent.h


CLEANFILES = \
	perlxsi.c

INCLUDES = -I$(srcdir)/include -I$(includedir)

EXTRA_DIST = $(perlpm_DATA)
