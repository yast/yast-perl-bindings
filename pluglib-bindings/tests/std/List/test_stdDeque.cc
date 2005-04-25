#include <iostream>
#include <iterator>

#include <deque>

using namespace std;


deque<int> test_Int(deque<int> l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_Int([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

deque<string> test_String(deque<string> l)
{
    ostream_iterator<string> out(cerr, ", ");
    cerr << "D: test_String([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

void test_RefInt(deque<int> &l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_RefInt([ ";
    copy(l.begin(), l.end(), out);
    cerr << "]) ->";
    
    std::deque<int>::iterator i;
    for (i=l.begin(); i!=l.end(); i++) {
	(*i)*=2;
    }
    
    cerr << "([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
}

void test_PInt(deque<int> *l)
{
    ostream_iterator<int> out(cerr, ", ");
    cerr << "D: test_PInt([ ";
    copy(l->begin(), l->end(), out);
    cerr << "]) ->";
    
    std::deque<int>::iterator i;
    for (i=l->begin(); i!=l->end(); i++) {
	(*i)/=2;
    }
    
    cerr << "([ ";
    copy(l->begin(), l->end(), out);
    cerr << "])" << endl;
}

