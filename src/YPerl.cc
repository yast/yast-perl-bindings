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

  File:	      YPerl.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include <EXTERN.h>	// Perl stuff
#include <perl.h>

#define y2log_component "Y2Perl"
#include <ycp/y2log.h>

#include <ycp/YCPBoolean.h>
#include <ycp/YCPFloat.h>
#include <ycp/YCPError.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPList.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPString.h>
#include <ycp/YCPSymbol.h>
#include <ycp/YCPVoid.h>

#include <YPerl.h>

#define DIM(ARRAY)	( sizeof( ARRAY )/sizeof( ARRAY[0] ) )

// The weird Perl macros need a PerlInterpreter of name 'my_perl' (!) almost everywhere.
#define WEIRD_PERL_MACRO_DEFS PerlInterpreter * my_perl = YPerl::perlInterpreter()



// Stub for dynamic loading of Perl modules as xs_init function for perl_parse().
// The code for this is generated in Makefile.am into perlxsi.c
EXTERN_C void xs_init( pTHX );


YPerl * YPerl::_yPerl = 0;


YPerl::YPerl()
    : _perlInterpreter(0)
    , _haveParseTree( false )
{
    _perlInterpreter = perl_alloc();

    if ( _perlInterpreter )
	perl_construct( _perlInterpreter );
}

YPerl::~YPerl()
{
    if ( _perlInterpreter )
    {
	perl_destruct( _perlInterpreter );
	perl_free( _perlInterpreter );
    }
}


YPerl *
YPerl::yPerl()
{
    if ( ! _yPerl )
	_yPerl = new YPerl();

    return _yPerl;
}


YCPValue
YPerl::destroy()
{
    y2milestone( "Shutting down embedded Perl interpreter." );

    if ( _yPerl )
	delete _yPerl;

    _yPerl = 0;

    return YCPVoid();
}


PerlInterpreter *
YPerl::perlInterpreter()
{
    if ( YPerl::yPerl() )
	return YPerl::yPerl()->internalPerlInterpreter();

    return 0;
}


YCPValue
YPerl::parse( YCPList argList )
{
    PerlInterpreter * perl = YPerl::perlInterpreter();

    if ( ! perl )
	return YCPNull();

    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Parse(): Bad arguments: String expected!" );

    if ( yPerl()->haveParseTree() )
	y2warning( "Perl::Parse() multiply called - memory leak? Try Perl::Destroy() first!" );

    string script = argList->value(0)->asString()->value();
    const char *argv[] = { "", script.c_str() };
    int	argc = DIM( argv );

    if ( perl_parse( perl,
		     xs_init, // Init function from (generated) perlxsi.c
		     argc,
		     const_cast<char **> (argv),
		     0 )	// env
	 != 0 )
	return YCPError( "Perl::Parse(): Parse error." );

    yPerl()->setHaveParseTree( true );

    return YCPVoid();
}


