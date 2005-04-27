// author: Martin Lazar <mlazar@suse.cz>

%runtime "yast_perlrun.swg"

%include "std_string.i"
%include "typemaps.i"

%include "std_List.i"
%include "std_Hash.i"

%typemap(in) std::string* (std::string temp), std::string& (std::string temp),
    const std::string* (std::string temp), const std::string& (std::string temp)
{
    SV *sv = (SV*)SwigDeref($input, SVt_PV, $argnum, "$symname");
    if (!sv) SWIG_fail;
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
