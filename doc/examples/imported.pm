#! /usr/bin/perl -w
# $Id$
#
# Example Perl module for YaST2 transparent Perl bindings
#

package imported;

use strict;

# temporary for in-place development
#use lib '../../src';
use YaST::YCP;

our %TYPEINFO;

#-----------------------------------------------------------------------------

BEGIN
{
    print "Executing imported module BEGIN block\n";
}

END
{
    print "Executing imported module END block\n";
}

#-----------------------------------------------------------------------------

#
# Return message from module variable
#
BEGIN { $TYPEINFO{imported} = ["function", "void"]; }
sub imported()
{
    print "imported\n";
}

BEGIN { $TYPEINFO{plusone} = ["function", "integer", "integer"]; }
sub plusone ()
{
    # here we cheat and return a float instead of an integer
    return $_[0] + 1.33;
}

BEGIN { $TYPEINFO{getmap} = ["function", ["map", "string", "integer"]]; }
sub getmap ()
{
    return {"one" => 1, "two" => 2};
}

BEGIN { $TYPEINFO{getmap2} = ["function", ["map", "string", "integer"], "string"]; }
sub getmap2 ()
{
    return {"one" => 1, "two" => 2, "param" => length ($_[0])};
}

BEGIN { $TYPEINFO{loopback} = ["function", "any", "any"]; }
sub loopback ()
{
    return $_[0];
}

BEGIN { $TYPEINFO{longsymbol} = ["function", "symbol", "integer"]; }
sub longsymbol ()
{
    return "a" x $_[0];
}

sub notypeinfo ()
{
    return 42;
}

# this calls a function in a YCP namespace!
# so far, it is a dummy stub, so any name will do
BEGIN { $TYPEINFO{getback} = ["function", "void"]; }
sub getback ()
{
    use Devel::Peek 'Dump';

    YaST::YCP::Import ("foo");

    my @ret = foo::double([1, 2, 3, 4, 5]);
    print join(" ", @ret), "\n";
#    Dump (\@ret);
    warn "call to foo::double done";

    foo::bar ();
    warn "call to foo::bar done";

    my $sc = foo::scalar ();
    warn "call to foo::scalar done: $sc";

    YaST::YCP::Import ("SCR");
    my @netdir = SCR::Dir (".net");
#    Dump (\@netdir);
    print ("SCR::Dir ('.net') = ", join (" ", @netdir), "\n");
}

BEGIN { $TYPEINFO{fiddle_variables} = ["function", "void"]; }
sub fiddle_variables ()
{
    YaST::YCP::Import ("foo");

    print foo::intvar(), "\n";
    foo::intvar (24);
    print foo::intvar(), "\n";
    foo::intvar ("oops");
    if (0)
    {
	foo::r_intvar (25);
	print foo::intvar(), "\n";

	print foo::localvar(), "\n";
	foo::localvar (78);
	print foo::localvar(), "\n";
    }

}

our $myscalar = 1;
our @myarray = (2, 3);
our %myhash = (4, 5, 6, 7);

# demo when calling from perl
#getback;

# Return value
1;
