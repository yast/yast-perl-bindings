#
# Example Perl module for YaST2 Perl bindings
#

package mod_ex;

use strict;
use Exporter;

use vars qw(@ISA @EXPORT $helloMsg);

@ISA	= qw(Exporter);
@EXPORT	= qw(helloWorld copyargs);

#-----------------------------------------------------------------------------

BEGIN
{
    print "Executing mod_ex module BEGIN block\n";
}

END
{
    print "Executing mod_ex module END block\n";
}

#-----------------------------------------------------------------------------

#
# Return message from module variable
#
sub helloWorld()
{
    return $helloMsg;
}

#-----------------------------------------------------------------------------

#
# Return passed arguments
#
sub copyargs()
{
    if ( wantarray )
    {
	return @_;
    }
    else
    {
	return shift;
    }
}

#-----------------------------------------------------------------------------

$helloMsg = "Hello, World!\n";


# Return value
1;
