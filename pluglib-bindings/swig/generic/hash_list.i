/*
 * file:	generic/hash_list.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * generic STL-like hash of lists and list of hashes typemaps
 *
 * $Id$
 */

#ifndef SWIG_SetErrorf
#define SWIG_SetErrorf(msg, ...) Perl_croak(aTHX_ msg, __VA_ARGS__)
#endif

%define specialize_generic_hash_list(HASH, LIST, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)

%typemap(in) HASH< KEY, LIST< VAL > > {
    // convert from perl hash to HASH< KEY, LIST< VAL > 

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
	if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY))) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash<%1,???>.\n", SWIG_TypePrettyName($descriptor(KEY)));
	    SWIG_fail;
	}
	SV *rv = (SV*)hv_iterval(hv,he);
	AV *av;
	if (!rv || !SvROK(rv) || !(av = (AV*)SvRV(rv)) || SvTYPE(av)!=SVt_PVAV )
	    SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to hash<???,ARRAY>.\n");

        I32 len = av_len(av) + 1;
	LIST< VAL > inner;
	for (int j=0; j<len; j++) {
	    VAL val;
	    SV **sv = av_fetch(av, j, 0);
	    if (!sv || !*sv || !VAL_FROM_SV(*sv, &val, sizeof(VAL), $descriptor(VAL))) {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash of array of %1.\n", SWIG_TypePrettyName($descriptor(VAL)));
		SWIG_fail;
	    }
	    inner.push_back(val);
        }
        $1.operator[](key) = inner;
    }
}

%typemap(in) HASH< KEY, LIST< VAL > >& (HASH< KEY, LIST< VAL > > temp), 
    HASH< KEY, LIST< VAL > >* (HASH< KEY, LIST< VAL > > temp)
 {
    // convert from perl hash to HASH< KEY, LIST< VAL > 

    // dereference perl reference
    HV *hv;
    if (!SvROK($input) || !(hv = (HV*)SvRV($input)) || SvTYPE(hv)!=SVt_PVHV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to HASH.\n");

    // iterate
    hv_iterinit(hv);
    while(HE *he=hv_iternext(hv)) {
	KEY key;
	if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY))) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash<%1,???>.\n", SWIG_TypePrettyName($descriptor(KEY)));
	    SWIG_fail;
	}
	SV *rv = (SV*)hv_iterval(hv,he);
	AV *av;
	if (!rv || !SvROK(rv) || !(av = (AV*)SvRV(rv)) || SvTYPE(av)!=SVt_PVAV )
	    SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to hash<???,ARRAY>.\n");

        I32 len = av_len(av) + 1;
	LIST< VAL > inner;
	for (int j=0; j<len; j++) {
	    VAL val;
	    SV **sv = av_fetch(av, j, 0);
	    if (!sv || !*sv || !VAL_FROM_SV(*sv, &val, sizeof(VAL), $descriptor(VAL))) {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to hash of array of %1.\n", SWIG_TypePrettyName($descriptor(VAL)));
		SWIG_fail;
	    }
	    inner.push_back(val);
        }
        temp.operator[](key) = inner;
    }
    $1 = &temp;
}

%typemap(argout) const HASH< KEY, LIST< VAL > >&, const HASH< KEY, LIST< VAL > >* {}

%typemap(argout) HASH< KEY, LIST< VAL > >&, HASH< KEY, LIST< VAL > >* {
    HV *hv = (HV *)SvRV($input);
    hv_clear(hv);
    for (HASH< KEY, LIST< VAL > >::iterator i=$1->begin(); i!=$1->end(); i++) {
	SV *key = newSV(0);
	KEY_TO_SV(key, &(i->first), sizeof(KEY), $descriptor(KEY));
	int len = (i->second).size();
	AV *av = newAV();
	for (LIST< VAL >::iterator j=(i->second).begin(); j!=(i->second).end(); j++) {
	    SV * sv = newSV(0);
	    VAL_TO_SV(sv, &(*j), sizeof(VAL), $descriptor(VAL));
	    av_push(av, sv);
	}
	hv_store_ent(hv, key, newRV_noinc((SV*)av), 0);
    }
}

%typemap(out) HASH< KEY, LIST< VAL > >, HASH< KEY, LIST< VAL > > {
    HV *hv = newHV();
    for (HASH< KEY, LIST< VAL > >::iterator i=$1.begin(); i!=$1.end(); i++) {
	SV *key = newSV(0);
	KEY_TO_SV(key, &(i->first), sizeof(KEY), $descriptor(KEY));
	int len = (i->second).size();
	AV *av = newAV();
	for (LIST< VAL >::iterator j=(i->second).begin(); j!=(i->second).end(); j++) {
	    SV * sv = newSV(0);
	    VAL_TO_SV(sv, &(*j), sizeof(VAL), $descriptor(VAL));
	    av_push(av, sv);
	}
	hv_store_ent(hv, key, newRV_noinc((SV*)av), 0);
    }
    $result = newRV_noinc((SV*)hv);
    sv_2mortal($result);
    argvi++;
}


