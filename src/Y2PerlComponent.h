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

class YPerlInterpreter;

/**
 * @short YaST2 Component: Qt user interface
 * The YaST2 Component realizes a Qt based user interface with an
 * embedded YCP interpreter. It component name is "qt".
 */
class Y2PerlComponent : public Y2Component
{
public:
    /**
     * Initialize data.
     */
    Y2PerlComponent();

    /**
     * Cleans up.
     */
    ~Y2PerlComponent();

    /**
     * The name of this component.
     */
    string name() const { return "perl"; }

    /**
     * Implements the server. The interpreter is created here and not
     * in the constructor, because in the meantime the server options
     * may have been set.
     */
    YCPValue evaluate( const YCPValue & command );

    /**
     * Is called by the genericfrontend, when the session is finished.
     * Close the user interace here.
     */
    void result( const YCPValue & result );

    
protected:
    
    YPerlInterpreter *	_interpreter;
};

#endif	// Y2PerlComponent_h

