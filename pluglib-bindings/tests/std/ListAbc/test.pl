#!/usr/bin/perl

use lib "./modules";
use test;

use Data::Dumper;
#use Devel::Peek;

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
