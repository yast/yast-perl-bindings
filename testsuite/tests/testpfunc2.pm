#! /usr/bin/perl

package testpfunc;
use warnings;


BEGIN { $TYPEINFO{rxmatch} = ["function", "boolean", "string", "string"]; }
sub rxmatch
{
    my ($string, $pattern) = @_;
    return $string =~ $pattern;
}

BEGIN { $TYPEINFO{lengths} = ["function", ["list", "integer"], ["list", "string"]]; }

=pod

Computes the lengths of the strings in the list.

=cut

sub lengths
{
    # warning, we get a REFERENCE to the list of strings
#    return [ map (length, @{$_[0]}) ];
    return map (length, @{$_[0]});
}

BEGIN { $TYPEINFO{amap} = ["function", [ 'map', 'string', 'any' ], [ 'map', 'string', 'any' ]]; }
sub amap
{
    my $data = shift;

    my %returnMap = (a => 1, b => "two");

    return \%returnMap;
}

BEGIN { $TYPEINFO{run} = ["function", "symbol"]; }
sub run
{
    print STDERR $ENV{PERL5LIB}, "\n";
    use YaST::YCP qw(:LOGGING);
    y2milestone "Hello, YCP!";
    return "next";
}

use YaST::YCP qw(Boolean);
BEGIN { $TYPEINFO{bool1} = ["function", "boolean"]; }
sub bool1
{
    return 0;
}

BEGIN { $TYPEINFO{bool2} = ["function", "boolean"]; }
sub bool2
{
    return Boolean (0);
}

1;
