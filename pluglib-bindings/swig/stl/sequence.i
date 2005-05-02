/*
 * file:	stl/sequence.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * STL sequences (vector, deque, list, and slist) typemaps
 *
 * $Id$
 */


%include "generic/sequence.i"

namespace std {
    class deque;
    class list;
    class vector;
    class slist;
}

%define specialize_stl_sequence(T, FROM_SV, TO_SV, CHECK_SV)
    specialize_generic_sequence(std::list, T, FROM_SV, TO_SV, CHECK_SV)
    specialize_generic_sequence(std::deque, T, FROM_SV, TO_SV, CHECK_SV)
    specialize_generic_sequence(std::vector, T, FROM_SV, TO_SV, CHECK_SV)
    specialize_generic_sequence(std::slist, T, FROM_SV, TO_SV, CHECK_SV)
%enddef

