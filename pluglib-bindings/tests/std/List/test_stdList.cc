#include <iostream>
#include <iterator>

#include <list>

using namespace std;


list<int> test_listInt(list<int> l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_listInt([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

list<string> test_listString(list<string> l)
{
    ostream_iterator<string> out(cerr, ", ");
    cerr << "D: test_listString([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

void test_listRefInt(list<int> &l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_listRefInt([ ";
    copy(l.begin(), l.end(), out);
    cerr << "]) ->";
    
    std::list<int>::iterator i;
    for (i=l.begin(); i!=l.end(); i++) {
	(*i)*=2;
    }
    
    cerr << "([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
}

void test_listPInt(list<int> *l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_listPInt([ ";
    copy(l->begin(), l->end(), out);
    cerr << "]) ->";
    
    std::list<int>::iterator i;
    for (i=l->begin(); i!=l->end(); i++) {
	(*i)/=2;
    }
    
    cerr << "([ ";
    copy(l->begin(), l->end(), out);
    cerr << "])" << endl;
}

