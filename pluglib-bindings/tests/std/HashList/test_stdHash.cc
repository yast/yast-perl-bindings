#include <iostream>
#include <iterator>
#include <cstdio>

#include <string>
#include <map>
#include <list>

using namespace std;

#define L list<int>
#define T map< string, L >
T HashList(T l)
{
    printf("D: {");
    for(T::iterator i = l.begin(); i!=l.end(); i++){
	printf("%s=>[", (i->first).c_str());
	for(L::iterator j = (i->second).begin(); j!=(i->second).end(); j++) {
	    printf("%i, ", *j);
	    (*j)--;
	}
	printf("], ");
    }
    printf("}\n");
    return l;
}

void RHashList(T &l)
{
    printf("D: {");
    for(T::iterator i = l.begin(); i!=l.end(); i++){
	printf("%s=>[", (i->first).c_str());
	for(L::iterator j = (i->second).begin(); j!=(i->second).end(); j++) {
	    printf("%i, ", *j);
	    (*j)++;
	}
	printf("], ");
    }
    printf("}\n");
}

