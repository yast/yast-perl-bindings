#! /usr/bin/perl

package reference;

use warnings;
use strict;

our %TYPEINFO;


use YaST::YCP qw(:DATA);
BEGIN { $TYPEINFO{refInt} = ["function", "void", "&integer"]; }
sub refInt
{
    ${$_[1]} += 8;
}

BEGIN { $TYPEINFO{refBool} = ["function", "void", "&boolean"]; }
sub refBool
{
    ${$_[1]} = 1;
}

BEGIN { $TYPEINFO{refString} = ["function", "void", "&string"]; }
sub refString
{
    ${$_[1]} .= "-ok";
}

BEGIN { $TYPEINFO{refFloat} = ["function", "void", "&float"]; }
sub refFloat
{
    ${$_[1]} *= 2;
}

BEGIN { $TYPEINFO{refListInt} = ["function", "void", ["&list", "integer"]]; }
sub refListInt
{
    $_[1][0] = 8;
    $_[1][1]++;
    push @{$_[1]}, "-3";
}

BEGIN { $TYPEINFO{refMapStringString} = ["function", "void", ["&map", "string", "string"]]; }
sub refMapStringString
{
    $_[1]->{a} = "A";
    $_[1]->{b} .= "-ok";
    delete $_[1]->{c};
}

1;
