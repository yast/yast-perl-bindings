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
	      Martin Vidner <mvidner@suse.cz>

/-*/

#define y2log_component "Y2Perl"
#include <ycp/y2log.h>
#include <ycp/pathsearch.h>

#include "Y2PerlComponent.h"
#include "YPerl.h"
#include "YPerlNamespace.h"
using std::string;


Y2PerlComponent::Y2PerlComponent()
{
    // Actual creation of a Perl interpreter is postponed until one of the
    // YPerl static methods is used. They handle that.

    y2milestone( "Creating Y2PerlComponent" );
}


Y2PerlComponent::~Y2PerlComponent()
{
    YPerl::destroy();
}


void Y2PerlComponent::result( const YCPValue & )
{
}


Y2Namespace *Y2PerlComponent::import (const char* name)
{
    // TODO where to look for it
    // must be the same in Y2CCPerl and Y2PerlComponent
    string module = YCPPathSearch::find (YCPPathSearch::Module, string (name) + ".pm");
    if (module.empty ())
    {
	y2internal ("Couldn't find %s after Y2CCPerl pointed to us", name);
	return NULL;
    }

    module.erase (module.size () - 3 /* strlen (".pm") */);
    YCPList args;
    args->add (YCPString(/*module*/ name));

    // load it
    YPerl::loadModule (args);

    // introspect, create data structures for the interpreter
    Y2Namespace *ns = new YPerlNamespace (name);

    return ns;
}
