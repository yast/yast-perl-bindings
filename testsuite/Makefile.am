TESTS_ENVIRONMENT = PERL5LIB=$(DESTDIR)$(PERL_VENDORARCH)
TESTS = test-perl-min test-yast# test-perl
EXTRA_DIST = $(TESTS) tests/*.ycp tests/*.pl tests/*.pm tests/A/*.pm tests/*.out tests/*.err
clean-local:
	rm -f tmp.out.* tmp.err.*
