#!/usr/bin/perl

use lib "./modules";
use test_stdHash;

use Data::Dumper;
#use Devel::Peek;

$sOrg = {ab=>"ABCDE", xy=>"XYZ"};
$sWrk = {ab=>"ABCDE", xy=>"XYZ"};
$sOk = {ab=>"ABCDE.ok", xy=>"XYZ.ok", hu=>"he"};

$xOrg = {1=>10, 2=>20, 3=>30};
$xWrk = {1=>10, 2=>20, 3=>30};
$xOut1 = {1=>11, 2=>21, 3=>31, 8=>80};
$xOut2 = {1=>9, 2=>19, 3=>29, 9=>90};

sub Dump {
    Dumper([map {$_, $_[0]->{$_}} sort keys %{$_[0]}]);
}


test_stdHash::IntStr({65=>"A", 123=>"x"});
test_stdHash::IntBool({65=>8, 123=>0});

$sOut = test_stdHash::RStrStr($sWrk);
if (Dump($sWrk) ne Dump($sOk)) {
    print "test RStrStr A fail: ", Data::Dumper->Dump([$sWrk, $sOk], [qw(sWrk sOk)]);
} else {
    print "test RStrStr A ok\n";
}
if (Dump($sOut) ne Dump($sOk)) {
    print "test RStrStr B fail: ", Data::Dumper->Dump([$sOut, $sOk], [qw(sOut sOk)]);
} else {
    print "test RStrStr B ok\n";
}

$xOut = test_stdHash::IntInt($xWrk);
if (Dump($xWrk) ne Dump($xOrg)) {
    print "test IntInt A fail: ", Data::Dumper->Dump([$xWrk, $xOrg], [qw(xWrk xOrg)]);
} else {
    print "test IntInt A ok\n";
}
if (Dump($xOut) ne Dump($xOut1)) {
    print "test IntInt B fail: ", Data::Dumper->Dump([$xOut, $xOut1], [qw(xOut xOout1)]);
} else {
    print "test IntInt B ok\n";
}
test_stdHash::RIntInt($xWrk);
if (Dump($xWrk) ne Dump($xOut2)) {
    print "test RIntInt fail: ", Data::Dumper->Dump([$xWrk, $xOut2], [qw(xWrk xOut2)]);
} else {
    print "test RIntInt ok\n";
}
