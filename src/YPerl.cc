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

  Maintainer: Martin Vidner <mvidner@suse.cz>
  Author:     Stefan Hundhammer <sh@suse.de>

  This is the common part, defining data type conversions.

/-*/


#include <stdlib.h>
#include <list>
#include <iosfwd>
#include <sstream>
#include <iomanip>

// Perl stuff
#define PERL_NO_GET_CONTEXT     /* we want efficiency, man perlguts */
#include <EXTERN.h>
#include <perl.h>


#define y2log_component "Y2Perl"
#include <ycp/y2log.h>
#include <ycp/pathsearch.h>


#include <ycp/YCPBoolean.h>
#include <ycp/YCPByteblock.h>
#include <ycp/YCPFloat.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPList.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPPath.h>
#include <ycp/YCPString.h>
#include <ycp/YCPSymbol.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPVoid.h>
#include <ycp/YCPCode.h>
#include <ycp/YCPExternal.h>

#include <YPerl.h>

#define DIM(ARRAY)	( sizeof( ARRAY )/sizeof( ARRAY[0] ) )

#define YCP_EXTERNAL_MAGIC "Reference to perl object (v1.0)"


// Stub for dynamic loading of Perl modules as xs_init function for perl_parse().
// The code for this is generated in Makefile.am into perlxsi.c
EXTERN_C void xs_init( pTHX );


YPerl * YPerl::_yPerl = 0;

// prepend YCP's search path
static void PrependModulePath (pTHX)
{
    YCPPathSearch::initialize ();

    list<string>::const_iterator
	b = YCPPathSearch::searchListBegin (YCPPathSearch::Module),
	e = YCPPathSearch::searchListEnd (YCPPathSearch::Module),
	i;
    // count the number of directories to prepend
    int n = 0;
    for (i = b; i != e; ++i)
    {
	++n;
    }

    AV * INC = get_av ("INC", 1 /* create */);
    av_unshift (INC, n);	// make room in the front

    n = 0;
    for (i = b; i != e; ++i)
    {
	SV *path = newSVpv (i->c_str (), 0);
	av_store (INC, n++, path); // does not update refcount which is ok
    }
}

YPerl::YPerl()
    : _perlInterpreter(0)
    , _interpreterOwnership (true)
{
    _perlInterpreter = perl_alloc();
    PERL_SET_CONTEXT (_perlInterpreter);
    if ( _perlInterpreter )
	perl_construct( _perlInterpreter );

    // Preliminary call perl_parse so the Perl interpreter is ready right away.

    const char *argv[] = { "yperl", "-e", "" };
    int	argc = DIM( argv );

    perl_parse( _perlInterpreter,
		xs_init, // Init function from (generated) perlxsi.c
		argc,
		const_cast<char **> (argv),
		0 );	// env

    PrependModulePath (internalPerlInterpreter ());
}

YPerl::YPerl(pTHX)
    : _perlInterpreter(aTHX)
    , _interpreterOwnership (false)
{
    PrependModulePath (internalPerlInterpreter ());
}

