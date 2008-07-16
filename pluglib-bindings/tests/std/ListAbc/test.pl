#!/usr/bin/perl
use Test::More tests => 3;

use lib "./modules";
use test;
use test::abc;

use Data::Dumper;
#use Devel::Peek;

$d = test::abc->new();
$d->swig_a_set(1);
$d->swig_b_set(-2);
$d->swig_c_set(3);
is($d->swig_a_get(), 1, ".a");
is($d->swig_b_get(), -2, ".b");
is($d->swig_c_get(), 3, ".c");

$abc = [];
$xyz = test::test_listAbc($abc);
print Dumper($xyz);
$xyz = test::test_listAbc($xyz);
print Dumper($xyz);
$xyz = test::test_listAbc($xyz);
print Dumper($xyz);

test::test_RlistAbc($xyz);
print Dumper($xyz);
test::test_RlistAbc($xyz);
print Dumper($xyz);
