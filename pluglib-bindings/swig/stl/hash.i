/*
 * file:	stl/hash.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * STL associative container (map) typemaps
 *
 * $Id$
 */


%include "generic/hash.i"

namespace std {
    class map;
}

%define specialize_stl_hash(KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash(std::map, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
%enddef

