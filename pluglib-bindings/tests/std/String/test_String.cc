
#include <iostream>
#include <string>

std::string test_string(std::string s)
{
    std::cerr << "D: test_string(" << s << "): length()=" << s.length() << std::endl;
    return s;
}

