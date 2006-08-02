/*
 * file:	generic/hash.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * generic STL-like associative containers typemaps
 *
 * $Id$
 */

#ifndef SWIG_SetErrorf
#define SWIG_SetErrorf(msg, ...) Perl_croak(aTHX_ msg, __VA_ARGS__)
#endif

%define specialize_generic_hash(HASH, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV,
    VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)

%typemap(in) HASH< KEY, VAL > {
    // convert from perl hash to HASH< KEY, VAL >

    // dereference perl reference
    HV *hv;
    if (!SvROK($input) || !(hv = (HV*)SvRV($input)) || SvTYPE(hv)!=SVt_PVHV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to HASH.\n");

    // initialize associative container
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;

    // iterate
    hv_iterinit(hv);
    while(HE *he=hv_iternext(hv)) {
	KEY key;
	VAL val;
	if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY)) 
	    || !VAL_FROM_SV(hv_iterval(hv,he), &val, sizeof(VAL), $descriptor(VAL)))
	{
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash of <%1,%2>.\n", SWIG_TypePrettyName($descriptor(KEY)), SWIG_TypePrettyName($descriptor(VAL)) );
	    SWIG_fail;
	}
	$1.operator[](key) = val;
    }
}

%typemap(in) HASH< KEY, VAL >& (HASH< KEY, VAL > temp), HASH< KEY, VAL >* (HASH< KEY, VAL > temp) {
    // convert from perl hash to reference/pointer to HASH< KEY, VAL >

    // dereference perl reference
    HV *hv;
    if (!SvROK($input) || !(hv = (HV*)SvRV($input)) || SvTYPE(hv)!=SVt_PVHV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to HASH.\n");

    // iterate
    hv_iterinit(hv);
    while(HE *he=hv_iternext(hv)) {
	KEY key;
	VAL val;
	if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY)) 
	    || !VAL_FROM_SV(hv_iterval(hv,he), &val, sizeof(VAL), $descriptor(VAL)))
	{
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash of <%1,%2>.\n", SWIG_TypePrettyName($descriptor(KEY)), SWIG_TypePrettyName($descriptor(VAL)) );
	    SWIG_fail;
	}
	temp.operator[](key) = val;
    }
    $1 = &temp;
}

%typemap(argout) HASH< KEY, VAL >&, HASH< KEY, VAL >* {
    HV *hv = (HV *)SvRV($input);
    hv_clear(hv);
    for (HASH< KEY, VAL >::const_iterator i=$1->begin(); i!=$1->end(); i++) {
	SV *key = newSV(0);
	KEY_TO_SV(key, &(i->first), sizeof(KEY), $descriptor(KEY));
	SV *val = newSV(0);
	VAL_TO_SV(val, &(i->second), sizeof(VAL), $descriptor(VAL));
	hv_store_ent(hv, key, val, 0);
    }
}

%typemap(out) HASH< KEY, VAL > {
    HV *hv = newHV();
    for (HASH< KEY, VAL >::const_iterator i=$1.begin(); i!=$1.end(); i++) {
	SV *key = newSV(0);
	KEY_TO_SV(key, &(i->first), sizeof(KEY), $descriptor(KEY));
	SV *val = newSV(0);
	VAL_TO_SV(val, &(i->second), sizeof(VAL), $descriptor(VAL));
	hv_store_ent(hv, key, val, 0);
    }
    $result = newRV_noinc((SV*)hv);
    sv_2mortal($result);
    argvi++;
}

%enddef

