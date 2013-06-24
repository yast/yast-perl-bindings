#! /usr/bin/perl

package Types;

use warnings;
use strict;

our %TYPEINFO;


use YaST::YCP qw(Boolean);
BEGIN { $TYPEINFO{bool1} = ["function", "boolean"]; }
sub bool1
{
    my $class = shift;
    return 0;
}

BEGIN { $TYPEINFO{bool2} = ["function", "boolean"]; }
sub bool2
{
    my $class = shift;
    return Boolean (0);
}

1;
