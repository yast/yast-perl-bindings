// BloCxx::List

%include "BloCxx_Types.i"
%include "BloCxx_Bool.i"
%include "BloCxx_String.i"


%include "std_Common.i"
%include "BloCxx_Common.i"

%include "std_List.i"

specialize_stdList(BLOCXX_NAMESPACE::String)

%define specialize_blocxxList(T)
specialize_List(BLOCXX_NAMESPACE::List,T)
%enddef

specialize_blocxxList(bool)

specialize_blocxxList(signed char)
specialize_blocxxList(signed short)
specialize_blocxxList(signed int)
specialize_blocxxList(signed long)
specialize_blocxxList(signed long long)

specialize_blocxxList(unsigned char)
specialize_blocxxList(unsigned short)
specialize_blocxxList(unsigned int)
specialize_blocxxList(unsigned long)
specialize_blocxxList(unsigned long long)

specialize_blocxxList(float)
specialize_blocxxList(double)

specialize_blocxxList(std::string)
specialize_blocxxList(BLOCXX_NAMESPACE::String)

