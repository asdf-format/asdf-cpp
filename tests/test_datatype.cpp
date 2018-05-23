#include <cstdint>

#include "gtest/gtest.h"

#include <asdf-cpp/asdf.hpp>
#include <asdf-cpp/datatypes.hpp>


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

TEST(DatatypeTest, DtypeMatches)
{
    EXPECT_TRUE(dtype_matches<char>("int8"));
    EXPECT_TRUE(dtype_matches<int>("int32"));
    EXPECT_TRUE(dtype_matches<long>("int64"));

    EXPECT_TRUE(dtype_matches<int8_t>("int8"));
    EXPECT_TRUE(dtype_matches<int16_t>("int16"));
    EXPECT_TRUE(dtype_matches<int32_t>("int32"));
    EXPECT_TRUE(dtype_matches<int64_t>("int64"));

    EXPECT_TRUE(dtype_matches<uint8_t>("uint8"));
    EXPECT_TRUE(dtype_matches<uint16_t>("uint16"));
    EXPECT_TRUE(dtype_matches<uint32_t>("uint32"));
    EXPECT_TRUE(dtype_matches<uint64_t>("uint64"));

    EXPECT_TRUE(dtype_matches<float>("float32"));
    EXPECT_TRUE(dtype_matches<double>("float64"));

    /* Sanity check to make sure these functions don't always return true */
    EXPECT_FALSE(dtype_matches<float>("int8"));
    EXPECT_FALSE(dtype_matches<double>("int32"));
    EXPECT_FALSE(dtype_matches<uint64_t>("int64"));

    EXPECT_FALSE(dtype_matches<uint8_t>("int8"));
    EXPECT_FALSE(dtype_matches<uint16_t>("int16"));
    EXPECT_FALSE(dtype_matches<uint32_t>("int32"));
    EXPECT_FALSE(dtype_matches<uint64_t>("int64"));

    EXPECT_FALSE(dtype_matches<int8_t>("uint8"));
    EXPECT_FALSE(dtype_matches<int16_t>("uint16"));
    EXPECT_FALSE(dtype_matches<int32_t>("uint32"));
    EXPECT_FALSE(dtype_matches<int64_t>("uint64"));

    EXPECT_FALSE(dtype_matches<float>("int8"));
    EXPECT_FALSE(dtype_matches<double>("uint64"));
}
