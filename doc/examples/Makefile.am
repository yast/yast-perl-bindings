#
# Makefile.am for perl-bindings/doc/examples
#
# making the examples currently needs that
# "make -C ../../src install" has run
# so for build it is excluded from SUBDIRS in ../Makefile.am

include Makefile.am.common

ycp_examplesdir = 	$(docdir)/examples
perl_examplesdir = 	$(docdir)/examples

ycp_examples_DATA =				\
	ArgCheck.ycp				\
	Eval.ycp				\
	ModEx.ycp				\
	ModParse.ycp				\
	ModUse.ycp				\
	ModWithMain.ycp				\
	Run.ycp

#for dependencies
module_DATA =					\
	doimport.ycp				\
	example.ycp				\
	foo.ycp					\
	import.ycp				\
	import2.ycp				\
	intermediate.ycp

perl_examples_DATA =				\
	arg_check.pl				\
	dummy.pm				\
	imported.pm				\
	mod_ex.pm				\
	mod_ex_main.pl

EXTRA_DIST = $(ycp_examples_DATA) $(perl_examples_DATA)
