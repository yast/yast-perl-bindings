#!/usr/bin/perl
#
# Example Perl main that uses a Perl module
#

use mod_ex;

print "Executing mod_ex_main.pl\n";

my $msg = mod_ex::helloWorld();
print "mod_ex::helloWorld() returned: $msg\n";

print "mod_ex_main.pl done.\n";

