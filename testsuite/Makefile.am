TESTS_ENVIRONMENT = PERL5LIB=$(DESTDIR)$(PERL_VENDORARCH) libdir=$(libdir)
TESTS = test-perl-min test-yast# test-perl
EXTRA_DIST = $(TESTS) tests/*.ycp tests/*.pm tests/A/*.pm tests/*.out tests/*.err tests.pl/*.pl tests.pl/*.ycp tests.pl/*.out tests.pl/*.err
clean-local:
	rm -f tmp.out.* tmp.err.*
