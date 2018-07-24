#pragma once

#include <cstdlib>
#include <type_traits>

#define bswap_16(value) \
    ((((value) & 0xff) << 8) | ((value) >> 8))

#define bswap_32(value) \
    (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
    (uint32_t)bswap_16((uint16_t)((value) >> 16)))

#define bswap_64(value) \
    (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) << 32) | \
    (uint64_t)bswap_32((uint32_t)((value) >> 32)))


template <typename T>
typename std::enable_if<(sizeof(T) == 4), void>::type
byteswap_data(T *data, size_t count)
{
    /* This loop could be parallelized with OpenMP */
    for (size_t i = 0; i < count; i++)
    {
        data[i] = bswap_32(data[i]);
    }
}
