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

  File:	      PerlLogger.h

  Author:     Stanislav Visnovsky <visnov@suse.cz>

/-*/


#ifndef PerlLogger_h
#define PerlLogger_h

#include "ycp/y2log.h"

/**
 * @short A class to provide logging for Perl bindings errors and warning
 */
class PerlLogger : public Logger
{
    static PerlLogger* m_perllogger;

public:
    void error (string message);
    void warning (string message);

    static PerlLogger* instance ();
};

#endif	// ifndef PerlLogger_h


// EOF
