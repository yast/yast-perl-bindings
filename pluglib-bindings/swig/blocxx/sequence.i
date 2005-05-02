/*
 * file:	blocxx/sequence.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * BloCxx sequences (list) typemaps
 *
 * $Id$
 */

%include "generic/sequence.i"

namespace BLOCXX_NAMESPACE {
    class List;
}

%define specialize_blocxx_sequence(T, FROM_SV, TO_SV, CHECK_SV)
    specialize_generic_sequence(BLOCXX_NAMESPACE::List, T, FROM_SV, TO_SV, CHECK_SV)
%enddef
