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

sub echoargs_recursive(@);

#-----------------------------------------------------------------------------

#
# Dump arguments to stdout
#

sub echoargs()
{
    echoargs_recursive( 0, @_ );
    print( "\n" );
}


#-----------------------------------------------------------------------------


#
# Recursively dump arguments to stdout
#
# Params:
#     $level	indentation level
#     @args	arguments to dump
#

sub echoargs_recursive(@)
{
    my $level = shift;
    my @args = @_;
    my $arg;

    foreach $arg ( @args )
    {
	print( " " x ( $level * 4 ) );

	if ( ref( $arg ) )
	{
	    if ( ref( $arg ) eq "ARRAY" )
	    {
		print( "Array reference:\n" );
		echoargs_recursive( $level+1, @$arg );
	    }
	    elsif ( ref( $arg ) eq "HASH" )
	    {
		print( "Hash reference:\n" );

		my %hash = %$arg;
		my $key;

		foreach $key ( sort keys %hash )
		{
		    print( " " x ( ($level+1) * 4 ) );
		    print( "\"$key\" => " );

		    if ( ref( $hash{ $key } ) )
		    {
			print( "\n" );
			echoargs_recursive( $level + 1, $hash{ $key } );
		    }
		    else
		    {
			print( "\"$hash{ $key }\"\n" );
		    }
		}
	    }
	    else
	    {
		print( "Reference to " . ref( $arg ) );
	    }
	}
	else
	{
	    print( "\"$arg\"\n" );
	}
    }
}




# EOF
