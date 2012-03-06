#include <iostream>
#include <iterator>
#include <cstdio>

#include <string>
#include <map>
#include <list>

using namespace std;

#define H map< string, int >
#define L list< H >
L ListHash(L l)
{
    printf("D: [");
    for(L::iterator i = l.begin(); i!=l.end(); i++){
	printf("{");
	for(H::iterator j = i->begin(); j!=i->end(); j++) {
	    printf("%s=>%i, ", (j->first).c_str(), j->second);
	    (j->second)--;
	}
	printf("}, ");
    }
    printf("]\n");
    return l;
}

void RListHash(L &l)
{
    printf("D: [");
    for(L::iterator i = l.begin(); i!=l.end(); i++){
	printf("{");
	for(H::iterator j = i->begin(); j!=i->end(); j++) {
	    printf("%s=>%i, ", (j->first).c_str(), j->second);
	    (j->second)++;
	}
	printf("}, ");
    }
    printf("]\n");
}

