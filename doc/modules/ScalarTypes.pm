#! /usr/bin/perl -w
#
# Perl module to test scalar types with the yast-perl-bindings.

use strict;
use English;

package ScalarTypes;

our %TYPEINFO;


# BEGIN { print "Executing ScalarTypes module BEGIN block\n"; }
# END   { print "Executing ScalarTypes module END block\n";   }


BEGIN { $TYPEINFO{ hello } = [ "function", "string" ]; }
sub hello()
{
    return "hello";
}


# Perl implicitly converts values that look like numbers to a numeric scalar,
# even if it's in a string. This can be used to test the yast-perl-bindings
# that should use a string when a YCPString was requested.

BEGIN { $TYPEINFO{ str_int } = [ "function", "string" ]; }
sub str_int()
{
    return "42";
}


BEGIN { $TYPEINFO{ str_float } = [ "function", "string" ]; }
sub str_float()
{
    return "42.84";
}



BEGIN { $TYPEINFO{ int_val } = [ "function", "integer" ]; }
sub int_val()
{
    return 42;
}


BEGIN { $TYPEINFO{ float_val } = [ "function", "float" ]; }
sub float_val
{
    return 42.84;
}



BEGIN { $TYPEINFO{ any_str } = [ "function", "any" ]; }
sub any_str()
{
    return "foo";
}


BEGIN { $TYPEINFO{ any_int } = [ "function", "any" ]; }
sub any_int()
{
    return 42;
}


BEGIN { $TYPEINFO{ any_float } = [ "function", "any" ]; }
sub any_float()
{
    return 42.84;
}


# Return value
1;
