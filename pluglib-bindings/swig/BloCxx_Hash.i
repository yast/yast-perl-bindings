// BloCxx::Hash
// author: Martin Lazar <mlazar@suse.cz>

%include "BloCxx_Types.i"
%include "BloCxx_Bool.i"
%include "BloCxx_String.i"


%include "std_Common.i"
%include "BloCxx_Common.i"

%include "std_Hash.i"

specialize_stdHash(BLOCXX_NAMESPACE::String)

%define specialize_blocxxHash(T)
specialize_Hash2(BLOCXX_NAMESPACE::Map, T)
specialize_Hash(BLOCXX_NAMESPACE::Map, BLOCXX_NAMESPACE::String, T)
%enddef

specialize_blocxxHash(bool)

specialize_blocxxHash(signed char)
specialize_blocxxHash(signed short)
specialize_blocxxHash(signed int)
specialize_blocxxHash(signed long)
specialize_blocxxHash(signed long long)

specialize_blocxxHash(unsigned char)
specialize_blocxxHash(unsigned short)
specialize_blocxxHash(unsigned int)
specialize_blocxxHash(unsigned long)
specialize_blocxxHash(unsigned long long)

specialize_blocxxHash(float)
specialize_blocxxHash(double)

specialize_blocxxHash(std::string)
specialize_blocxxHash(BLOCXX_NAMESPACE::String)

