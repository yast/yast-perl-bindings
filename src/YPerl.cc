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
#include <iosfwd>
#include <sstream>
#include <iomanip>

#include <EXTERN.h>	// Perl stuff
#include <perl.h>


#define y2log_component "Y2Perl"
#include <ycp/y2log.h>


#include <ycp/YCPBoolean.h>
#include <ycp/YCPFloat.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPList.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPPath.h>
#include <ycp/YCPString.h>
#include <ycp/YCPSymbol.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPVoid.h>

#include <YPerl.h>

#define DIM(ARRAY)	( sizeof( ARRAY )/sizeof( ARRAY[0] ) )



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

    const char *argv[] = { "yperl", "-e", "" };
    int	argc = DIM( argv );

    perl_parse( _perlInterpreter,
		xs_init, // Init function from (generated) perlxsi.c
		argc,
		const_cast<char **> (argv),
		0 );	// env
}

YPerl::YPerl(pTHX)
    : _perlInterpreter(aTHX)
    , _haveParseTree( false )
{
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

YPerl *
YPerl::yPerl(pTHX)
{
    if ( ! _yPerl )
	_yPerl = new YPerl(aTHX);
    else
	_yPerl->_perlInterpreter = aTHX;

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

/**
 * @builtin Perl::AAA_DISABLED () -> void
 * Note that with the migration to the new interpreter,
 * these builtins don't work! I will rework the docs.
 * -- mvidner
 */

/**
 * @builtin Perl::Parse (string file_name) -> void
 * Loads a Perl script.
 *
 * This executes Perl "use" instructions and BEGIN blocks,
 * but not the script itself.
 *
 * Internal structures must be cleaned up by Perl::Destroy
 * before it can be called again.
 */
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


/**
 * @builtin Perl::Run () -> void
 * Runs a script loaded by Perl::Parse or Perl::LoadModule.
 *
 * Can be called repeatedly.
 */
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


/**
 * @builtin Perl::Use        (string module) -> void
 * @builtin Perl::LoadModule (string module) -> void
 * Loads a module.
 */
YCPValue
YPerl::loadModule( YCPList argList )
{
    EMBEDDED_PERL_DEFS;

// Trying to avoid accessing freed filename when later sv_dumping.
// No success yet.
/*
    dSP;
    ENTER;
    SAVETMPS;

    PUSHMARK (SP);
    PUTBACK;
*/
    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::loadModule() / Perl::Use() : Bad arguments: String expected!" );

    string module_name = argList->value(0)->asString()->value();
    I32 flags	 = PERL_LOADMOD_NOIMPORT;	// load_module() flags are undocumented  :-(
    SV * version = 0;
    SV * name = newSVpv( module_name.c_str(), 0 );

    //sv_dump (name);
    // the name is unref'd by load_module, so it must not be mortal
    // on the contrary, we ref it so that the file name gets preserved for debugging
    // :-( does not work
    newRV (name);
    load_module( flags, name, version );
    //sv_dump (name);

    yPerl()->setHaveParseTree( true );

/*
    SPAGAIN;
    PUTBACK;

    FREETMPS;
    LEAVE;
*/
    return YCPVoid();
}

/**
 * @builtin Perl::CallVoid (string funcname, any param1, ...) -> void
 * Calls a perl function ...
 */
YCPValue
YPerl::callVoid( YCPList argList )
{
    return yPerl()->call( argList, Type::Void );
}


/**
 * @builtin Perl::CallString (string funcname, any param1, ...) -> string
 * Calls a perl function ...
 */
YCPValue
YPerl::callString( YCPList argList )
{
    return yPerl()->call( argList, Type::String );
}


/**
 * @builtin Perl::CallList (string funcname, any param1, ...) -> list
 * Calls a perl function ...
 */
YCPValue
YPerl::callList( YCPList argList )
{
    return yPerl()->call( argList, Type::List ());
}


// there are &nbsp;s after CallBool
/**
 * @builtin Perl::CallBoolean (string funcname, any param1, ...) -> boolean
 * @builtin Perl::CallBool    (string funcname, any param1, ...) -> boolean
 * Calls a perl function ...
 */
YCPValue
YPerl::callBool( YCPList argList )
{
    return yPerl()->call( argList, Type::Boolean );
}


/**
 * @builtin Perl::CallInteger (string funcname, any param1, ...) -> integer
 * @builtin Perl::CallInt     (string funcname, any param1, ...) -> integer
 * Calls a perl function ...
 */
YCPValue
YPerl::callInt( YCPList argList )
{
    return yPerl()->call( argList, Type::Integer );
}


YCPValue
YPerl::call( YCPList argList, constTypePtr wanted_result_type )
{
    EMBEDDED_PERL_DEFS;

    if ( argList->size() < 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Call(): Bad arguments: No function to execute!" );

    if ( ! yPerl()->haveParseTree() )
	return YCPError( "Perl::Call: Use Perl::Parse() or Perl::LoadModule() before Perl::Call() !" );

    string functionName = argList->value(0)->asString()->value();
    string originalName = functionName;
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

    if (wanted_result_type->isList ())		calling_context = G_ARRAY;
    else if (wanted_result_type->isVoid ())	calling_context = G_VOID;
    else					calling_context = G_SCALAR;

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
	XPUSHs( sv_2mortal( newPerlScalar( argList->value(i), false ) ) );
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

    if ( wanted_result_type->isList () )
    {
	constTypePtr value_type = ((constListTypePtr)(wanted_result_type))->type ();
	std::list<SV *> results;

	// We want a list, but Perl uses a stack, so invert order of return values.

	while ( ret_count-- > 0 )
	    results.push_front( POPs );

	YCPList result_list;

	for ( std::list<SV *>::iterator it = results.begin(); it != results.end(); ++it )
	    result_list->add (fromPerlScalar (*it, value_type));

	result = result_list;
    }
    else
    {
	if (wanted_result_type->isVoid ())
	{
	    // Perl always returns something - the last expression calculated.
	    // Ignore this if return type void is desired.

	    result = YCPVoid();
	}
	else
	{
	    result = fromPerlScalar (POPs, wanted_result_type);
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

    // fromPerlScalar can return Null to indicate that the type check failed.
    // It is used here and in passing parameters,
    // so specify the location better
    if (result.isNull ())
    {
	y2error ("... when returning from %s", originalName.c_str ());
	result = YCPVoid ();
    }

    return result;
}


/**
 * @builtin Perl::Eval (string perl_code) -> any
 * Evaluates Perl code and returns the result.
 */
YCPValue
YPerl::eval( YCPList argList )
{
    EMBEDDED_PERL_DEFS;

    if ( argList->size() != 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Eval(): Bad arguments: String expected!" );

    SV * result = eval_pv( argList->value(0)->asString()->value_cstr(), 1 );

    if ( ! result )
	return YCPVoid();

    return yPerl()->fromPerlScalarToAny( result );
}


SV *
YPerl::newPerlScalar( const YCPValue & val, bool composite )
{
    EMBEDDED_PERL_DEFS;

    if ( val->isString()  )	return newSVpv( val->asString()->value_cstr(), 0 );
    if ( val->isList()    )	return newPerlArrayRef( val->asList() );
    if ( val->isMap()     )	return newPerlHashRef( val->asMap() );
    if ( val->isInteger() )	return newSViv( val->asInteger()->value() );
    if ( val->isBoolean() )	return newSViv( val->asBoolean()->value() ? 1 : 0 );
    if ( val->isFloat()   )	return newSVnv( val->asFloat()->value() );
    if ( val->isVoid()    )	return composite? newSV (0): &PL_sv_undef;

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
	SV * scalarVal = newPerlScalar( yList->value(i), true );

	if ( scalarVal )
	{
	    av_push( array, scalarVal );

	    if ( SvREFCNT( scalarVal ) != 1 )
	    {
		// U32 is unsigned long, even on a Hammer
		y2error( "Internal error: Reference count is %" IVdf " (should be 1)",
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

	    SV * scalarVal = newPerlScalar( it.value(), true );

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
		    y2error( "Internal error: Reference count is %" IVdf " (should be 1)",
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

static
string
debugDump (SV *sv)
{
    static char *svtypes[] = {
	"NULL",
	"IV",
	"NV",
	"RV",
	"PV",
	"PVIV",
	"PVNV",
	"PVMG",
	"PVBM",
	"PVLV",
	"PVAV",
	"PVHV",
	"PVCV",
	"PVGV",
	"PVFM",
	"PVIO",
    };
    U32 f = SvFLAGS (sv);
    std::ostringstream ss;

    ss << "SV with TYPE: "
       << svtypes[SvTYPE (sv) & 15] // just to be sure if Perl changes weirdly

       << ", FLAGS:"
       << (f & 0x00000100 ? " PADBUSY": "")/* reserved for tmp or my already */
       << (f & 0x00000200 ? " PADTMP": "") /* in use as tmp */
       << (f & 0x00000400 ? " PADMY": "")  /* in use a "my" variable */
       << (f & 0x00000800 ? " TEMP": "")   /* string is stealable? */
       << (f & 0x00001000 ? " OBJECT": "") /* is "blessed" */
       << (f & 0x00002000 ? " GMG": "")    /* has magical get method */
       << (f & 0x00004000 ? " SMG": "")    /* has magical set method */
       << (f & 0x00008000 ? " RMG": "")    /* has random magical methods */

       << (f & 0x00010000 ? " IOK": "")    /* has valid public integer value */
       << (f & 0x00020000 ? " NOK": "")    /* has valid public numeric value */
       << (f & 0x00040000 ? " POK": "")    /* has valid public pointer value */
       << (f & 0x00080000 ? " ROK": "")    /* has a valid reference pointer */

       << (f & 0x00100000 ? " FAKE": "")   /* glob or lexical is just a copy */
       << (f & 0x00200000 ? " OOK": "")    /* has valid offset value */
       << (f & 0x00400000 ? " BREAK": "")  /* refcnt is artificially low - used
					    * by SV's in final arena  cleanup */
       << (f & 0x00800000 ? " READONLY": "") /* may not be modified */

       << (f & 0x01000000 ? " pIOK": "")   /* has valid non-public integer value */
       << (f & 0x02000000 ? " pNOK": "")   /* has valid non-public numeric value */
       << (f & 0x04000000 ? " pPOK": "")   /* has valid non-public pointer value */
       << (f & 0x08000000 ? " pSCREAM": "")/* has been studied? */
       << (f & 0x20000000 ? " UTF8": "")   /* SvPV is UTF-8 encoded */
       << (f & 0x10000000 ? " AMAGIC": "") /* has magical overloaded methods */
	;

    return ss.str ();
}

bool
YPerl::tryFromPerlClassBoolean (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::Boolean"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::Boolean::value");
	out = YCPBoolean (SvTRUE (sval));
	SvREFCNT_dec (sval);
	return true;
    }
    else
    {
	return false;
    }
}

bool
YPerl::tryFromPerlClassSymbol (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    bool ret;
    if (!strcmp (class_name, "YaST::YCP::Symbol"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::Symbol::value");
	if (SvPOK (sval))
	{
	    out = YCPSymbol (SvPV_nolen (sval));
	    ret = true;
	}
	else
	{
	    y2internal ("YaST::YCP::Symbol::value did not return a string");
	    ret = false;
	}
	SvREFCNT_dec (sval);
    }
    else
    {
	ret = false;
    }
    return ret;
}

bool
YPerl::tryFromPerlClassTerm (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::Term"))
    {
	SV *s_name = callMethod (sv, "YaST::YCP::Term::name");
	YCPValue name = fromPerlScalar (s_name, Type::String); // optimize
	SvREFCNT_dec (s_name);
	if (name.isNull () || !name->isString ())
	{
	    y2internal ("YaST::YCP::Term::name did not return a string");
	    return false;
	}
	SV *s_args = callMethod (sv, "YaST::YCP::Term::args");
	YCPValue args = fromPerlScalar (s_args, new ListType (Type::Any)); // optimize
	SvREFCNT_dec (s_args);
	if (args.isNull () || !args->isList ())
	{
	    y2internal ("YaST::YCP::Term::args did not return a list");
	    return false;
	}
	out = YCPTerm (name->asString ()->value (),
		       args->asList ());
	return true;
    }
    else
    {
	return false;
    }
}

/**
 * If type did not match, returns YCPNull
 * (which must then be converted to YCPVoid)
 *
 */
YCPValue
YPerl::fromPerlScalar( SV * sv, constTypePtr wanted_type )
{
    EMBEDDED_PERL_DEFS;

    YCPValue val = YCPNull ();

    // Decide by the wanted type,
    // Except first check if we got undef and in that case return nil
    if (!SvOK (sv))
    {
	val = YCPVoid ();
    }
    else if (wanted_type->isAny ())
    {
	val = fromPerlScalarToAny (sv);
    }
    else if (wanted_type->isBoolean ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    if (!tryFromPerlClassBoolean (class_name, sv, val))
	    {
	    		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
	    }
	}
	else
	{
	    val = YCPBoolean (SvTRUE (sv));
	}
    }
    else if (wanted_type->isString ())
    {
	// Perl relies on automatic coercion between strings and numbers
	//
	// So to behave more like it, instead of "if (SvXOK (sv)) SvXV (sv)"
	// we first SvXV (sv) and only then SvXOK.
	const char *pv = SvPV_nolen (sv);
	if (SvPOK (sv))	val = YCPString (pv);
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
    else if (wanted_type->isInteger ())
    {
	// see isString
	IV iv = SvIV (sv);
	if (SvIOK (sv))	val = YCPInteger (iv);
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
    else if (wanted_type->isFloat ())
    {
	// see isString
	NV nv = SvNV (sv);
	if (SvNOK (sv))	val = YCPFloat (nv);
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
    else if (wanted_type->isSymbol ())
    {
	if (SvPOK (sv))	val = YCPSymbol (SvPV_nolen (sv));
	else if (!sv_isobject (sv) ||
		 !tryFromPerlClassTerm (HvNAME (SvSTASH (SvRV (sv))), sv,
					val))
	{
	    		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
	}
    }
/*
  function - probably not. or its name?
 */
/*
term - pass as a list ["asymbol", arg1, arg2...], like typeinfo
no, not needed yet
 */
    else if (wanted_type->isTerm ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    if (!tryFromPerlClassTerm (class_name, sv, val))
	    {
	    		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
	    }
	}
//	else if //... try from a list
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
    else if (wanted_type->isPath ())
    {
	// a string
	if (SvPOK (sv))	val = YCPPath (SvPV_nolen (sv));
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
	// maybe allow list later?
    }
    else if (wanted_type->isList ())
    {
	if (SvROK (sv))
	{
	    SV *ref = SvRV (sv);
	    if (SvTYPE (ref) == SVt_PVAV)
	    {
		constListTypePtr list_type = (constListTypePtr) wanted_type;
		val = fromPerlArray ((AV *) ref, list_type->type ());
	    }
	    else	y2error ("Expected %s, got reference to %s",
				 wanted_type->toString ().c_str (),
				 debugDump(ref).c_str ());

	}
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
    else if (wanted_type->isMap ())
    {
	if (SvROK (sv))
	{
	    SV *ref = SvRV (sv);
	    if (SvTYPE (ref) == SVt_PVHV)
	    {
		constMapTypePtr map_type = (constMapTypePtr) wanted_type;
		val = fromPerlHash ((HV *) ref,
				    map_type->keytype (),
				    map_type->valuetype ());
	    }
	    else	y2error ("Expected %s, got reference to %s",
				 wanted_type->toString ().c_str (),
				 debugDump(ref).c_str ());

	}
	else		y2error ("Expected %s, got %s",
				 wanted_type->toString ().c_str (),
				 debugDump(sv).c_str ());
    }
/*
  tuple?
 */
    else
    {
	y2internal ("Unhandled conversion to %s from %s",
		    wanted_type->toString ().c_str (), debugDump(sv).c_str ());
    }


    return val;
}

// call a pethod of a perl object
// that takes no arguments and returns one scalar
SV*
YPerl::callMethod (SV * instance, const char * full_method_name)
{
    EMBEDDED_PERL_DEFS;

    SV *ret = &PL_sv_undef;

    dSP;
    ENTER;
    SAVETMPS;

    PUSHMARK (SP);
    XPUSHs (instance);
    PUTBACK;

    int count = call_method (full_method_name, G_SCALAR);

    SPAGAIN;
    if (count != 1)
    {
	// must be 0 because we specified G_SCALAR
	y2error ("Method %s did not return a value", full_method_name);
    }
    else
    {
	ret = POPs;
    }
    PUTBACK;

    // FREETMPS frees also the return value,
    // so we must ref it here and our _caller_ must unref it
    SvREFCNT_inc (ret);
    FREETMPS;
    LEAVE;

    return ret;
}

/**
 * This is copied from the original sh's function.
 * It converts according to what Perl provides, not what YCP wants.
 */
YCPValue
YPerl::fromPerlScalarToAny (SV * sv)
{
    EMBEDDED_PERL_DEFS;

    YCPValue val = YCPNull ();

    // Try strings only after numbers
    // because numbers will get an additional string nature
    // after being prited out.
    // This is not foolproof vice versa but that should be really less common.
    if      ( SvIOK( sv ) )		val = YCPInteger( SvIV( sv ) );
    else if ( SvNOK( sv ) )		val = YCPFloat  ( SvNV( sv ) );
    else if ( SvPOK( sv ) )		val = YCPString ( SvPV_nolen( sv ) );
    else if (sv_isobject (sv))
    {
	char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	if (!tryFromPerlClassBoolean	(class_name, sv, val) &&
	    !tryFromPerlClassSymbol	(class_name, sv, val) &&
	    !tryFromPerlClassTerm	(class_name, sv, val))
	{
	    y2error ("Expected any, got object of class %s",
		     class_name);
	}
    }
    else if ( SvROK( sv ) )
    {
	SV * ref = SvRV( sv );

	switch ( SvTYPE( ref ) )
	{
	    case SVt_PVAV:	// Reference to an array
		val = fromPerlArray ((AV *) ref, Type::Any);
		break;

	    case SVt_PVHV:	// Reference to a hash
		val = fromPerlHash ((HV *) ref, Type::Any, Type::Any);
		break;

	    default:
		y2error ("Expected any, got reference to %s",
				 debugDump(ref).c_str ());
		break;
	}
    }

    return val;
}

YCPList
YPerl::fromPerlArray (AV * array, constTypePtr wanted_type)
{
    EMBEDDED_PERL_DEFS;

    YCPList yList;

    I32 last = av_len (array);
    for (I32 i = 0; i <= last; ++i)
    {
	// Error propagation:
	// May be better to continue converting even if an error occurred
	// so that code will run longer...
	// Then we would have to pass also an error code to indicate a failure
	// After all, does not seem as such a good idea
	// => abort at first failure
	SV ** svp = av_fetch (array, i, 0 /* not lval */);
	if (svp == NULL)
	{
	    y2internal ("av_fetch returned NULL for index %" IVdf, i);
	    return YCPNull ();
	}
	YCPValue v = fromPerlScalar (*svp, wanted_type);
	if (v.isNull ())
	{
	    y2error ("... when converting to a list");
	    return YCPNull ();
	}
	yList->add( v );
    }

    return yList;
}


YCPMap
YPerl::fromPerlHash (HV * hv, constTypePtr key_type, constTypePtr value_type)
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
	{
	    // The map may want a symbol or integer as key
	    // so massage the key through fromPerlScalar too
	    SV *key_sv = newSVpv (key, key_len);
	    YCPValue ykey = fromPerlScalar (key_sv, key_type);
	    SvREFCNT_dec (key_sv);
	    if (ykey.isNull ())
	    {
		y2error ("... when converting to a map key");
		return YCPNull ();
	    }

	    YCPValue yval = fromPerlScalar (sv, value_type);
	    if (yval.isNull ())
	    {
		y2error ("... when converting to a map value");
		return YCPNull ();
	    }
	    map->add (ykey, yval);
	}
    }

    return map;
}
