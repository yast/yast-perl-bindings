/*
 * file:	stl/hash.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * STL associative container (map) typemaps
 *
 * $Id$
 */


%include "generic/hash.i"
%include "generic/hash_list.i"

namespace std {
    class map;
    class deque;
    class list;
    class vector;
    class slist;
}

%define specialize_stl_hash(KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash(std::map, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)

    specialize_generic_hash_list(std::map, std::list, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash_list(std::map, std::deque, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash_list(std::map, std::vector, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
    specialize_generic_hash_list(std::map, std::slist, KEY, KEY_FROM_SV, KEY_TO_SV, KEY_CHECK_SV, VAL, VAL_FROM_SV, VAL_TO_SV, VAL_CHECK_SV)
%enddef
