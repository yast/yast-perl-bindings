#include <iostream>
#include <iterator>

#include <list>
#include <string>

using namespace std;


list<list<string> > RListListString(list<list<string> > &l)
{
    printf("D: [");
    for(list<list<string> >::iterator i=l.begin(); i!=l.end(); i++) {
	printf("[");
	for(list<string>::iterator j=i->begin(); j!=i->end(); j++) {
	    printf("%s, ", j->c_str());
	    *j+=".ok";
	}
	printf("]");
    }
    printf("]\n");
    return l;
}

list<list<int> > ListListInt(list<list<int> > l)
{
    printf("D: [");
    for(list<list<int> >::iterator i=l.begin(); i!=l.end(); i++) {
	printf("[");
	for(list<int>::iterator j=i->begin(); j!=i->end(); j++) {
	    printf("%i ", *j);
	    *j*=2;
	}
	printf("]");
    }
    printf("]\n");
    return l;
}

void RListListInt(list<list<int> > &l)
{
    printf("D: [");
    for(list<list<int> >::iterator i=l.begin(); i!=l.end(); i++) {
	printf("[");
	for(list<int>::iterator j=i->begin(); j!=i->end(); j++) {
	    printf("%i ", *j);
	    *j*=2;
	}
	printf("]");
    }
    printf("]\n");
}

void PListListInt(list<list<int> > *l)
{
    printf("D: [");
    for(list<list<int> >::iterator i=l->begin(); i!=l->end(); i++) {
	printf("[");
	for(list<int>::iterator j=i->begin(); j!=i->end(); j++) {
	    printf("%i ", *j);
	    *j/=2;
	}
	printf("]");
    }
    printf("]\n");
}

