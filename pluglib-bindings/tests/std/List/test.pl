#!/usr/bin/perl
use Test::More tests => 2;

use lib "./modules";
use test_stdList;
use test_stdDeque;
use test_stdDeque::mystruct;

use Data::Dumper;
use Devel::Peek;

$m = test_stdDeque::mystruct->new();
$n = $m->swig_numbers_get();
is_deeply($n, [], "empty number list");

$m->swig_numbers_set([7, 7, 7]);
$n = $m->swig_numbers_get();
is_deeply($n, [7, 7, 7], "filled number list");

# FIXME make this a test, make other tests,
# split package, run test automatically,
# publish notes

$i=[1,2,8,-10];
$ix1 = [ @$i ];
$ix2 = [map {$_ * 2} @$i ];
$s=["hello world","žluťoučký kůň úpěl ďábelské ódy",'8'];

$i1 = test_stdList::test_listInt($i);
print "test stdList::test_listInt ... ", (Dumper($i) eq Dumper($i1)) ? "ok\n" : "fail\n" . Dumper($i,$i1);

$s1 = test_stdList::test_listString($s);
print "test stdList::test_listStrnig ... ", (Dumper($s) eq Dumper($s1)) ? "ok\n" : "fail\n" . Dumper($s,$s1);

test_stdList::test_listRefInt($i);
print "test stdList::test_listRefInt ... ", (Dumper($i) eq Dumper($ix2)) ? "ok\n" : "fail\n" . Dumper($i,$ix2);

test_stdList::test_listPInt($i);
print "test stdList::test_listPInt ... ", (Dumper($i) eq Dumper($ix1)) ? "ok\n" : "fail\n" . Dumper($i,$ix1);


$i1 = test_stdDeque::test_Int($i);
print "test stdDeque::test_Int ... ", (Dumper($i) eq Dumper($i1)) ? "ok\n" : "fail\n" . Dumper($i,$i1);

$s1 = test_stdDeque::test_String($s);
print "test stdDeque::test_Strnig ... ", (Dumper($s) eq Dumper($s1)) ? "ok\n" : "fail\n" . Dumper($s,$s1);

test_stdDeque::test_RefInt($i);
print "test stdDeque::test_RefInt ... ", (Dumper($i) eq Dumper($ix2)) ? "ok\n" : "fail\n" . Dumper($i,$ix2);

test_stdDeque::test_PInt($i);
print "test stdDeque::test_PInt ... ", (Dumper($i) eq Dumper($ix1)) ? "ok\n" : "fail\n" . Dumper($i,$ix1);
