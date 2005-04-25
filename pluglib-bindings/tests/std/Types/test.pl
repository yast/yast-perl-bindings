#!/usr/bin/perl

use lib "./modules";
use test_Types;


$b0 = test_Types::test_bool(0);
$b1 = test_Types::test_bool(1);
if (!$b0 && $b1) {
    print "test bool ... ok\n";
} else {
    print "test bool ... fail\n";
    print "  false => $b0, true => $b1\n";
}

$i = test_Types::test_int(8);
if ($i == 8) {
    print "test int ... ok\n";
} else {
    print "test int ... fail\n";
    print "  8 => $i\n";
}
