#! /usr/bin/perl -w
# Martin Vidner
# $Id$

=head1 NAME

YaST::YCP - a binary interface between Perl and YCP

=head1 SYNOPSIS

 use YaST::YCP qw(Boolean);

 YaST::YCP::Import ("SCR");
 my $m = SCR::Read (".sysconfig.displaymanager.DISPLAYMANAGER");
 SCR::Write (".sysconfig.kernel.CRASH_OFTEN", Boolean (1));

=head1 YaST::YCP

=cut

package YaST::YCP;
use strict;
use warnings;
use diagnostics;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT_OK = qw(Boolean);

=head2 debug

 $olddebug = YaST::YCP::debug (1);
 YaST::YCP::...
 YaST::YCP::debug ($olddebug);

Enables miscellaneous unscpecified debugging

=cut

my $debug = 0;
sub debug (;$)
{
    my $param = shift;
    if (defined $param)
    {
	$debug = $param;
    }
    return $debug;
}


## calls boot_YaST__YCP
require XSLoader;
XSLoader::load ('YaST::YCP');

=head2 Import

 YaST::YCP::Import "Namespace";
 Namespace::foo ("bar");

=cut

sub Import ($)
{
    my $package = shift;
    print "Importing $package" if debug;

    no strict;
    # let it get our autoload
    *{"${package}::AUTOLOAD"} = \&YaST::YCP::Autoload::AUTOLOAD;
}

# shortcuts for the data types
# for POD see packages below

sub Boolean ($)
{
    return new YaST::YCP::Boolean (@_);
}

# by defining AUTOLOAD in a separate package, undefined functions in
# the main one will be detected
package YaST::YCP::Autoload;
use strict;
use warnings;
use diagnostics;

# cannot rely on UNIVERSAL::AUTOLOAD getting automatically called
# http://www.rocketaware.com/perl/perldelta/Deprecated_Inherited_C_AUTOLOAD.htm

# Gets called instead of all functions in Import'ed modules
# It assumes a normal function, not a class or instance method
sub AUTOLOAD
{
    our $AUTOLOAD;

    print "$AUTOLOAD (", join (", ", @_), ")\n" if YaST::YCP::debug;

    my @components = split ("::", $AUTOLOAD);
    my $func = pop (@components);
    return YaST::YCP::call_ycp (join ("::", @components), $func, @_);
}

=head2 Boolean

 $b = YaST::YCP::Boolean (1);
 $b->value (0);
 print $b->value, "\n";
 SCR::Write (.foo, $b);

=cut

package YaST::YCP::Boolean;
use strict;
use warnings;
use diagnostics;

# a Boolean is just a blessed reference to a scalar

sub new
{
    my $class = shift;
    my $val = shift;
    return bless \$val, $class
}

# get/set
sub value
{
    # see "Constructors and Instance Methods" in perltoot
    my $self = shift;
    if (@_) { $$self = shift; }
    return $$self;
}

1;
