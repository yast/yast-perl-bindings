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

  File:	      Y2CCPerl.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include "Y2CCPerl.h"
#include <ycp/pathsearch.h>
#define y2log_component "Y2Perl"
#include <ycp/y2log.h>

// This is very important: We create one global variable of
// Y2CCPerl. Its constructor will register it automatically to
// the Y2ComponentBroker, so that will be able to find it.
// This all happens before main() is called!

Y2CCPerl g_y2ccperl;

Y2Component *Y2CCPerl::provideNamespace (const char *name)
{
    y2debug ("Y2CCPerl::provideNamespace %s", name);
    if (strcmp (name, "Perl") == 0)
    {
	// low level functions

	// leave implementation to later
	return 0;
    }
    else
    {
	// is there a perl module?
	// must be the same in Y2CCPerl and Y2PerlComponent
	string module = YCPPathSearch::find (YCPPathSearch::Module, string (name) + ".pm");
	if (!module.empty ())
	{
	    if (!cperl)
	    {
		cperl = new Y2PerlComponent ();
	    }
	    return cperl;
	}

	// let someone else try creating the namespace
	return 0;
    }
}
