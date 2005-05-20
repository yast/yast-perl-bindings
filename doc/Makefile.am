#
# Makefile.am for perl-bindings/doc
#

# see examples/Makefile.am for why it is disabled
#SUBDIRS = examples 

htmldir = $(docdir)
txtdir = $(docdir)

html_DATA = YaST__YCP.html YaPI.html
txt_DATA = typeinfo
CLEANFILES = $(html_DATA)

YaST__YCP.html: $(srcdir)/../src/YCP.pm
	pod2html < $^ > $@

YaPI.html: $(srcdir)/../src/YaPI.pm
	pod2html < $^ > $@

EXTRA_DIST = $(html_DATA) $(txt_DATA)
