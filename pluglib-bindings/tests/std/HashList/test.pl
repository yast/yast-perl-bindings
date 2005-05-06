#!/usr/bin/perl

use lib "./modules";
use test_stdHash;

use Data::Dumper;
#use Devel::Peek;

$in = {"abc" => [65, 66, 67], "123" => [1, 2, 3], "red" => [255, 0, 0]};
$org = {"abc" => [65, 66, 67], "123" => [1, 2, 3], "red" => [255, 0, 0]};
$inc = {"abc" => [66, 67, 68], "123" => [2, 3, 4], "red" => [256, 1, 1]};
$dec = {"abc" => [64, 65, 66], "123" => [0, 1, 2], "red" => [254, -1, -1]};

$out = test_stdHash::HashList($in);
if (Dumper($out) ne Dumper($dec)) {
    print "test A error: ", Dumper($out, $dec);
}
if (Dumper($in) ne Dumper($org)) {
    print "test B error: ", Dumper($in, $out);
}

test_stdHash::RHashList($in);
if (Dumper($in) ne Dumper($inc)) {
    print "test C error: ", Dumper($in, $inc);
}

