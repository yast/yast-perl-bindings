#!/usr/bin/perl

use lib "./modules";
use test_stdHash;

use Data::Dumper;
#use Devel::Peek;

$in = [{"abc"=>65, "blue"=>1}, {"red"=>255, "blue"=>8}];
$org = [{"abc"=>65, "blue"=>1}, {"red"=>255, "blue"=>8}];
$inc = [{"abc"=>66, "blue"=>2}, {"red"=>256, "blue"=>9}];
$dec = [{"abc"=>64, "blue"=>0}, {"red"=>254, "blue"=>7}];

$out = test_stdHash::ListHash($in);
if (Dumper($out) ne Dumper($dec)) {
    print "test A error: ", Dumper($out, $dec);
}
if (Dumper($in) ne Dumper($org)) {
    print "test B error: ", Dumper($in, $out);
}

test_stdHash::RListHash($in);
if (Dumper($in) ne Dumper($inc)) {
    print "test C error: ", Dumper($in, $inc);
}

