TESTS_ENVIRONMENT = PERL5LIB=$(DESTDIR)$(PERL_VENDORARCH) prefix=$(prefix) libdir=$(libdir)
# test-perl writes to the log :(
# but so does test-perl-min
TESTS = test-perl-min test-yast# test-perl
EXTRA_DIST = $(TESTS) tests/*.ycp tests/*.pm tests/A/*.pm tests/*.out tests/*.err tests.pl/*.pl tests.pl/*.ycp tests.pl/*.out tests.pl/*.err
clean-local:
	rm -f tmp.out.* tmp.err.*
