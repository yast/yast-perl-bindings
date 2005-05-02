/*
 * file:	stl/types.i
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * STL typemaps and helpers
 *
 * $Id$
 */

%include "stl/string.i"
%include "stl/sequence.i"
%include "stl/hash.i"

%define apply_stl_types(WHAT, ...)
WHAT(__VA_ARGS__, std::string, TO_STD_STRING, FROM_STD_STRING, CHECK)
%enddef

%define apply_stl_keytypes(WHAT, ...)
WHAT(__VA_ARGS__, std::string, TO_STD_STRING, FROM_STD_STRING, CHECK)
%enddef
