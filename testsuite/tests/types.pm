#! /usr/bin/perl

package types;

use warnings;
use strict;

our %TYPEINFO;


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
