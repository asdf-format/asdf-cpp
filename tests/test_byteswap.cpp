#include <cstdint>

#include "gtest/gtest.h"

#include "asdf-cpp/byteswap.hpp"


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
