#! /usr/bin/perl -w
# Martin Vidner
# $Id$

=head1 NAME

YaST::YCP - a binary interface between Perl and YCP

=head1 SYNOPSIS

 use YaST::YCP qw(:DATA);

 YaST::YCP::Import ("SCR");
 my $m = SCR::Read (".sysconfig.displaymanager.DISPLAYMANAGER");
 SCR::Write (".sysconfig.kernel.CRASH_OFTEN", Boolean (1));

=head1 YaST::YCP

The DATA tag (in C<use YaST::YCP qw(:DATA)>) imports the data
constructor functions such as Boolean, Symbol or Term.

=cut

package YaST::YCP;
use strict;
use warnings;
use diagnostics;

require Exporter;
our @ISA = qw(Exporter);
my @e_data = qw(Boolean Integer Float String Symbol Term);
our @EXPORT_OK = @e_data;
our %EXPORT_TAGS = ( DATA => [@e_data] );

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

sub Integer ($)
{
    return new YaST::YCP::Integer (@_);
}

sub Float ($)
{
    return new YaST::YCP::Float (@_);
}

sub String ($)
{
    return new YaST::YCP::String (@_);
}

sub Symbol ($)
{
    return new YaST::YCP::Symbol (@_);
}

sub Term ($@)
{
    return new YaST::YCP::Term (@_);
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

=head2 Integer

 An explicitly typed integer, useful to put in heterogenous data structures.

 use YaST::YCP qw(:DATA);

 $i = Integer ("42 and more");
 $i->value ("43, actually");
 print $i->value, "\n";
 return [ $i ];

=cut

package YaST::YCP::Integer;
use strict;
use warnings;
use diagnostics;


# an Integer is just a blessed reference to a scalar
# just like Boolean, so use it!

our @ISA = qw (YaST::YCP::Boolean);

=head2 Float

 An explicitly typed float, useful to put in heterogenous data structures.

 use YaST::YCP qw(:DATA);

 $f = Float ("3.41 is PI");
 $f->value ("3.14 is PI");
 print $f->value, "\n";
 return [ $f ];

=cut

package YaST::YCP::Float;
use strict;
use warnings;
use diagnostics;


# a Float is just a blessed reference to a scalar
# just like Boolean, so use it!

our @ISA = qw (YaST::YCP::Boolean);

=head2 String

 An explicitly typed string, useful to put in heterogenous data structures.

 use YaST::YCP qw(:DATA);

 $s = String (42);
 $s->value (1 + 1);
 print $s->value, "\n";
 return [ $s ];

=cut

package YaST::YCP::String;
use strict;
use warnings;
use diagnostics;

# a String is just a blessed reference to a scalar
# just like Boolean, so use it!

our @ISA = qw (YaST::YCP::Boolean);

=head2 Symbol

 use YaST::YCP qw(:DATA);

 $s = Symbol ("next");
 $s->value ("back");
 print $s->value, "\n";
 return Term ("id", $s);

=cut

package YaST::YCP::Symbol;
use strict;
use warnings;
use diagnostics;


# a Symbol is just a blessed reference to a scalar
# just like Boolean, so use it!

our @ISA = qw (YaST::YCP::Boolean);

=head2 Term

 $t = YaST::YCP::Term ("CzechBox", "Accept spam", YaST::YCP::Boolean (0));
 $t->name ("CheckBox");
 print $t->args[0], "\n";
 UI::OpenDialog ($t);

=cut

package YaST::YCP::Term;
use strict;
use warnings;
use diagnostics;

# a Term has a name and arguments

sub new
{
    my $class = shift;
    my $name = shift;
    my $args = [ @_ ];
    return bless { name => $name, args => $args }, $class
}

# get/set
sub name
{
    # see "Constructors and Instance Methods" in perltoot
    my $self = shift;
    if (@_) { $self->{name} = shift; }
    return $self->{name};
}

# get/set
sub args
{
    # see "Constructors and Instance Methods" in perltoot
    my $self = shift;
    if (@_) { @{ $self->{args} } = @_; }
    # HACK:
    # because I don't want to process multiple return values,
    # I return it as a reference
    return $self->{args};
}

1;
