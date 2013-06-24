#! /usr/bin/perl

package Long;

use warnings;
use strict;

our %TYPEINFO;


use YaST::YCP qw(Integer);

BEGIN { $TYPEINFO{three_billion} = ["function", "integer"]; }
sub three_billion
{
    my $class = shift;
    return 3 * 1024 * 1024 *1024;
}

BEGIN { $TYPEINFO{three_billion_c} = ["function", "integer"]; }
sub three_billion_c
{
    my $class = shift;
    return Integer(3 * 1024 * 1024 *1024);
}

BEGIN { $TYPEINFO{three_trillion} = ["function", "integer"]; }
sub three_trillion
{
    my $class = shift;
    return 3 * 1024 * 1024 * 1024 *1024;
}

BEGIN { $TYPEINFO{three_trillion_c} = ["function", "integer"]; }
sub three_trillion_c
{
    my $class = shift;
    return Integer(3 * 1024 * 1024 * 1024 *1024);
}

BEGIN { $TYPEINFO{big_num} = ["function", "integer", "integer", "integer"]; }
sub big_num
{
    my $class = shift;
    my ($mantissa, $k_exp) = @_;
    foreach (1..$k_exp) {
	$mantissa *= 1024;
    }
    return $mantissa;
}

BEGIN { $TYPEINFO{big_num_c} = ["function", "integer", "integer", "integer"]; }
sub big_num_c
{
    my $class = shift;
    return Integer ($class->big_num (@_));
}

BEGIN { $TYPEINFO{loop} = ["function", "integer", "integer"]; }
sub loop
{
    my $class = shift;
    my $arg =shift;
    return $arg;
}

BEGIN { $TYPEINFO{loop_c} = ["function", "integer", "integer"]; }
sub loop_c
{
    my $class = shift;
    return Integer ($class->loop (@_));
}

1;
