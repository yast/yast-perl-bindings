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

  File:	      PerlLogger.cc

  Author:     Stanislav Visnovsky <visnov@suse.cz>

/-*/


#include "PerlLogger.h"
#include <ycp/ExecutionEnvironment.h>


void
PerlLogger::error (string error_message)
{
    y2_logger(LOG_ERROR, "Perl", YaST::ee.filename().c_str(),
	      YaST::ee.linenumber(), "", "%s", error_message.c_str());
}


void
PerlLogger::warning (string warning_message)
{
    y2_logger(LOG_ERROR, "Perl", YaST::ee.filename().c_str(),
	      YaST::ee.linenumber(), "", "%s", warning_message.c_str());
}


PerlLogger*
PerlLogger::instance ()
{
    if ( ! m_perllogger )
    {
	m_perllogger = new PerlLogger ();
    }
    return m_perllogger;
}

PerlLogger* PerlLogger::m_perllogger = NULL;
