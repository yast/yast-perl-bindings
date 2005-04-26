// std::list
// author: Martin Lazar <mlazar@suse.cz>

%include "std_Common.i"

namespace std {
    class deque;
    class list;
}

%define specialize_List(L,T)

%typemap(in) L< T > {
    // convert from L< T > to perl scalar
    AV *av = (AV*)SwigDeref($input, SVt_PVAV, $argnum, "$symname");
    if (!av) SWIG_fail;
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

%typemap(in) L< L< T > > {
    AV *av = (AV*)SwigDeref($input, SVt_PVAV, $argnum, "$symname");
    if (!av) SWIG_fail;
    I32 len = av_len(av) + 1;
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	if (sv) {
	    AV *av1 = (AV*)SwigDeref(*sv, SVt_PVAV, $argnum, "$symname");
	    if (!av1) SWIG_fail;
	    I32 len1 = av_len(av1) + 1;
	    L< T > l;
	    for (int j=0; j<len1; j++) {
		SV **sv1 = av_fetch(av1, j, 0);
		if (sv1) {
		    T val;
		    SwigConvertFromSv(*sv1, val);
		    l.push_back(val);
		}
	    }
	    $1.push_back(l);
	}
    }
}

%typemap(in) L< T >& (L< T > temp), L< T >* (L< T > temp) {
    // convert from reference/pointer to L< T > to perl scalar
    AV *av = (AV*)SwigDeref($input, SVt_PVAV, $argnum, "$symname");
    if (!av) SWIG_fail;
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

%typemap(in) L< L< T > >& (L< L< T > > temp), L< L< T > >* (L< L< T > > temp) {
    AV *av = (AV*)SwigDeref($input, SVt_PVAV, $argnum, "$symname");
    if (!av) SWIG_fail;
    I32 len = av_len(av) + 1;
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	if (sv) {
	    AV *av1 = (AV*)SwigDeref(*sv, SVt_PVAV, $argnum, "$symname");
	    if (!av1) SWIG_fail;
	    I32 len1 = av_len(av1) + 1;
	    L< T > l;
	    for (int j=0; j<len1; j++) {
		SV **sv1 = av_fetch(av1, j, 0);
		if (sv1) {
		    T val;
		    SwigConvertFromSv(*sv1, val);
		    l.push_back(val);
		}
	    }
	    temp.push_back(l);
	}
    }
    $1 = &temp;
}


%typemap(argout) L< T >&, L< T >* {
    AV *av = (AV *)SvRV($input);
    av_clear(av);
    for (L< T >::const_iterator i=$1->begin(); i!=$1->end(); i++) {
	SV * sv = newSV(0);
	SwigConvertToSv(sv, *i);
	av_push(av, sv);
    }
}

%typemap(argout) L< L< T > >&, L< L< T > >* {
    AV *av = (AV *)SvRV($input);
    av_clear(av);
    for (L< L< T > >::const_iterator i=$1->begin(); i!=$1->end(); i++) {
	SV * sv = newSV(0);
	int len1 = i->size();
	AV *av1 = newAV();
	for (L< T >::const_iterator j=i->begin(); j!=i->end(); j++) {
	    SV * sv = newSV(0);
	    SwigConvertToSv(sv, *j);
	    av_push(av1, sv);
	}
	SV* rv = newRV_noinc((SV*)av1);
	av_push(av, rv);
    }
}

%typemap(out) L< T > {
    unsigned int k = 0;
    int len = $1.size();
    SV **svs = new SV*[len];
    for (L< T >::const_iterator i=$1.begin(); i!=$1.end(); i++) {
	svs[k] = sv_newmortal();
        SwigConvertToSv(svs[k++], *i);
    }
    $result = newRV_noinc((SV*)av_make(len, svs));
    sv_2mortal($result);
    delete[] svs;
    argvi++;
}

%typemap(out) L< L< T > > {
    AV *av = newAV();
    for (L< L< T > >::const_iterator i=$1.begin(); i!=$1.end(); i++) {
	int len = i->size();
	unsigned k = 0;
	SV **svs = new SV*[len];
        for (L< T >::const_iterator j=i->begin(); j!=i->end(); j++) {
	    svs[k] = sv_newmortal();
    	    SwigConvertToSv(svs[k++], *j);
	}
	av_push(av, newRV_noinc((SV*)av_make(len, svs)));
	delete[] svs;
    }
    $result = newRV_noinc((SV*)av);
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