YPerl::~YPerl()
{
    if (_perlInterpreter && _interpreterOwnership)
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

void
YPerl::acceptInterpreter (pTHX)
{
    if ( ! _yPerl )
	_yPerl = new YPerl(aTHX);
    else
    {
	// Do not replace it
	// There are really no multiple interpreters
	// But if there were, we would need to call PERL_SET_CONTEXT
	// all over the place
//	_yPerl->_perlInterpreter = aTHX;
    }
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
    U32 flags = PERL_LOADMOD_NOIMPORT;	// we do not pass arguments for import
    SV * version = 0;
    SV * name = newSVpv( module_name.c_str(), 0 );

    //sv_dump (name);
    // the name is unref'd by load_module, so it must not be mortal
    // on the contrary, we ref it so that the file name gets preserved for debugging
    // :-( does not work
    newRV (name);
    load_module( flags, name, version );
    //sv_dump (name);

/*
    SPAGAIN;
    PUTBACK;

    FREETMPS;
    LEAVE;
*/
    return YCPVoid();
}



/**
 * @param argList arguments start 1!, 0 is dummy
 */
YCPValue
YPerl::callInner (string module, string function, bool method,
		  YCPList argList, constTypePtr wanted_result_type)
{
    EMBEDDED_PERL_DEFS;

    //
    // Determine Perl calling context
    //
    // We used to set G_ARRAY for wanted_result_type being a list.
    // But YCP does not ave a concept of wantarray, so better
    // not create it here.
    // It's better to always want G_SCALAR because that way error
    // handling is easier: we can distinguish between 'nil' and ['nil'].
    int calling_context = G_SCALAR;

    // Using the weird embedded-Perl macros as described in
    // man perlembed, man perlcall, man perlapi, man perlguts
    //
    // It's not pretty. Try to concentrate on the right side (the comments).

    dSP;		// Declare Perl stack pointer
    ENTER;		// Open a new Perl scope
    SAVETMPS;		// Save temporary variables
    PUSHMARK(SP);	// Save stack pointer

    
    // For class method calls put the class name on the stack first
    
    if (method)
    {
	XPUSHs( sv_2mortal( newSVpv( module.c_str(), 0 ) ) );
    }
    

    // Put arguments on the stack
    SV **svs = new SV*[argList->size()];

    for ( int i=1; i < argList->size(); i++ )
    {
	svs[i] = sv_2mortal(newPerlScalar(argList->value(i), false));
	XPUSHs(svs[i]);
    }

    PUTBACK;		// Make local stack pointer global


    //
    // Call the function
    //

    string full_name = module + "::" + function;
    int ret_count = 0;
    
    // so far we use static methods, so call_pv is enough
    ret_count = call_pv( full_name.c_str(), calling_context );


    //
    // Pop result from the stack
    //

    SPAGAIN;		// Copy global stack pointer to local one

    YCPValue result = fromPerlScalar (POPs, wanted_result_type);

    // If we called it with G_ARRAY, we would have to pop all return
    // values and reverse their order. See the CVS history.

    if ( ret_count > 1 )
    {
	// Check for excess return values.
	y2warning ("Perl function %s returned %d arguments, expecting just 1",
		   full_name.c_str(), ret_count );

	// Get rid of excess return values.
	while (--ret_count > 0)
	{
	    (void) POPs;
	}
    }

    PUTBACK;		// Make local stack pointer global

    // Update referenced variables
    for ( int i=1; i < argList->size(); i++ )
    {
	if (argList->value(i)->isReference()) {
	    constTypePtr type = argList->value(i)->asReference()->entry()->type();
	    YCPValue val=fromPerlScalar(svs[i], type);
	    argList->value(i)->asReference()->entry()->setValue(val);
	}
    }
    delete[] svs;

    FREETMPS;		// Free temporary variables
    LEAVE;		// Close the Perl scope

    // fromPerlScalar can return Null to indicate that the type check failed.
    // It is used here and in passing parameters,
    // so specify the location better
    if (result.isNull ())
    {
	y2error ("... when returning from %s", full_name.c_str ());
	result = YCPVoid ();
    }

    return result;
}


SV *
YPerl::newPerlScalar( const YCPValue & xval, bool composite )
{
    EMBEDDED_PERL_DEFS;
    
    YCPValue val = xval;
    if ( val->isReference()) {
	val = val->asReference()->entry()->value();
	if ( val->isString()  )	return newRV(newSVpv( val->asString()->value_cstr(), 0 ));
	if ( val->isInteger() )	return newRV(newSViv( val->asInteger()->value()));
	if ( val->isBoolean() )	return newRV(newSViv( val->asBoolean()->value() ? 1 : 0 ));
	if ( val->isFloat()   )	return newRV(newSVnv( val->asFloat()->value() ));
    }
    
    if ( val->isString()  )	return newSVpv( val->asString()->value_cstr(), 0 );
    if ( val->isList()    )	return newPerlArrayRef( val->asList() );
    if ( val->isMap()     )	return newPerlHashRef( val->asMap() );
    if ( val->isInteger() )	return newSViv( val->asInteger()->value() );
    if ( val->isBoolean() )	return newSViv( val->asBoolean()->value() ? 1 : 0 );
    if ( val->isExternal()) {
	YCPExternal ex = val->asExternal();
	if (ex->magic() != string(YCP_EXTERNAL_MAGIC)) {
	    y2error("Unexpected magic '%s'.", (ex->magic()).c_str());
	    return 0;
	}
	return newRV_inc((SV*)(ex->payload()));
    }
    if ( val->isByteblock())
    {
	YCPByteblock b = val->asByteblock();
	return newSVpv (reinterpret_cast<const char *> (b->value ()), b->size ());
    }
    if ( val->isTerm()    )
    {
	YCPTerm t = val->asTerm ();
	YCPString name = t->name ();
	YCPList args = t->args ();
	return callConstructor ("YaST::YCP::Term", "YaST::YCP::Term::new",
				args->functionalAdd (name, true /*prepend*/ ));
    }
    if ( val->isSymbol () )
    {
	YCPList args;
	return callConstructor ("YaST::YCP::Symbol", "YaST::YCP::Symbol::new",
				args->functionalAdd (
				    YCPString (
					val->asSymbol ()->symbol ())));
    }
    if ( val->isFloat()   )	return newSVnv( val->asFloat()->value() );
    if ( val->isVoid()    )	return composite? newSV (0): &PL_sv_undef;

    // YuCK, stringify
    y2error ("Unhandled conversion from YCP type #%d", val->valuetype ());
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
		// there is no format for U32
		y2internal ("Reference count is %" UVuf " (should be 1)",
			    (UV) SvREFCNT( scalarVal ) );
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
		    y2internal( "Reference count is %" UVuf " (should be 1)",
				(UV) SvREFCNT( scalarVal ) );
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
    EMBEDDED_PERL_DEFS;

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

    std::ostringstream ss;

    ss << (
	SvIOK (sv)? "integer, ":
	SvNOK (sv)? "float, ":
	SvPOK (sv)? "string, ":
	"");

    if (sv_isobject (sv))
    {
	ss << HvNAME (SvSTASH (SvRV (sv))) << ", ";
    }

    U32 f = SvFLAGS (sv);
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

void perl_class_destructor(void *ref, string magic)
{
    if (YPerl::_yPerl && magic == string(YCP_EXTERNAL_MAGIC)) {
	y2debug("perl-bindings YCPExternal destructor [[[");
	dTHX;
	SvREFCNT_dec((SV*)ref);
	y2debug("perl-bindings YCPExternal destructor ]]]");
    }
}

void YPerl::fromPerlClassToExternal(const char *class_name, SV *sv, YCPValue &out)
{
    SV * ref = SvRV(sv);
    SvREFCNT_inc(ref);
    
    YCPExternal ex(ref, string(YCP_EXTERNAL_MAGIC), &perl_class_destructor);
    out = ex;
}


bool
YPerl::tryFromPerlClassByteblock (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::Byteblock"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::Byteblock::value");
	STRLEN len;
	const char * pv = SvPV (sval, len);
	out = YCPByteblock (reinterpret_cast<const unsigned char *> (pv), len);
	SvREFCNT_dec (sval);
	return true;
    }
    else
    {
	return false;
    }
}

