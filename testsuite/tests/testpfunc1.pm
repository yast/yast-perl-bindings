#! /usr/bin/perl

package testpfunc1;
use warnings;
use strict;

our %TYPEINFO;

BEGIN { $TYPEINFO{rxmatch} = ["function", "boolean", "string", "string"]; }
sub rxmatch
{
    my $self = shift;
    my ($string, $pattern) = @_;
    return $string =~ $pattern;
}

BEGIN { $TYPEINFO{lengths} = ["function", ["list", "integer"], ["list", "string"]]; }

=pod

Computes the lengths of the strings in the list.

=cut

sub lengths
{
    my $self = shift;
    # warning, we get a REFERENCE to the list of strings
    return [ map (length, @{$_[0]}) ];
}

BEGIN { $TYPEINFO{amap} = ["function", [ 'map', 'string', 'any' ], [ 'map', 'string', 'any' ]]; }
sub amap
{
    my $self = shift;
    my $data = shift;

    my %returnMap = (a => 1, b => "two");

    return \%returnMap;
}

1;
