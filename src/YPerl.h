/*--- -------------------------------------------------------*- c++ -*-\
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

  File:	      YPerl.h

  Author:     Stefan Hundhammer <sh@suse.de>
  	      Martin Vidner <mvidner@suse.cz>
/-*/


#ifndef YPerl_h
#define YPerl_h

#include <EXTERN.h>
#include <perl.h>

#include <ycp/YCPList.h>
#include <ycp/Type.h>

class YPerl
{
public:

    /**
     * Load a Perl module - equivalent to "use" in Perl.
     *
     * Returns a YCPError on failure, YCPVoid on success.
     **/
    static YCPValue loadModule( YCPList argList );

    /**
     * Access the static (singleton) YPerl object. Create it if it isn't
     * created yet.
     *
     * Returns 0 on error.
     **/
    static YPerl * yPerl();

    /**
     * Tell it that we are an XSUB and have our own interpreter.
     * If there was no YPerl object, create it and lend it the interpreter
     * (it should not delete it in its destructor).
     * If the YPerl object exists already, assume that the interpreter
     * did not change and do nothing.
     **/
    static void acceptInterpreter (pTHX);

    /**
     * Access the static (singleton) YPerl object's embedde Perl
     * interpreter. Create and initialize it if it isn't created yet.
     *
     * Returns 0 on error.
     **/
    static PerlInterpreter * perlInterpreter();

    /**
     * Destroy the static (singleton) YPerl object and unload the embedded Perl
     * interpreter.
     *
     * Returns YCPVoid().
     **/
    static YCPValue destroy();

    
protected:

    /**
     * Protected constructor. Use one of the static methods rather than
     * instantiate an object of this class yourself.
     **/
    YPerl();

    /**
     * Protected constructor. Use one of the static methods rather than
     * instantiate an object of this class yourself.
     **/
    YPerl(pTHX);

    /**
     * Destructor.
     **/
    ~YPerl();

    /**
     * Returns the internal embedded Perl interpreter.
     **/
    PerlInterpreter * internalPerlInterpreter() const
	{ return _perlInterpreter; }

public:
    /**
     * Generic Perl call.
     **/
    YCPValue callInner (string module, string function, bool method,
			YCPList argList, constTypePtr wanted_result_type);
    
    /**
     * Create a new Perl scalar value from a YCP value.
     * @param composite If an undef should go to an array/hash, it must be represented specially.
     **/
    SV * newPerlScalar( const YCPValue & val, bool composite );

protected:
    /**
     * Create a Reference to a new Perl array from a YCP list.
     **/
    SV * newPerlArrayRef( const YCPList & list );

    /**
     * Create a Reference to a new Perl hash from a YCP map.
     **/
    SV * newPerlHashRef( const YCPMap & map );

public:
    /**
     * Convert a Perl scalar to a YCPValue.
     *
     * If the types mismatch, YCPNull is returned and an error to
     * the log file is issued.
     **/
    YCPValue fromPerlScalar( SV * perl_scalar,
			     constTypePtr wanted_type);

protected:
    //! call a pethod of a perl object
    //! that takes no arguments and returns one scalar
    SV* callMethod (SV * instance, const char * full_method_name);

    //! call a constructor of a perl object
    SV* callConstructor (const char * class_name,
			 const char * full_method_name,
			 YCPList args);
    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Boolean.
     * If yes, store its value into out
     */
    bool tryFromPerlClassBoolean (const char *class_name, SV *sv,
				  YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Byteblock.
     * If yes, store its value into out
     */
    bool tryFromPerlClassByteblock (const char *class_name, SV *sv,
				  YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Integer.
     * If yes, store its value into out
     */
    bool tryFromPerlClassInteger (const char *class_name, SV *sv,
				 YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Float.
     * If yes, store its value into out
     */
    bool tryFromPerlClassFloat (const char *class_name, SV *sv,
				 YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::String.
     * If yes, store its value into out
     */
    bool tryFromPerlClassString (const char *class_name, SV *sv,
				 YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Symbol.
     * If yes, store its value into out
     */
    bool tryFromPerlClassSymbol (const char *class_name, SV *sv,
				 YCPValue &out);

    /**
     * Given that sv is an object and its class name is class_name,
     * check whether it is YaST::YCP::Term.
     * If yes, store its value into out
     */
    bool tryFromPerlClassTerm (const char *class_name, SV *sv, YCPValue &out);

    /**
     * This is copied from the original sh's function.
     * It converts according to what Perl provides, not what YCP wants.
     */
    YCPValue fromPerlScalarToAny (SV * perl_scalar);

    /**
     * Convert a Perl array to a YCPList.
     **/
    YCPList fromPerlArray (AV * array, constTypePtr wanted_type);
    
    /**
     * Convert a Perl hash to a YCPMap.
     **/
    YCPMap fromPerlHash (HV * hv, constTypePtr key_type, constTypePtr value_type);


    // Data members.

    PerlInterpreter *	_perlInterpreter;
    bool		_interpreterOwnership; //!<  we create == we delete

    static YPerl *	_yPerl;
};

//! The weird Perl macros need a PerlInterpreter * named 'my_perl' (!) almost everywhere.
#define EMBEDDED_PERL_DEFS PerlInterpreter * my_perl = YPerl::perlInterpreter()

#endif	// YPerl_h
