#
# Makefile.am for perl-bindings/doc
#

# see examples/Makefile.am for why it is disabled
#SUBDIRS = examples 

htmldir = $(docdir)

html_DATA = YCP-builtins-perl.html
CLEANFILES = $(html_DATA)

builtin_perl = $(srcdir)/../src/YPerl.cc
YCP-builtins-perl.html: $(builtin_perl)
	@ybindir@/makebuiltindocs -t "Perl:: built-in functions" $^ > $@.new; mv $@.new $@

all: $(html_DATA)

EXTRA_DIST = $(html_DATA)
