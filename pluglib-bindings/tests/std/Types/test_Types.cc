
#include <iostream>

extern "C" {
#include <stdint.h>
}

#define TEST(N, T) T N(T x) { std::cerr << "D: test_" #T "(" << x << ")" << std::endl; return x; }


TEST(test_bool, bool)

TEST(test_int, int)


#undef TEST

