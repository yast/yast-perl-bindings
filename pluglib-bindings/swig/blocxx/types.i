/*
 * file:	blocxx/types.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * BloCxx basic type typemaps and helpers
 *
 * $Id$
 */

%include <blocxx/BLOCXX_config.h>
%include <blocxx/Types.hpp>

%apply bool { BLOCXX_NAMESPACE::Bool }

%include "blocxx/string.i"
%include "blocxx/sequence.i"
%include "blocxx/hash.i"

%define apply_blocxx_types(WHAT, ...)
    WHAT(__VA_ARGS__, BLOCXX_NAMESPACE::Bool, TO_BOOL, FROM_BOOL, CHECK)
    WHAT(__VA_ARGS__, BLOCXX_NAMESPACE::String, TO_BLOCXX_STRING, FROM_BLOCXX_STRING, CHECK)
%enddef

%define apply_blocxx_keytypes(WHAT, ...)
    WHAT(__VA_ARGS__, BLOCXX_NAMESPACE::String, TO_BLOCXX_STRING, FROM_BLOCXX_STRING, CHECK)
%enddef