%typemap(in) LIST< HASH< KEY, VAL > > {
    // convert from perl array to LIST< HASH< KEY, VAL > >

    // dereference perl reference
    AV *av;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY.\n");

    // initialize associative container
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;

    // iterate
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	HV *hv;
	if (!sv || !*sv || !SvROK(*sv) || !(hv = (HV*)SvRV(*sv)) || SvTYPE(hv)!=SVt_PVHV ) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of HASH.\n");
	    SWIG_fail;
	}
	HASH< KEY, VAL > inner;
	while(HE *he=hv_iternext(hv)) {
	    KEY key;
	    VAL val;
	    if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY)) 
		|| !VAL_FROM_SV(hv_iterval(hv,he), &val, sizeof(VAL), $descriptor(VAL)))
	    {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of hash of <%1,%2>.\n", SWIG_TypePrettyName($descriptor(KEY)), SWIG_TypePrettyName($descriptor(VAL)) );
	        SWIG_fail;
	    }
	    inner.operator[](key) = val;
	}
	$1.push_back(inner);
    }
}

%typemap(in) LIST< HASH< KEY, VAL > >& (LIST< HASH< KEY, VAL > > temp), 
    LIST< HASH< KEY, VAL > >* (LIST< HASH<KEY, VAL > > temp)
{
    // convert from perl array to LIST< HASH< KEY, VAL > >

    // dereference perl reference
    AV *av;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY.\n");

    // iterate
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	SV **sv = av_fetch(av, i, 0);
	HV *hv;
	if (!sv || !*sv || !SvROK(*sv) || !(hv = (HV*)SvRV(*sv)) || SvTYPE(hv)!=SVt_PVHV ) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of HASH.\n");
	    SWIG_fail;
	}
	HASH< KEY, VAL > inner;
	while(HE *he=hv_iternext(hv)) {
	    KEY key;
	    VAL val;
	    if (!KEY_FROM_SV(hv_iterkeysv(he), &key, sizeof(KEY), $descriptor(KEY)) 
		|| !VAL_FROM_SV(hv_iterval(hv,he), &val, sizeof(VAL), $descriptor(VAL)))
	    {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of hash of <%1,%2>.\n", SWIG_TypePrettyName($descriptor(KEY)), SWIG_TypePrettyName($descriptor(VAL)) );
	        SWIG_fail;
	    }
	    inner.operator[](key) = val;
	}
	temp.push_back(inner);
    }
    $1 = &temp;
}

%typemap(argout) const LIST < HASH< KEY, VAL > >&, const LIST< HASH< KEY, VAL > >* {}

%typemap(argout) LIST< HASH< KEY, VAL > >&, LIST< HASH< KEY, VAL > >* {
    AV *av = (AV *)SvRV($input);
    av_clear(av);
    for (LIST< HASH< KEY, VAL > >::iterator i=$1->begin(); i!=$1->end(); i++) {
	HV *hv = newHV();
	for (HASH< KEY, VAL >::iterator j=i->begin(); j!=i->end(); j++) {
	    SV *key = newSV(0);
	    KEY_TO_SV(key, &(j->first), sizeof(KEY), $descriptor(KEY));
	    SV *val = newSV(0);
	    VAL_TO_SV(val, &(j->second), sizeof(VAL), $descriptor(VAL));
	    hv_store_ent(hv, key, val, 0);
	}
	av_push(av, newRV_noinc((SV*)hv));
    }
}

%typemap(out) LIST< HASH< KEY, VAL > > {
    AV *av = newAV();
    for (LIST< HASH< KEY, VAL > >::iterator i=$1.begin(); i!=$1.end(); i++) {
	HV *hv = newHV();
	for (HASH< KEY, VAL >::iterator j=i->begin(); j!=i->end(); j++) {
	    SV *key = newSV(0);
	    KEY_TO_SV(key, &(j->first), sizeof(KEY), $descriptor(KEY));
	    SV *val = newSV(0);
	    VAL_TO_SV(val, &(j->second), sizeof(VAL), $descriptor(VAL));
	    hv_store_ent(hv, key, val, 0);
	}
	av_push(av, newRV_noinc((SV*)hv));
    }
    $result = newRV_noinc((SV*)av);
    sv_2mortal($result);
    argvi++;
}

%enddef

