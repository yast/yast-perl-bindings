#!/usr/bin/perl

use lib "./modules";
use test_String;
#use Data::Dumper;
#use Devel::Peek;

foreach("hello", "žluťoučký kůň", undef, 8) {
	$x=test_String::test_string($_);
	if ($x ne $_) {
	    print "B: test String ",++$i," ... fail\n";
	    print "   $_ => $x\n";
	} else {
	    print "B: test String ",++$i," ... ok\n";
	}
}
