/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						     (C) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:	      Y2PerlComponent.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

#define y2log_component "Y2Perl"
#include <ycp/y2log.h>
#include "Y2PerlComponent.h"
#include "YPerl.h"


Y2PerlComponent::Y2PerlComponent()
    , _interpreter(0)
{
}


Y2PerlComponent::~Y2PerlComponent()
{
    if ( _interpreter )
	delete _interpreter;
}


YCPValue Y2PerlComponent::evaluate( const YCPValue & command )
{
    if ( ! _interpreter )
    {
	_interpreter = new YPerlInterpreter();

	if ( ! _interpreter )
	    return YCPNull();
    }

    YCPValue val = _interpreter->evaluate( command );

    return val;
}


void Y2PerlComponent::result( const YCPValue & )
{
    if ( _interpreter )
	delete _interpreter;

    _interpreter = 0;
}



