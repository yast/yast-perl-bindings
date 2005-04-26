// Associative Containers
// author: Martin Lazar <mlazar@suse.cz>

%include "std_Common.i"

namespace std {
    class map;
}

%define specialize_Hash(L,A,B)

%typemap(in) L< A, B > {
    // convert from L< A, B > to perl scalar
    HV *hv = (HV*)SwigDeref($input, SVt_PVHV, $argnum, "$symname");
    hv_iterinit(hv);
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;
    A key;
    B val;
    while(HE *he=hv_iternext(hv)) {
	SwigConvertFromSv(hv_iterkeysv(he), key);
	SwigConvertFromSv(hv_iterval(hv, he), val);
	$1.operator[](key) = val;
    }
}

%typemap(in) L< A, B >& (L< A, B > temp), L< A, B >* (L< A, B > temp) {
    // convert from pointer or reference to L< A, B > to perl scalar
    HV *hv = (HV*)SwigDeref($input, SVt_PVHV, $argnum, "$symname");
    hv_iterinit(hv);
    A key;
    B val;
    while(HE *he=hv_iternext(hv)) {
	SwigConvertFromSv(hv_iterkeysv(he), key);
	SwigConvertFromSv(hv_iterval(hv, he), val);
	temp.operator[](key) = val;
    }
    $1 = &temp;
}

%typemap(argout) L< A, B >&, L< A, B >* {
    HV *hv = (HV *)SvRV($input);
    hv_clear(hv);
    for (L< A, B >::const_iterator i=$1->begin(); i!=$1->end(); i++) {
	SV *key = newSV(0);
	SwigConvertToSv(key, (A)i->first);
	SV *val = newSV(0);
	SwigConvertToSv(val, (B)i->second);
	hv_store_ent(hv, key, val, 0);
    }
}

%typemap(out) L< A, B > {
    HV *hv = newHV();
    for (L< A, B >::const_iterator i=$1.begin(); i!=$1.end(); i++) {
	SV *key = newSV(0);
	SwigConvertToSv(key, (A)i->first);
	SV *val = newSV(0);
	SwigConvertToSv(val, (B)i->second);
	hv_store_ent(hv, key, val, 0);
    }
    $result = newRV_noinc((SV*)hv);
    sv_2mortal($result);
    argvi++;
}

%enddef


%define specialize_Hash2(L, T)
specialize_Hash(L, signed char,T)
specialize_Hash(L, signed short,T)
specialize_Hash(L, signed int,T)
specialize_Hash(L, signed long,T)
specialize_Hash(L, signed long long,T)
specialize_Hash(L, unsigned char,T)
specialize_Hash(L, unsigned short,T)
specialize_Hash(L, unsigned int,T)
specialize_Hash(L, unsigned long,T)
specialize_Hash(L, unsigned long long,T)
specialize_Hash(L, float,T)
specialize_Hash(L, double,T)
specialize_Hash(L, std::string,T)
%enddef

%define specialize_stdHash(T)
specialize_Hash2(std::map, T)
%enddef

specialize_stdHash(bool)
specialize_stdHash(signed char)
specialize_stdHash(signed short)
specialize_stdHash(signed int)
specialize_stdHash(signed long)
specialize_stdHash(signed long long)
specialize_stdHash(unsigned char)
specialize_stdHash(unsigned short)
specialize_stdHash(unsigned int)
specialize_stdHash(unsigned long)
specialize_stdHash(unsigned long long)
specialize_stdHash(float)
specialize_stdHash(double)
specialize_stdHash(std::string)
