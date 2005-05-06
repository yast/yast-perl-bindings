/*
 * file:	blocxx/hash.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * BloCxx associative container (Map) typemaps
 *
 * $Id$
 */

%include "generic/hash.i"
%include "generic/hash_list.i"

namespace BLOCXX_NAMESPACE {
    class Map;
    class List;
}

%define specialize_blocxx_hash(KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash(BLOCXX_NAMESPACE::Map, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)

    specialize_generic_hash_list(BLOCXX_NAMESPACE::Map, BLOCXX_NAMESPACE::List, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
%enddef

