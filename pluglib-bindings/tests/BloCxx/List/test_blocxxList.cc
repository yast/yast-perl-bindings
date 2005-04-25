#include <iostream>
#include <iterator>

#include <blocxx/String.hpp>
#include <blocxx/List.hpp>
#include <blocxx/Types.hpp>


using namespace BLOCXX_NAMESPACE;
using namespace std;

List<Int32> test_listInt32(List<Int32> l)
{
    ostream_iterator<Int32> out(cerr, " ");
    cerr << "D: test_listInt([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

List<String> test_listString(List<String> l)
{
    ostream_iterator<String> out(cerr, " ");
    cerr << "D: test_listString([ ";
    copy(l.begin(), l.end(), out);
    cerr << "])" << endl;
    return l;
}

