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

  File:	      YPerl.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YPerl_h
#define YPerl_h

#include <ycp/YCPList.h>


// Cheat g++ to accept a forward declaration of PerlInterpreter.
// g++ doesn't accept "class PerlInterpreter" if that "PerlInterpreter" is
// later typedef'ed to a struct.
class interpreter;
typedef struct interpreter PerlInterpreter;


class YPerl
{
public:

    /**
     * Call a Perl script.
     *
     *		***MORE DOC TO FOLLOW***
     *		***MORE DOC TO FOLLOW***
     *		***MORE DOC TO FOLLOW***
     **/
    static YCPValue call( YCPList argList );

    /**
     * Evaluate a Perl expression.
     **/
    static YCPValue eval( YCPList argList );

    /**
     * Access the static (singleton) YPerl object. Create it if it isn't
     * created yet.
     *
     * Returns 0 on error.
     **/
    static YPerl * yPerl();

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
     **/
    static void destroy();


protected:

    /**
     * Protected constructor. Use one of the static methods rather than
     * instantiate an object of this class yourself.
     **/
    YPerl();

    /**
     * Destructor.
     **/
    ~YPerl();

    /**
     * Returns the internal embedded Perl interpreter.
     **/
    PerlInterpreter * internalPerlInterpreter() const
	{ return _perlInterpreter; }


    // Data members.

    PerlInterpreter *	_perlInterpreter;
    static YPerl *	_yPerl;
};


#endif	// YPerl_h
