// BLOCXX_NAMESPACE::String and std::string
// author: Martin Lazar <mlazar@suse.cz>

%include "std_string.i";

%{
#include <blocxx/String.hpp>
%}

class BLOCXX_NAMESPACE::String;

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

%typemap(in) 
    BLOCXX_NAMESPACE::String* (BLOCXX_NAMESPACE::String temp),
    BLOCXX_NAMESPACE::String& (BLOCXX_NAMESPACE::String temp),
    const BLOCXX_NAMESPACE::String* (BLOCXX_NAMESPACE::String temp),
    const BLOCXX_NAMESPACE::String& (BLOCXX_NAMESPACE::String temp)
{
    if (!SvROK($input))
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to a string.\n");
    SV *sv = (SV *)SvRV($input);
    if (!SvPOK(sv))
        SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to a STRING.\n");
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