bool
YPerl::tryFromPerlClassInteger (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::Integer"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::Integer::value");
	out = YCPInteger (SvIV (sval));
	SvREFCNT_dec (sval);
	return true;
    }
    else
    {
	return false;
    }
}

bool
YPerl::tryFromPerlClassFloat (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::Float"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::Float::value");
	out = YCPFloat (SvNV (sval));
	SvREFCNT_dec (sval);
	return true;
    }
    else
    {
	return false;
    }
}

bool
YPerl::tryFromPerlClassString (const char *class_name, SV *sv, YCPValue &out)
{
    EMBEDDED_PERL_DEFS;
    if (!strcmp (class_name, "YaST::YCP::String"))
    {
	SV *sval = callMethod (sv, "YaST::YCP::String::value");
	out = YCPString (SvPV_nolen (sval));
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

    bool mismatch = false; // type mismatch?
    YCPValue val = YCPNull ();

    // Decide by the wanted type,
    // Except first check if we got undef and in that case return nil
    if (!SvOK (sv) || wanted_type->isVoid ())
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
	    mismatch = !tryFromPerlClassBoolean (class_name, sv, val);
	}
	else
	{
	    if (SvROK(sv))
		sv = SvRV(sv);
	    val = YCPBoolean (SvTRUE (sv));
	}
    }
    else if (wanted_type->isString ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    mismatch = !tryFromPerlClassString (class_name, sv, val);
	}
	else
	{
	    if (SvROK(sv))
		sv = SvRV(sv);
	    // Perl relies on automatic coercion between strings and numbers
	    // So to behave more like it,
	    // instead of "if (SvXOK (sv)) SvXV (sv)"
	    // we first SvXV (sv) and only then SvXOK.
	    const char *pv = SvPV_nolen (sv);
	    if (SvPOK (sv))
	    {
		val = YCPString (pv);
	    }
	    else
	    {
		mismatch = true;
	    }
	}
    }
    else if (wanted_type->isInteger ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    mismatch = !tryFromPerlClassInteger (class_name, sv, val);
	}
	else
	{
	    if (SvROK(sv))
		sv = SvRV(sv);
	    // see isString
	    IV iv = SvIV (sv);
	    if (SvIOK (sv))
	    {
		val = YCPInteger (iv);
	    }
	    else
	    {
		mismatch = true;
	    }
	}
    }
    else if (wanted_type->isFloat ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    mismatch = !tryFromPerlClassFloat (class_name, sv, val);
	}
	else
	{
	    if (SvROK(sv))
		sv = SvRV(sv);
	    // see isString
	    NV nv = SvNV (sv);
	    if (SvNOK (sv))
	    {
		val = YCPFloat (nv);
	    }
	    else
	    {
		mismatch = true;
	    }

	}
    }
    else if (wanted_type->isSymbol ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    mismatch = !tryFromPerlClassSymbol (class_name, sv, val);
	}
	else
	{
	    // see isString
	    const char *pv = SvPV_nolen (sv);
	    if (SvPOK (sv))
	    {
		val = YCPSymbol (pv);
	    }
	    else
	    {
		mismatch = true;
	    }
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
	    mismatch = !tryFromPerlClassTerm (class_name, sv, val);
	}
