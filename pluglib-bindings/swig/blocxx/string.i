/*
 * file:	blocxx/_string.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * BloCxx string typemaps and helpers
 *
 * $Id$
 */

class BLOCXX_NAMESPACE::String;

%{

#include <blocxx/String.hpp>

/* convert BloCxx::String to SV */
bool FROM_BLOCXX_STRING(SV *&sv, const BLOCXX_NAMESPACE::String *x, int size, const swig_type_info *t) {
    sv_setpv(sv,x->c_str());
    return true;
}

/* convert SV to BloCxx::String */
bool TO_BLOCXX_STRING(SV* sv, BLOCXX_NAMESPACE::String *x, int size, const swig_type_info *t) { 
    *x = BLOCXX_NAMESPACE::String(SvPV_nolen(sv));
    return true;
}

%}


%typemap(in) BLOCXX_NAMESPACE::String {
    STRLEN len;
    const char *ptr = SvPV($input, len);
    $1 = ptr ? BLOCXX_NAMESPACE::String(ptr, len) : BLOCXX_NAMESPACE::String();
}

%typemap(out) BLOCXX_NAMESPACE::String {
    if (argvi >= items) EXTEND(sp, 1);
    sv_setpv($result = sv_newmortal(), $1.c_str());
    ++argvi;
}

%typemap(in) BLOCXX_NAMESPACE::String* (BLOCXX_NAMESPACE::String temp), BLOCXX_NAMESPACE::String& (BLOCXX_NAMESPACE::String temp)
{
    SV *sv;
    if (!SvROK($input) || !(sv = (SV*)SvRV($input)))
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to STRING.\n");
    STRLEN len;
    const char *ptr = SvPV(sv, len);
    if (!ptr)
        SWIG_croak("Undefined variable in argument $argnum of $symname.");
    temp = ptr ? BLOCXX_NAMESPACE::String(ptr, len) : BLOCXX_NAMESPACE::String();
    $1 = &temp;
}

%typemap(argout) BLOCXX_NAMESPACE::String*, BLOCXX_NAMESPACE::String&
{
    SV *sv = (SV *)SvRV($input);
    sv_setpv(sv, $1->c_str());
}
