#!/usr/bin/perl

use lib "./modules";
use test_Bool;
#use Data::Dumper;
#use Devel::Peek;

foreach(0, 1) {
	$x=test_Bool::test_blocxxBool($_);
	if ($x == $_) {
	    print "test blocxxBool $_ ... fail\n";
	    print "   !$_ => $x\n";
	} else {
	    print "test blocxxBool $_ ... ok\n";
	}

	$x=test_Bool::test_bool($_);
	if ($x == $_) {
	    print "test bool $_ ... fail\n";
	    print "   !$_ => $x\n";
	} else {
	    print "test bool $_ ... ok\n";
	}
}
