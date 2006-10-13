#! /usr/bin/perl
# A first example of a YaST module in Perl

package PerlHello;

BEGIN { $TYPEINFO{Hello} = ["function", "void"]; }
sub Hello
{
    print "Hello, YaST\n";
}

1;
