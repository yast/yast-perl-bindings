#
# Makefile.am for perl-bindings/doc/examples
#
# making the examples currently needs that
# "make -C ../../src install" has run
# so for build it is excluded from SUBDIRS in ../Makefile.am

include Makefile.am.common

ycp_examplesdir = 	$(docdir)/examples
perl_examplesdir = 	$(docdir)/examples

FIXME

ycp_examples_DATA =

#for dependencies
module_DATA =					\
	doimport.ycp				\
	example.ycp				\
	foo.ycp					\
	import.ycp				\
	import2.ycp				\
	intermediate.ycp

perl_examples_DATA =				\
	dummy.pm				\
	imported.pm

EXTRA_DIST = $(ycp_examples_DATA) $(perl_examples_DATA)
