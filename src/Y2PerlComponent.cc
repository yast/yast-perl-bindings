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


// The old way of calling builtins has to be redone
// TODO later
/*
YCPValue
Y2PerlComponent::evaluate( const YCPValue & val )
{
    if ( ! val->isTerm() )
	return YCPError( "Syntax error: Term expected" );

    YCPTerm term    = val->asTerm();
    string function = term->symbol()->symbol();
    YCPList argList = term->args();


    y2debug("YPerl::evaluate( %s, %s )", function.c_str(), argList->toString().c_str());

    // Optimization: Commands are ordered by how frequently they are called.
    
    if ( function == "CallVoid" 	)	return YPerl::callVoid	( argList );
    if ( function == "CallList" 	)	return YPerl::callList	( argList );
    if ( function == "CallString" 	)	return YPerl::callString( argList );
    if ( function == "CallBool" 	)	return YPerl::callBool	( argList );
    if ( function == "CallBoolean"	)	return YPerl::callBool	( argList );
    if ( function == "CallInt" 		)	return YPerl::callInt	( argList );
    if ( function == "CallInteger" 	)	return YPerl::callInt	( argList );
    if ( function == "Eval" 		)	return YPerl::eval	( argList );
    if ( function == "Parse" 		)	return YPerl::parse	( argList );
    if ( function == "Run" 		)	return YPerl::run	( argList );
    if ( function == "LoadModule"	)	return YPerl::loadModule( argList );
    if ( function == "Use"		)	return YPerl::loadModule( argList );
    if ( function == "Destroy" 		)	return YPerl::destroy();
	
    return YCPError( string ( "Undefined Perl::" ) + function );
}
*/

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
