#!/usr/bin/perl

use lib "./modules";
use test_Types;
use Math::BigInt;
use Math::BigFloat;
#use Data::Dumper;
#use Devel::Peek;

foreach $size (8, 16, 32, 64) {
    $err="";
    $umax=Math::BigInt->new(2)->bpow($size)->bdec()->bstr(); # (2^N)-1
    $imax=Math::BigInt->new(2)->bpow($size)->bdiv(2)->bdec()->bstr(); # (2^N)/2-1
    $imin=Math::BigInt->new(2)->bpow($size)->bdiv(2)->bneg()->bstr(); # -(2^N)/2
    
    $imin1 = eval "test_Types::test_Int$size('$imin')";
    $err .= "   $@\n" if $@;
    $imax1 = eval "test_Types::test_Int$size('$imax')";
    $err .= "   $@\n" if $@;
    $umax1 = eval "test_Types::test_UInt$size('$umax')";
    $err .= "   $@\n" if $@;
    
    unless ($umax eq $umax1 and $imax eq $imax1 and $imin eq $imin1) {
	$err .= "  $imin => $imin1, $imax => $imax1, $umax => $umax1\n";
    }
    if ($err) {
	print "test (U)Int$size ... fail\n$err";
    } else {
	print "test (U)Int$size ... ok\n";
    }
}

$r = 8.8;
$r1 = test_Types::test_Real32($r);
if ($r - $r1 < 0.0001) {
    print "test Real32 ... ok\n";
} else {
    print "test Real32 ... fail\n";
    print "  $r => $r1\n";
}

$r2 = test_Types::test_Real64($r);
if ($r - $r1 < 0.0001) {
    print "test Real64 ... ok\n";
} else {
    print "test Real64 ... fail\n";
    print "  $r => $r2\n";
}

