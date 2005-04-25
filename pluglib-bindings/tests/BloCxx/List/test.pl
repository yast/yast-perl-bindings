#!/usr/bin/perl

use lib "./modules";
use test_blocxxList;

use Data::Dumper;
#use Devel::Peek;

$i=[1,2,8,-10];
$ix1 = [ @$i ];
$ix2 = [map {$_ * 2} @$i ];
$s=["hello world","žluťoučký kůň úpěl ďábelské ódy",'8'];


$i2 = test_blocxxList::test_listInt32($i);
print "test blocxxList::test_listInt32 ... ", (Dumper($i) eq Dumper($i2)) ? "ok\n" : "fail\n" . Dumper($i,$i2);

$s2 = test_blocxxList::test_listString($s);
print "test blocxxList::test_listString ... ", (Dumper($s) eq Dumper($s2)) ? "ok\n" : "fail\n" . Dumper($s,$s2);
