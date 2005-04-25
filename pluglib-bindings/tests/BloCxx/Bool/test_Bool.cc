
#include <iostream>

#include <blocxx/Bool.hpp>

using namespace BLOCXX_NAMESPACE;

Bool test_blocxxBool(Bool s)
{
    std::cerr << "D: test_bocxxBool(" << s << ")" << std::endl;
    return !s;
}

bool test_bool(bool s)
{
    std::cerr << "D: test_bool(" << s << ")" << std::endl;
    return !s;
}

