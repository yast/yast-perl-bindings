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


#include <list>

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

// The weird Perl macros need a PerlInterpreter * named 'my_perl' (!) almost everywhere.
#define EMBEDDED_PERL_DEFS PerlInterpreter * my_perl = YPerl::perlInterpreter()



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

    // Preliminary call perl_parse so the Perl interpreter is ready right away.
    // This does _not_ affect _haveParseTree: This is intended for real code trees.

    const char *argv[] = { "", "-e", "" };
    int	argc = DIM( argv );

    perl_parse( _perlInterpreter,
		xs_init, // Init function from (generated) perlxsi.c
		argc,
		const_cast<char **> (argv),
		0 );	// env
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
	y2warning( "Perl::Parse() multiply called - memory leak? "
		   "Try Perl::Destroy() first!" );

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
YPerl::run( YCPList argList )
{
    if ( argList->size() != 0 )
	return YCPError( "Perl::Run(): No arguments expected" );

    if ( ! yPerl()->haveParseTree() )
	return YCPError( "Perl::Run(): Use Perl::Parse() or Perl::LoadModule() before Perl::Run() !" );

    perl_run( yPerl()->perlInterpreter() );
    return YCPVoid();
}


YCPValue
YPerl::loadModule( YCPList argList )
{
    EMBEDDED_PERL_DEFS;

    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::loadModule() / Perl::Use() : Bad arguments: String expected!" );

    string module_name = argList->value(0)->asString()->value();
    I32 flags	 = PERL_LOADMOD_NOIMPORT;	// load_module() flags are undocumented  :-(
    SV * version = 0;

    load_module( flags, sv_2mortal( newSVpv( module_name.c_str(), 0 ) ), version );
    yPerl()->setHaveParseTree( true );

    return YCPVoid();
}


YCPValue
YPerl::callVoid( YCPList argList )
{
    return yPerl()->call( argList, YT_VOID );
}


YCPValue
YPerl::callString( YCPList argList )
{
    return yPerl()->call( argList, YT_STRING );
}


YCPValue
YPerl::callList( YCPList argList )
{
    return yPerl()->call( argList, YT_LIST );
}


YCPValue
YPerl::callBool( YCPList argList )
{
    return yPerl()->call( argList, YT_BOOLEAN );
}


YCPValue
YPerl::callInt( YCPList argList )
{
    return yPerl()->call( argList, YT_INTEGER );
}


