#!/usr/bin/perl

use lib "./modules";
use test_String;
#use Data::Dumper;
#use Devel::Peek;

foreach my $s ("hello", "žluťoučký kůň", undef, 8) {
	$s1 = $s;
	$x=test_String::test_String($s);
	if ($x ne $s) {
	    print "A: test String ",++$i," ... fail\n";
	    print "   $s => $x\n";
	} else {
	    print "A: test String ",++$i," ... ok\n";
	}

	$x=test_String::test_RString(\$s);
	if ($x ne $s) {
	    print "B: test String ",++$i," ... fail\n";
	    print "   $s => $x\n";
	} else {
	    print "B: test String ",++$i," ... ok\n";
	}
	if ($x ne $s1.".A") {
	    print "C: test String ",++$i," ... fail\n";
	    print "   ${s1}.A => $x\n";
	} else {
	    print "C: test String ",++$i," ... ok\n";
	}
}
