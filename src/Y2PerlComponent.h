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
// not yet, prototype the transparent bindings first
//    YCPValue evaluate( const YCPValue & val );

    /**
     * Try to import a given namespace. This method is used
     * for transparent handling of namespaces (YCP modules)
     * through whole YaST.
     * @param name_space the name of the required namespace
     * @param timestamp a string containing unique timestamp
     * if only the given timestamp is requested. If not NULL,
     * component must provide a namespace with exactly the
     * same timestamp.
     * @return on errors, NULL should be returned. The
     * error reporting must be done by the component itself
     * (typically using y2log). On success, the method
     * should return a proper instance of the imported namespace
     * ready to be used. The returned instance is still owned
     * by the component, any other part of YaST will try to
     * free it. Thus, it's possible to share the instance.
     */
    Y2Namespace *import (const char* name, const char* timestamp = NULL);
};

#endif	// Y2PerlComponent_h