//	else if //... try from a list
	else
	{
	    mismatch = true;
	}
    }
    else if (wanted_type->isPath ())
    {
	// a string
	// see isString
	const char *pv = SvPV_nolen (sv);
	if (SvPOK (sv))
	{
	    val = YCPPath (pv);
	}
	else
	{
	    mismatch = true;
	}
	// maybe allow list later?
    }
    else if (wanted_type->isByteblock ())
    {
	if (sv_isobject (sv))
	{
	    char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	    mismatch = !tryFromPerlClassByteblock (class_name, sv, val);
	}
	else
	{
	    // see isString
	    STRLEN len;
	    const char * pv = SvPV (sv, len);
	    if (SvPOK (sv))
	    {
		val = YCPByteblock (reinterpret_cast<const unsigned char *> (pv), len);
	    }
	    else
	    {
		mismatch = true;
	    }
	}
    }
    else if (wanted_type->isList ())
    {
	mismatch = true;
	if (SvROK (sv))
	{
	    SV *ref = SvRV (sv);
	    if (SvTYPE (ref) == SVt_PVAV)
	    {
		constListTypePtr list_type = (constListTypePtr) wanted_type;
		val = fromPerlArray ((AV *) ref, list_type->type ());
		mismatch = false;
	    }
	}
    }
    else if (wanted_type->isMap ())
    {
	mismatch = true;
	if (SvROK (sv))
	{
	    SV *ref = SvRV (sv);
	    if (SvTYPE (ref) == SVt_PVHV)
	    {
		constMapTypePtr map_type = (constMapTypePtr) wanted_type;
		val = fromPerlHash ((HV *) ref,
				    map_type->keytype (),
				    map_type->valuetype ());
		mismatch = false;
	    }
	}
    }
