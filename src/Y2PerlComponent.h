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

  File:	      Y2PerlComponent.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef Y2PerlComponent_h
#define Y2PerlComponent_h

#include "Y2.h"


/**
 * @short YaST2 Component: Perl bindings
 */
class Y2PerlComponent : public Y2Component
{
public:
    /**
     * Constructor.
     */
    Y2PerlComponent();

    /**
     * Destructor.
     */
    ~Y2PerlComponent();

    /**
     * The name of this component.
     */
    string name() const { return "perl"; }

    /**
     * Is called by the generic frontend when the session is finished.
     */
    void result( const YCPValue & result );

    /**
     * Implements the Perl:: functions.
     **/
    YCPValue evaluate( const YCPValue & val );
};

#endif	// Y2PerlComponent_h

