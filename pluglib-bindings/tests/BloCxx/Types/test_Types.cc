
#include <iostream>

#include <blocxx/Types.hpp>

using namespace BLOCXX_NAMESPACE;

#define TEST(N, T) T N(T x) { std::cerr << "D: test_" #T "(" << x << ")" << std::endl; return x; }

TEST(test_Int8, Int8)
TEST(test_UInt8, UInt8)

TEST(test_Int16, Int16)
TEST(test_UInt16, UInt16)

TEST(test_Int32, Int32)
TEST(test_UInt32, UInt32)

TEST(test_Int64, Int64)
TEST(test_UInt64, UInt64)

TEST(test_Real32, Real32)
TEST(test_Real64, Real64)

#undef TEST

