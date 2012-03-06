#include <iostream>
#include <iterator>
#include <cstdio>

#include <blocxx/String.hpp>
#include <blocxx/Map.hpp>
#include <blocxx/Types.hpp>


using namespace BLOCXX_NAMESPACE;
using namespace std;

void StringInt32(Map<String,Int32> l)
{
    printf("D: {");
    for(Map<String,Int32>::iterator i = l.begin(); i!=l.end(); i++){
	printf("%s=>%i, ", (i->first).c_str(), i->second);
    }
    printf("}\n");
}

