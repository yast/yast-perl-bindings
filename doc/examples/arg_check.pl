#!/usr/bin/perl -w
#
# arg_check.pl
#
# Check YCP/Perl argument passing
#
# Author:  Stefan Hundhammer <sh@suse.de>
# Updated: 2003-07-29

use strict;
use English;


print "You should never see this.\n"
    . "\n"
    . "This is a collection of Perl subs, "
    . "not a script for general use.\n"
    . "\n";
exit 1;


#-----------------------------------------------------------------------------

#
# Dump arguments to stdout
#

sub echoargs()
{
    echoargs_recursive( 0, @_ );
}


#-----------------------------------------------------------------------------


#
# Recursively dump arguments to stdout
#
# Params:
#     $level	indentation level
#     @args	arguments to dump
#

sub echoargs_recursive()
{
    my $level = shift;
    my @args = @_;
    my $arg;

    foreach $arg ( @args )
    {
	print( $level . ": " . ( "." x ( $level * 4 ) ) );
	print( $arg . "\n" );
    }
}




# EOF