YCPValue
YPerl::callVoid( YCPList argList )
{
    PerlInterpreter * my_perl = YPerl::perlInterpreter();

    if ( ! my_perl )
	return YCPNull();

    if ( argList->size() < 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Call(): Bad arguments: No function to execute!" );

    if ( ! yPerl()->haveParseTree() )
	return YCPError( "Do Perl::Parse() before Perl::Call() !" );

    string functionName = argList->value(0)->asString()->value();
    int flags = G_VOID;


    // Using the weird embedded-Perl macros as described in
    // man perlembed, man perlcall, man perlapi, man perlguts
    //
    // It's not pretty. Try to concentrate on the right side (the comments).

    dSP;		// Declare Perl stack pointer
    ENTER;		// Open a new Perl scope
    SAVETMPS;		// Save temporary variables
    PUSHMARK(SP);	// Save stack pointer

    // Put arguments on the stack

    for ( int i=1; i < argList->size(); i++ )
    {
	XPUSHs( sv_2mortal( newPerlScalar( argList->value(i) ) ) );
    }

    PUTBACK;		// Make local stack pointer global

    int ret_count = call_pv( functionName.c_str(), flags ); // Call the function

    SPAGAIN;		// Copy global stack pointer to local one


    // Pop result from the stack

    if ( ret_count != 1 )
    {
	// Perl always returns something (the last expression calculated),
	// so let's just make sure we don't get any more than one.

	y2warning( "Perl function %s returned %d arguments, expecting none",
		   functionName.c_str(), ret_count );
    }

    while ( ret_count-- > 0 )
	(void) POPs;

    PUTBACK;		// Make local stack pointer global
    FREETMPS;		// Free temporary variables
    LEAVE;		// Close the Perl scope

    return YCPVoid();
}


YCPValue
YPerl::eval( YCPList argList )
{
    PerlInterpreter * perl = YPerl::perlInterpreter();

    if ( ! perl )
	return YCPNull();

    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Eval(): Bad arguments: String expected!" );

    string expr = argList->value(0)->asString()->value();
    const char *argv[] = { "", "-w", "-e", expr.c_str() };
    int	argc = DIM( argv );

    perl_parse( perl,
		0,	// xs_init function
#if 0
		xs_init, // Init function from (generated) perlxsi.c
#endif
		argc,
		const_cast<char **> (argv),
		0 );	// env
    perl_run( perl );

    return YCPVoid();
}


SV *
YPerl::newPerlScalar( const YCPValue & val )
{
    WEIRD_PERL_MACRO_DEFS;

    if ( val->isString()  )	return newSVpv( val->asString()->value_cstr(), 0 );
    if ( val->isList()    )	return newPerlArrayRef( val->asList() );
    if ( val->isMap() )		return newPerlHashRef( val->asMap() );
    if ( val->isInteger() )	return newSViv( val->asInteger()->value() );
    if ( val->isBoolean() )	return newSViv( val->asBoolean()->value() ? 1 : 0 );
    if ( val->isFloat()   )	return newSVnv( val->asFloat()->value() );
    if ( val->isVoid()    )	return &PL_sv_undef;

    return 0;
}


SV *
YPerl::newPerlArrayRef( const YCPList & list )
{
    WEIRD_PERL_MACRO_DEFS;

    //
    // Create array
    //

    AV * array = newAV();

    if ( ! array )
	return 0;

    //
    // Fill array
    //

    for ( int i = 0; i < list->size(); i++ )
    {
	SV * scalarVal = newPerlScalar( list->value(i) );

	if ( scalarVal )
	{
	    av_push( array, scalarVal );

	    if ( SvREFCNT( scalarVal ) != 1 )
	    {
		y2error( "Internal error: Reference count is %d (should be 1)",
			 SvREFCNT( scalarVal ) );
	    }
	}
	else
	{
	    y2error( "Couldn't convert YCP list item '%s' to Perl array item",
		     list->value(i)->toString().c_str() );
	}
    }

    //
    // Return a reference to the new array
    //

    return newRV_noinc( (SV *) array );
}


SV *
YPerl::newPerlHashRef( const YCPMap & map )
{
    WEIRD_PERL_MACRO_DEFS;

    //
    // Create hash
    //

    HV * hash = newHV();

    if ( ! hash )
	return 0;

    //
    // Fill hash
    //

    for ( YCPMapIterator it = map->begin(); it != map->end(); ++it )
    {
	string keyStr;

	if      ( it.key()->isString() )	keyStr = it.key()->asString()->value();
	else if ( it.key()->isSymbol() )	keyStr = it.key()->asSymbol()->symbol();
	else if ( it.key()->isInteger() )	keyStr = it.key()->toString();

	if ( keyStr.empty() )
	{
	    y2error( "Couldn't convert YCP map key '%s' to Perl hash key",
		     it.key()->toString().c_str() );
	}
	else
	{
	    //
	    // Add one key / value pair
	    //

	    SV * scalarVal = newPerlScalar( it.value() );

	    if ( scalarVal )
	    {
		SV ** ret = hv_store( hash, keyStr.c_str(), keyStr.length(), scalarVal, 0 );

		if ( ret == 0 )
		{
		    y2error( "Couldn't insert Perl hash value '%s' => '%s'",
			     keyStr.c_str(), it.value()->toString().c_str() );

		    SvREFCNT_dec( scalarVal );	// Free scalar (avoid memory leak)
		}
		else if ( SvREFCNT( scalarVal ) != 1 )
		{
		    y2error( "Internal error: Reference count is %d (should be 1)",
			     SvREFCNT( scalarVal ) );
		}
	    }
	    else
	    {
		y2error( "Couldn't convert YCP map value '%s' to Perl hash value",
			 it.value()->toString().c_str() );
	    }
	}
    }

    //
    // Return a reference to the new hash
    //

    return newRV_noinc( (SV *) hash );
}
