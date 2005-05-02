
%module test

%include "LiMaL.i"

specialize_sequence(abc, TO_PACK, FROM_PACK, CHECK)

%include "test.h"
%{
#include "test.h"
%}

