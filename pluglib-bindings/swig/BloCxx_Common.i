
%{
#include <blocxx/String.hpp>

/* convert any to SV */
void SwigConvertToSv(SV *&sv, const BLOCXX_NAMESPACE::String &x) { sv_setpv(sv,x.c_str());}

/* convert from SV to any */
void SwigConvertFromSv(SV* sv, BLOCXX_NAMESPACE::String &x) { x = BLOCXX_NAMESPACE::String(SvPV_nolen(sv)); }

%}
