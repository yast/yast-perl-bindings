/*-----------------------------------------------------------*- c++ -*-\
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

  File:	      Y2CCPerl.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef _Y2CCPerl_h
#define _Y2CCPerl_h

#include "Y2PerlComponent.h"

/**
 * @short Y2ComponentCreator that creates Perl-from-YCP bindings.
 *
 * A Y2ComponentCreator is an object that can create components.
 * It receives a component name and - if it knows how to create
 * such a component - returns a newly created component of this
 * type. Y2CCPerl can create components with the name "Perl".
 */
class Y2CCPerl : public Y2ComponentCreator
{
private:
    Y2Component *cperl;

public:
    /**
     * Creates a Perl component creator
     */
    Y2CCPerl() : Y2ComponentCreator( Y2ComponentBroker::BUILTIN ),
	cperl (0) {};

    ~Y2CCPerl () {
	if (cperl)
	    delete cperl;
    }

    /**
     * Returns true, since the Perl component is a YaST2 server.
     */
    bool isServerCreator() const { return true; };

    /**
     * Creates a new Perl component.
     */
    Y2Component *create( const char * name ) const
    {
	// create as many as requested, they all share the static YPerl anyway
	if ( ! strcmp( name, "perl") ) return new Y2PerlComponent();
	else return 0;
    }

    /**
     * always returns the same component, deletes it finally
     */
    Y2Component *provideNamespace (const char *name);

};

#endif	// ifndef _Y2CCPerl_h


// EOF
