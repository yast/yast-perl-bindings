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
    print "Executing mod_ex module BEGIN block, sleep 5\n";
    sleep 5;
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

sub getInteger()
{
    return 43;
}

sub getFloat()
{
    return 3.14;
}

sub getBoolean()
{
    return 0;
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
