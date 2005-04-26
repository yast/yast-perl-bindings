// author: Martin Lazar <mlazar@suse.cz>

%runtime "yast_perlrun.swg"

%include "std_string.i"
%include "typemaps.i"

%include "std_List.i"

%typemap(in) std::string* (std::string temp), std::string& (std::string temp),
    const std::string* (std::string temp), const std::string& (std::string temp)
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
    temp.assign(ptr, len);
    $1 = &temp;
}

%typemap(argout) std::string*, std::string&
{
    SV *sv = (SV *)SvRV($input);
    sv_setpv(sv, $1->c_str());
}

%typemap(argout) const std::string*, const std::string&;
%typemap(out) std::string*, std::string&, const std::string*, const std::string&;
