#
# Example dummy Perl module for YaST2 Perl bindings
#

package dummy;

use strict;
use Exporter;

use vars qw(@ISA @EXPORT);

@ISA	= qw(Exporter);
@EXPORT	= qw(dummy);

#-----------------------------------------------------------------------------

BEGIN
{
    print "Executing dummy module BEGIN block\n";
}

END
{
    print "Executing dummy module END block\n";
}

#-----------------------------------------------------------------------------

#
# Return message from module variable
#
sub dummy()
{
    print "dummy\n";
}



# Return value
1;
