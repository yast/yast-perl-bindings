
#include <iostream>
#include <string>

#include <blocxx/String.hpp>

using namespace BLOCXX_NAMESPACE;

String test_String(String s)
{
    std::cerr << "D: test_String(" << s << "): length()=" << s.length() <<
	 ", UTF8Length=" << s.UTF8Length() << std::endl;
    return s;
}

