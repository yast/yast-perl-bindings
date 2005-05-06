#include <iostream>
#include <iterator>

#include <blocxx/String.hpp>
#include <blocxx/Map.hpp>
#include <blocxx/List.hpp>
#include <blocxx/Types.hpp>


using namespace BLOCXX_NAMESPACE;
using namespace std;

#define L List<Int32>
#define H Map<String, L >
void HashList(H l)
{
    printf("D: {");
    for(H::iterator i = l.begin(); i!=l.end(); i++){
	printf("%s=>[", (i->first).c_str());
	for(L::iterator j = (i->second).begin(); j!=(i->second).end(); j++){
	    printf("%i, ", *j);
	}
	printf("], ");
    }
    printf("}\n");
}

