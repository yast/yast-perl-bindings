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

#include <ycp/YCPString.h>
#include <ycp/YCPVoid.h>
#include <ycp/YCPError.h>

#include <YPerl.h>

#define DIM(ARRAY)	( sizeof( ARRAY )/sizeof( ARRAY[0] ) )


YPerl * YPerl::_yPerl = 0;


YPerl::YPerl()
    : _perlInterpreter(0)
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


void
YPerl::destroy()
{
    if ( _yPerl )
	delete _yPerl;

    _yPerl = 0;
}


PerlInterpreter *
YPerl::perlInterpreter()
{
    if ( YPerl::yPerl() )
	return YPerl::yPerl()->internalPerlInterpreter();

    return 0;
}


YCPValue
YPerl::call( YCPList argList )
{
    PerlInterpreter * perl = YPerl::perlInterpreter();

    if ( ! perl )
	return YCPNull();

    if ( argList->size() < 1 || ! argList->value(0)->isString() )
	return YCPError( "Perl::Call(): Bad arguments: No script to execute!" );

    string scriptName = argList->value(0)->asString()->value();

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
    const char *argv[] = { "-w", "-e", expr.c_str() };
    int	argc = DIM( argv );

    perl_parse( perl,
		0,	// xsinit function
		argc,
		const_cast<char **> (argv),
		0 );	// env
    perl_run( perl );

    return YCPVoid();
}
