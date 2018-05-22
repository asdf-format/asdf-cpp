#include <cstdint>

#include <asdf.hpp>
#include "gtest/gtest.h"

#include <private/datatypes.hpp>


TEST(DatatypeTest, DtypeToString)
{
    EXPECT_EQ(dtype_to_string<char>(),      "int8");
    EXPECT_EQ(dtype_to_string<int>(),       "int32");
    EXPECT_EQ(dtype_to_string<long>(),      "int64");

    EXPECT_EQ(dtype_to_string<int8_t>(),    "int8");
    EXPECT_EQ(dtype_to_string<int16_t>(),   "int16");
    EXPECT_EQ(dtype_to_string<int32_t>(),   "int32");
    EXPECT_EQ(dtype_to_string<int64_t>(),   "int64");

    EXPECT_EQ(dtype_to_string<uint8_t>(),   "uint8");
    EXPECT_EQ(dtype_to_string<uint16_t>(),  "uint16");
    EXPECT_EQ(dtype_to_string<uint32_t>(),  "uint32");
    EXPECT_EQ(dtype_to_string<uint64_t>(),  "uint64");

    EXPECT_EQ(dtype_to_string<float>(),     "float32");
    EXPECT_EQ(dtype_to_string<double>(),    "float64");
}
