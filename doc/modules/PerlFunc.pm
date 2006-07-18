#! /usr/bin/perl -w
use strict;

package PerlFunc;

our %TYPEINFO;

BEGIN { $TYPEINFO{PlusOne} = ["function", "integer", "integer"]; }
sub PlusOne
{
    my $package = shift;
    my $i = shift;
    return $i + 1;
}

1;