YCPValue
YPerl::call( YCPList argList, YCPValueType wanted_result_type )
{
    EMBEDDED_PERL_DEFS;

    if ( argList->size() < 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Call(): Bad arguments: No function to execute!" );

    if ( ! yPerl()->haveParseTree() )
	return YCPError( "Perl::Call: Use Perl::Parse() or Perl::LoadModule() before Perl::Call() !" );

    string functionName = argList->value(0)->asString()->value();
    string::size_type arrowPos = functionName.find( "->" );
    string className;

    if ( arrowPos != string::npos )	// "->" in function name?
    {
	className = functionName.substr( 0, arrowPos );	// extract class name
	functionName.erase( 0, arrowPos+2 );		// remove  class name and "->"
    }

    
    //
    // Determine Perl calling context
    //

    int calling_context;

    switch ( wanted_result_type )
    {
	case YT_LIST:	calling_context = G_ARRAY;	break;
	case YT_VOID:	calling_context = G_VOID;	break;
	default:	calling_context = G_SCALAR;	break;

    }

    // Using the weird embedded-Perl macros as described in
    // man perlembed, man perlcall, man perlapi, man perlguts
    //
    // It's not pretty. Try to concentrate on the right side (the comments).

    dSP;		// Declare Perl stack pointer
    ENTER;		// Open a new Perl scope
    SAVETMPS;		// Save temporary variables
    PUSHMARK(SP);	// Save stack pointer

    
    // For class method calls put the class name on the stack first
    
    if ( ! className.empty() )
    {
	XPUSHs( sv_2mortal( newSVpv( className.c_str(), 0 ) ) );
    }
    

    // Put arguments on the stack

    for ( int i=1; i < argList->size(); i++ )
    {
	XPUSHs( sv_2mortal( newPerlScalar( argList->value(i) ) ) );
    }

    PUTBACK;		// Make local stack pointer global

    
    //
    // Call the function
    //
    
    int ret_count = 0;
    
    if ( className.empty() )
	ret_count = call_pv( functionName.c_str(), calling_context );
    else
	ret_count = call_method( functionName.c_str(), calling_context );
    

    //
    // Pop result from the stack
    //

    SPAGAIN;		// Copy global stack pointer to local one
    YCPValue result = YCPVoid();

    if ( wanted_result_type == YT_LIST )
    {
	std::list<SV *> results;

	// We want a list, but Perl uses a stack, so invert order of return values.

	while ( ret_count-- > 0 )
	    results.push_front( POPs );

	YCPList result_list;

	for ( std::list<SV *>::iterator it = results.begin(); it != results.end(); ++it )
	    result_list->add( fromPerlScalar( *it ) );

	result = result_list;
    }
    else
    {
	if ( wanted_result_type == YT_VOID )
	{
	    // Perl always returns something - the last expression calculated.
	    // Ignore this if return type void is desired.

	    result = YCPVoid();
	}
	else
	{
	    result = fromPerlScalar( POPs );
	}

	if ( ret_count > 1 )
	{
	    // Check for excess return values.

	    y2warning( "Perl function %s returned %d arguments, expecting just one",
		       functionName.c_str(), ret_count );

	    // Get rid of excess return values.

	    while ( --ret_count > 0 )
		(void) POPs;
	}
    }

    PUTBACK;		// Make local stack pointer global
    FREETMPS;		// Free temporary variables
    LEAVE;		// Close the Perl scope

    return result;
}


YCPValue
YPerl::eval( YCPList argList )
{
    EMBEDDED_PERL_DEFS;

    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Eval(): Bad arguments: String expected!" );

    SV * result = eval_pv( argList->value(0)->asString()->value_cstr(), 1 );

    if ( ! result )
	return YCPVoid();

    return yPerl()->fromPerlScalar( result );
}


SV *
YPerl::newPerlScalar( const YCPValue & val )
{
    EMBEDDED_PERL_DEFS;

    if ( val->isString()  )	return newSVpv( val->asString()->value_cstr(), 0 );
    if ( val->isList()    )	return newPerlArrayRef( val->asList() );
    if ( val->isMap()     )	return newPerlHashRef( val->asMap() );
    if ( val->isInteger() )	return newSViv( val->asInteger()->value() );
    if ( val->isBoolean() )	return newSViv( val->asBoolean()->value() ? 1 : 0 );
    if ( val->isFloat()   )	return newSVnv( val->asFloat()->value() );
    if ( val->isVoid()    )	return &PL_sv_undef;

    return 0;
}


SV *
YPerl::newPerlArrayRef( const YCPList & yList )
{
    EMBEDDED_PERL_DEFS;

    //
    // Create array
    //

    AV * array = newAV();

    if ( ! array )
	return 0;

    //
    // Fill array
    //

    for ( int i = 0; i < yList->size(); i++ )
    {
	SV * scalarVal = newPerlScalar( yList->value(i) );

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
		     yList->value(i)->toString().c_str() );
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
    EMBEDDED_PERL_DEFS;

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


YCPValue
YPerl::fromPerlScalar( SV * sv, YCPValueType wanted_type )
{
    EMBEDDED_PERL_DEFS;
    STRLEN len; // Caution: The SvPV macro uses the address (!) of this!

    YCPValue val = YCPVoid();

    if      ( SvPOK( sv ) )		val = YCPString ( SvPV( sv, len ) );
    else if ( SvNOK( sv ) )		val = YCPFloat  ( SvNV( sv ) );
    else if ( SvIOK( sv ) )		val = YCPInteger( SvIV( sv ) );
    else if ( SvROK( sv ) )
    {
	SV * ref = SvRV( sv );

	switch ( SvTYPE( ref ) )
	{
	    case SVt_IV:
	    case SVt_NV:
	    case SVt_PV:
	    case SVt_RV:
		val = fromPerlScalar( ref, wanted_type );
		break;

	    case SVt_PVAV:	// Reference to an array
		val = fromPerlArray( (AV *) ref );
		break;

	    case SVt_PVHV:	// Reference to a hash
		val = fromPerlHash( (HV *) ref );
		break;

	    default:
		y2error( "Reference to unknown type #%d", SvTYPE( ref ) );
		break;
	}
    }

    if ( wanted_type == YT_BOOLEAN )	val = YCPBoolean( SvOK( sv ) );


    if ( wanted_type != YT_UNDEFINED )
    {
	if ( wanted_type != val->valuetype() )
	{
	    y2error( "Perl returned type #%d, expected type #%d",
		     val->valuetype(), wanted_type );
	    val = YCPVoid();
	}
    }

    return val;
}


YCPList
YPerl::fromPerlArray( AV * array )
{
    EMBEDDED_PERL_DEFS;

    YCPList yList;
    SV * sv;

    while ( ( sv = av_shift( array ) ) != &PL_sv_undef )
    {
	yList->add( fromPerlScalar( sv ) );
    }

    return yList;
}


YCPMap
YPerl::fromPerlHash( HV * hv )
{
    EMBEDDED_PERL_DEFS;

    YCPMap map;
    I32 count = hv_iterinit( hv );

    for ( int i=0; i < count; i++ )
    {
	char * key;
	I32 key_len;

	SV * sv = hv_iternextsv( hv, &key, &key_len );

	if ( sv && key )
	    map->add( YCPString( key ), fromPerlScalar( sv ) );
    }

    return map;
}




