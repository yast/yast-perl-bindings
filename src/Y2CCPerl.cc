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

  File:	      Y2CCPerl.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include "Y2CCPerl.h"

// This is very important: We create one global variable of
// Y2CCPerl. Its constructor will register it automatically to
// the Y2ComponentBroker, so that will be able to find it.
// This all happens before main() is called!

Y2CCPerl g_y2ccperl;


