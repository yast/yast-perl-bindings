#! /usr/bin/perl

package Types2;

use warnings;
use strict;

our %TYPEINFO;


use YaST::YCP qw(:DATA);
BEGIN { $TYPEINFO{termloop} = ["function", "term", "term"]; }
sub termloop
{
    my $class = shift;
    return shift;
}

BEGIN { $TYPEINFO{termreverse} = ["function", "term", "term"]; }
sub termreverse
{
    my $class = shift;
    my $t = shift;

    my @rargs = reverse @{$t->args};
    return Term (reverse ($t->name), @rargs);
}

1;
