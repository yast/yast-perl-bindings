// std::list

%include "std_Common.i"

namespace std {
    class deque;
    class list;
}

%define specialize_List(L,T)

%typemap(in) L< T > {
    if (!SvROK($input))
        SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to an array of " #T ".\n");
    AV *av = (AV *)SvRV($input);
    if (SvTYPE(av) != SVt_PVAV)
        SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to an ARRAY of " #T ".\n");
    I32 len = av_len(av) + 1;
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	if (sv) {
	    T val;
	    SwigConvertFromSv(*sv, val);
	    $1.push_back(val);
	}
    }
}

%typemap(in) L< T >& (L< T > temp), L< T >* (L< T > temp) {
    if (!SvROK($input))
        SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to an array of " #T ".\n");
    AV *av = (AV *)SvRV($input);
    if (SvTYPE(av) != SVt_PVAV)
        SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to an ARRAY of " #T ".\n");
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	if (sv) {
	    T val;
	    SwigConvertFromSv(*sv, val);
	    temp.push_back(val);
	}
    }
    $1 = &temp;
}

%typemap(argout) L< T >&, L< T >* {
    int len = $1->size();
    AV *av = (AV *)SvRV($input);
    av_clear(av);
    for (L< T >::const_iterator i=$1->begin(); i!=$1->end(); i++) {
	SV * sv = newSV(0);
	SwigConvertToSv(sv, *i);
	av_push(av, sv);
    }
}

%typemap(out) L< T > {
    L< T >::const_iterator i;
    unsigned int j;
    int len = $1.size();
    SV **svs = new SV*[len];
    for (i=$1.begin(), j=0; i!=$1.end(); i++, j++) {
	svs[j] = sv_newmortal();
        SwigConvertToSv(svs[j], *i);
    }
    AV *myav = av_make(len, svs);
    delete[] svs;
    $result = newRV_noinc((SV*) myav);
    sv_2mortal($result);
    argvi++;
}
%enddef

%define specialize_stdList(T)
specialize_List(std::list,T)
specialize_List(std::deque,T)
%enddef


specialize_stdList(bool)

specialize_stdList(signed char)
specialize_stdList(signed short)
specialize_stdList(signed int)
specialize_stdList(signed long)
specialize_stdList(signed long long)

specialize_stdList(unsigned char)
specialize_stdList(unsigned short)
specialize_stdList(unsigned int)
specialize_stdList(unsigned long)
specialize_stdList(unsigned long long)

specialize_stdList(float)
specialize_stdList(double)

specialize_stdList(std::string)

specialize_stdList(std::list<int>)