/*
  tuple?
 */
    else
    {
	y2internal ("Unhandled conversion to %s from %s",
		    wanted_type->toString ().c_str (), debugDump(sv).c_str ());
    }

    if (mismatch)
    {
	y2error ("Expected %s, got %s",
		 wanted_type->toString ().c_str (),
		 debugDump (sv).c_str ());
    }

    return val;
}

// call a method of a perl object
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

// call a constructor of a perl object
SV*
YPerl::callConstructor (const char * class_name, const char * full_method_name,
			YCPList args)
{
    EMBEDDED_PERL_DEFS;

    // Ensure that the module is imported
    // This is enough when all of them reside in a single pm file
    static bool module_imported = false;
    if (! module_imported)
    {
	// It does not crash anymore, why?
	//Importing YaST::YCP initializes YCP twice and makes it crash :-(

	YCPList m;
	m->add (YCPString ("YaST::YCP"));
	loadModule (m);
	module_imported = true;
    }

    SV *ret = &PL_sv_undef;

    dSP;
    ENTER;
    SAVETMPS;

    // This function can be called recursively (typically if there are
    // nested terms). I don't understand the Perl stack well enough
    // so create the argument SVs when the stack is in a consistent state.

    int nargs = args.size ();
    // the array does not own the pointers, FREETMPS does
    SV ** sv_args = new SV *[nargs];
    for (int i = 0; i < nargs; ++i)
    {
	sv_args[i] = sv_2mortal (newPerlScalar (args->value (i), false ));
    }
    
    PUSHMARK (SP);

    // Class name
    XPUSHs (sv_2mortal (newSVpv (class_name, 0)));

    // Other arguments
    for (int i = 0; i < nargs; ++i)
    {
	XPUSHs (sv_args[i]);
    }

    PUTBACK;

    delete[] (sv_args);

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
    // when it no longer needs it
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

    // Do not convert scalars to numbers!
    // This may look strange, but it is better to get consistent
    // results than expect a string and sometimes(!) get a number just
    // because the string looks like a number.  For returning numbers,
    // use the explicit data classes YaST::YCP::Integer.

    const char *pv = SvPV_nolen (sv);
    if (SvPOK (sv))
    {
	val = YCPString (pv);
    }
    else if (sv_isobject (sv))
    {
	char *class_name = HvNAME (SvSTASH (SvRV (sv)));
	if (
	    !tryFromPerlClassBoolean	(class_name, sv, val) &&
	    !tryFromPerlClassByteblock	(class_name, sv, val) &&
	    !tryFromPerlClassInteger	(class_name, sv, val) &&
	    !tryFromPerlClassFloat	(class_name, sv, val) &&
	    !tryFromPerlClassString	(class_name, sv, val) &&
	    !tryFromPerlClassSymbol	(class_name, sv, val) &&
	    !tryFromPerlClassTerm	(class_name, sv, val) &&
	    true)
	{
	    fromPerlClassToExternal(class_name, sv, val);
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
	    y2internal ("av_fetch returned NULL for index %" IVdf, (IV) i);
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
