/*
 * file:	LiMaL.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * LiMaL typemaps
 *
 * $Id$
 */

%runtime "yast_perlrun.swg"

%include "stdc/types.i"
%include "stl/types.i"


%define specialize_sequence(T, FROM_SV, TO_SV, CHECK_SV)
    specialize_stl_sequence(T, FROM_SV, TO_SV, CHECK_SV)
%enddef

%define specialize_hash(KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_stl_hash(KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
%enddef


%define apply_types(WHAT, ...)
    apply_c_types(WHAT, __VA_ARGS__)
    apply_stl_types(WHAT, __VA_ARGS__)
%enddef

%define apply_keytypes(WHAT, ...)
    apply_c_keytypes(WHAT, __VA_ARGS__)
    apply_stl_keytypes(WHAT, __VA_ARGS__)
%enddef


apply_types(specialize_sequence)
apply_keytypes(apply_types, specialize_hash)
