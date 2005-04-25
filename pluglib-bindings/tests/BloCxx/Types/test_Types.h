
#include <blocxx/Types.hpp>

#define TEST(N,T) BLOCXX_NAMESPACE::T N(BLOCXX_NAMESPACE::T x);

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
