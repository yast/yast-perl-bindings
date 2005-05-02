/*
 * file:	generic/sequence.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * generic STL-like sequence typemaps
 *
 * $Id$
 */

%define specialize_generic_sequence(L, T, FROM_SV, TO_SV, CHECK_SV)
    specialize_one_level_sequence(L, T, FROM_SV, TO_SV, CHECK_SV)
    specialize_two_level_sequence(L, T, FROM_SV, TO_SV, CHECK_SV)
%enddef


%define specialize_one_level_sequence(L, T, FROM_SV, TO_SV, CHECK_SV)

%typemap(in) L< T >
{
    /* convert from perl SV to L< T > (sequence of T) */
    
    // dereference perl reference
    AV *av;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY.\n");

    // initialize list
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;
    
    // iterate over sequence
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	T val;
	SV **sv = av_fetch(av, i, 0);
	if (!sv || !*sv || !FROM_SV(*sv, &val, sizeof(T), $descriptor(T))) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of %1.\n", SWIG_TypePrettyName($descriptor(T)));
	    SWIG_fail;
	}
	$1.push_back(val);
    }
}

%typemap(in) L< T >& (L< T > temp), L< T >* (L< T > temp)
{
    /* convert from perl SV to reference/pointer to L< T > */

    // dereference perl reference
    AV *av;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY.\n");

    // iterate over sequence
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	T val;
	SV **sv = av_fetch(av, i, 0);
	if (!sv || !*sv || !FROM_SV(*sv, &val, sizeof(T), $descriptor(T))) {
	    SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of %1.\n", SWIG_TypePrettyName($descriptor(T)));
	    SWIG_fail;
	}
	temp.push_back(val);
    }
    $1 = &temp;
}

%typemap(argout) L< T >&, L< T >*
{
    /* update argument from L< T > to perl SV */
    
    // dereference perl pointer
    AV *av = (AV *)SvRV($input);
    
    // clear array
    av_clear(av);
    
    // iterate and fill
    for (L< T >::iterator i=$1->begin(); i!=$1->end(); i++) {
	SV * sv = newSV(0);
	TO_SV(sv, &(*i), sizeof(T), $descriptor(T));
	av_push(av, sv);
    }
}

%typemap(out) L< T >
{
    /* convert from L< T > to perl SV */
    
    unsigned int k = 0;
    int len = $1.size();
    SV **svs = new SV*[len];
    for (L< T >::iterator i=$1.begin(); i!=$1.end(); i++) {
	svs[k] = sv_newmortal();
        TO_SV(svs[k++], &(*i), sizeof(T), $descriptor(T));
    }
    $result = newRV_noinc((SV*)av_make(len, svs));
    sv_2mortal($result);
    delete[] svs;
    argvi++;
}
%enddef



%define specialize_two_level_sequence(L, T, FROM_SV, TO_SV, CHECK_SV)

%typemap(in) L< L< T > >
{
    /* convert from perl SV to L< L< T > > (sequence of sequence of T) */
    
    // dereference perl reference
    AV *av, *av1;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY of array.\n");

    // initialize list
    $1_ltype workaround_SwigValueWrapper;
    $1 = workaround_SwigValueWrapper;

    // iterate over sequence
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	SV **rv = av_fetch(av, i, 0);
	if (!rv || !SvROK(*rv) || !(av1 = (AV*)SvRV(*rv)) || SvTYPE(av1)!=SVt_PVAV )
	    SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to array of ARRAY.\n");
        I32 len1 = av_len(av1) + 1;
	L< T > inner;
	for (int j=0; j<len1; j++) {
	    T val;
	    SV **sv1 = av_fetch(av1, j, 0);
	    if (!sv1 || !*sv1 || !FROM_SV(*sv1, &val, sizeof(T), $descriptor(T))) {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of array of %1.\n", SWIG_TypePrettyName($descriptor(T)));
		SWIG_fail;
	    }
	    inner.push_back(val);
        }
        $1.push_back(inner);
    }
}

%typemap(in) L< L< T > >& (L< L< T > > temp), L< L< T > >* (L< L< T > > temp)
{
    /* convert from perl SV to reference/pointer to L< L< T > > (sequence of sequence of T) */
    
    // dereference perl reference
    AV *av, *av1;
    if (!SvROK($input) || !(av = (AV*)SvRV($input)) || SvTYPE(av)!=SVt_PVAV )
	SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to ARRAY of array.\n");

    // iterate over sequence
    I32 len = av_len(av) + 1;
    for (int i=0; i<len; i++) {
	SV **rv = av_fetch(av, i, 0);
	if (!rv || !SvROK(*rv) || !(av1 = (AV*)SvRV(*rv)) || SvTYPE(av1)!=SVt_PVAV )
	    SWIG_croak("Type error in argument $argnum of $symname. Expected a reference to array of ARRAY.\n");
        I32 len1 = av_len(av1) + 1;
	L< T > inner;
	for (int j=0; j<len1; j++) {
	    T val;
	    SV **sv1 = av_fetch(av1, j, 0);
	    if (!sv1 || !*sv1 || !FROM_SV(*sv1, &val, sizeof(T), $descriptor(T))) {
		SWIG_SetErrorf("Type error in argument $argnum of $symname. Expected a reference to array of array of %1.\n", SWIG_TypePrettyName($descriptor(T)));
		SWIG_fail;
	    }
	    inner.push_back(val);
        }
        temp.push_back(inner);
    }
    $1 = &temp;
}

%typemap(argout) L< L< T > >&, L< L< T > >*
{
    /* update argument from L< < T > > to perl SV */
    
    // dereference perl reference
    AV *av = (AV *)SvRV($input);
    
    // clear perl array
    av_clear(av);
    
    // iterate and fill
    for (L< L< T > >::iterator i=$1->begin(); i!=$1->end(); i++) {
	int len1 = i->size();
	AV *av1 = newAV();
	for (L< T >::iterator j=i->begin(); j!=i->end(); j++) {
	    SV * sv = newSV(0);
	    TO_SV(sv, &(*j), sizeof(T), $descriptor(T));
	    av_push(av1, sv);
	}
	av_push(av, newRV_noinc((SV*)av1));
    }
}

%typemap(out) L< L< T > >
{
    /* convert from L< L< T > > to perl SV */
    
    AV *av = newAV();
    for (L< L< T > >::iterator i=$1.begin(); i!=$1.end(); i++) {
	int len = i->size();
	unsigned k = 0;
	SV **svs = new SV*[len];
        for (L< T >::iterator j=i->begin(); j!=i->end(); j++) {
	    svs[k] = sv_newmortal();
    	    TO_SV(svs[k++], &(*j), sizeof(T), $descriptor(T));
	}
	av_push(av, newRV_noinc((SV*)av_make(len, svs)));
	delete[] svs;
    }
    $result = newRV_noinc((SV*)av);
    sv_2mortal($result);
    argvi++;
}
%enddef
