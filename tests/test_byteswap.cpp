#include <cstdint>

#include "gtest/gtest.h"

#include "asdf-cpp/byteswap.hpp"


TEST(ByteswapTest, SWAPU16)
{
    uint16_t data[100];

    for (int i = 0; i < 100; i++)
    {
        data[i] = 0xf00d;
    }

    byteswap_data(data, 100);

    for (int i = 0; i < 100; i++)
    {
        ASSERT_EQ(data[i], 0x0df0);
    }
}

TEST(ByteswapTest, SwapU32)
{
    uint32_t data[100];

    for (int i = 0; i < 100; i++)
    {
        data[i] = 0xdeadbeef;
    }

    byteswap_data(data, 100);

    for (int i = 0; i < 100; i++)
    {
        ASSERT_EQ(data[i], 0xefbeadde);
    }
}

TEST(ByteswapTest, SwapU64)
{
    uint64_t data[100];

    for (int i = 0; i < 100; i++)
    {
        data[i] = 0xf00dfacedeadbeefUL;
    }

    byteswap_data(data, 100);

    for (int i = 0; i < 100; i++)
    {
        ASSERT_EQ(data[i], 0xefbeaddecefa0df0);
    }
}
