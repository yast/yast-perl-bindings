#!/usr/bin/perl

use lib "./modules";
use Data::Dumper;

use test_listList;

$sOrg = [[ "ab", "cd"], ["xyz"]];
$sWrk = [ map {[@$_]} @$sOrg ];
$sOk = [ map {[map {$_.".ok"} @$_]} @$sOrg ];

$lOrg = [ [ 1, 2, 3], [10, 11], [-3, -8], []];
$lDbl = [ map {[map {$_*2} @$_]} @$lOrg ];
$lWrk = [ map {[@$_]} @$lOrg ];

$sOut=test_listList::RListListString($sWrk);
if (Dumper($sWrk) ne Dumper($sOk)) {
    print STDERR "test RListListString A fail: ", Data::Dumper->Dump([$sOut, $sOk], [qw(sOut sOk)]);
} else {
    print "test RListListString A ok\n";
}
if (Dumper($sWrk) ne Dumper($sOk)) {
    print STDERR "test RListListString B fail: ", Data::Dumper->Dump([$sOut, $sOk], [qw(sOut sOk)]);
} else {
    print "test RListListString B ok\n";
}


$lOut=test_listList::ListListInt($lWrk);
if (Dumper($lWrk) ne Dumper($lOrg)) {
    print STDERR "test ListListInt A fail: ", Data::Dumper->Dump([$lWrk, $lOrg], [qw(lWrk lOrg)]);
} else {
    print "test ListListInt A ok\n";
}
if (Dumper($lOut) ne Dumper($lDbl)) {
    print STDERR "test ListListInt B fail: ", Data::Dumper->Dump([$lOut, $lDbl], [qw(lOut lDbl)]);
} else {
    print "test ListListInt B ok\n";
}

test_listList::RListListInt($lWrk);
if (Dumper($lWrk) ne Dumper($lDbl)) {
    print STDERR "test RListListInt fail: ", Data::Dumper->Dump([$lWrk, $lDbl], [qw(lWrk lDbl)]);
} else {
    print "test RListListInt ok\n";
}

test_listList::PListListInt($lWrk);
if (Dumper($lWrk) ne Dumper($lOrg)) {
    print STDERR "test PListListInt fail: ", Data::Dumper->Dump([$lWrk, $lOrg], [qw(lWrk lOrg)]);
} else {
    print "test PListListInt ok\n";
}

