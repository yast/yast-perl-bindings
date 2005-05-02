#include <iostream>
#include <iterator>

#include <list>

#include <test.h>

using namespace std;

list<abc> test_listAbc(list<abc> x)
{
    for(list<abc>::iterator i=x.begin(); i!=x.end(); i++) {
        printf("D: a=%i, b=%i, c=%i\n", i->a, i->b, i->c);
	i->a++;
	i->b--;
    }
    abc a = {1,2,3};
    x.push_back(a);
    return x;
}

void test_RlistAbc(list<abc> &x)
{
    for(list<abc>::iterator i=x.begin(); i!=x.end(); i++) {
        printf("D: a=%i, b=%i, c=%i\n", i->a, i->b, i->c);
	i->a++;
	i->b--;
    }
    abc a = {1,2,3};
    x.push_back(a);
}
