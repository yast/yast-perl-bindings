#! /usr/bin/perl -w
# This is a first try at a transparent interface layer between Perl and YCP
# Martin Vidner
# $Id$

# try noto to get into infinite recursion
package YCP_Autoload;

# cannot rely on UNIVERSAL::AUTOLOAD getting automatically called
# http://www.rocketaware.com/perl/perldelta/Deprecated_Inherited_C_AUTOLOAD.htm

# gets called instead of all functions in Import'ed modules
sub AUTOLOAD
{
    # Here we would call the YCP interface
    # For now, only show that we understand the request
    my $instance = $_[0];
    my $class = ref ($instance);
    if ($imported{$class})
    {
	shift;
#	print "Calling instance";
    }
    else
    {
#	print "Calling function";
    }
#    print " $AUTOLOAD (", join (", ", @_), ")\n";

    print "AUTOLOAD for $AUTOLOAD\n";

    my @components = split ("::", $AUTOLOAD);
    my $func = pop (@components);
    return YaST::YCP::call_ycp (join ("::", @components), $func, @_);
}

package YaST::YCP;

@ISA = qw(DynaLoader);

#use strict;
# may be useful
my %imported;

## calls boot_YCP
require XSLoader;
XSLoader::load ('YaST::YCP');
## We use DynaLoader directly so that we can specify the library search path

# gets executed on "use"
sub unused_import
{
    use DynaLoader;

    # search paths, in increasing importance:
    unshift @DynaLoader::dl_library_path, "/home/mvidner/2/tmp/headprefix/lib/YaST2/plugin";
    unshift @DynaLoader::dl_library_path, "/usr/lib/YaST2/plugin";
    unshift @DynaLoader::dl_library_path, @_;
    bootstrap YCP;
}

=head2 Import

YCP::Import "Namespace";

=cut

sub Import
{
    my $package = shift;
    warn "$package";
    $imported{$package} = 1;

    {
#	no strict refs;
	# let it get our autoload
	*{"${package}::AUTOLOAD"} = \&YCP_Autoload::AUTOLOAD;
    }
}

1;
