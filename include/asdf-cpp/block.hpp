#pragma once

#include <iostream>
#include <assert.h>
#include <cstdint>
#include <cstring>

#include "compression.hpp"


const uint8_t asdf_block_magic[] = {0xd3, 'B', 'L', 'K'};
const uint8_t zlib_compression[] = {'z', 'l', 'i', 'b'};
const uint8_t bzp2_compression[] = {'b', 'z', 'p', '2'};
const uint8_t no_compression[] = { 0, 0, 0, 0 };


#pragma pack(push, 1)
typedef struct block_header
{
    uint8_t     magic[4];
    uint8_t     header_size[2];
    uint32_t    flags;
    char        compression[4];
    uint8_t     allocated_size[8];
    uint8_t     used_size[8];
    uint8_t     data_size[8];
    uint8_t     checksum[16];

    block_header()
    {
        flags = 0;
        memcpy(magic, asdf_block_magic, sizeof(magic));
        memset(compression, 0, sizeof(compression));
        memset(checksum, 0, sizeof(checksum));
    }

    static inline void unpack_u64be(uint8_t dest[8], uint64_t source)
    {
        dest[0] = (source >> 56) & 0xff;
        dest[1] = (source >> 48) & 0xff;
        dest[2] = (source >> 40) & 0xff;
        dest[3] = (source >> 32) & 0xff;
        dest[4] = (source >> 24) & 0xff;
        dest[5] = (source >> 16) & 0xff;
        dest[6] = (source >> 8) & 0xff;
        dest[7] = source & 0xff;
    }

    static inline uint64_t pack_u64be(const uint8_t source[8])
    {
        uint64_t packed =
            ((uint64_t) source[0] << 56) |
            ((uint64_t) source[1] << 48) |
            ((uint64_t) source[2] << 40) |
            ((uint64_t) source[3] << 32) |
            ((uint64_t) source[4] << 24) |
            ((uint64_t) source[5] << 16) |
            ((uint64_t) source[6] << 8) | source[7];

        return packed;
    }

    void set_header_size(uint16_t size)
    {
        header_size[0] = (size >> 8) & 0xff;
        header_size[1] = size & 0xff;
    }

    void set_allocated_size(uint64_t size)
    {
        unpack_u64be(allocated_size, size);
    }

    void set_used_size(uint64_t size)
    {
        unpack_u64be(used_size, size);
    }

    void set_data_size(uint64_t size)
    {
        unpack_u64be(data_size, size);
    }

    void set_compression(CompressionType ct)
    {
        switch (ct)
        {
            case zlib:
                memcpy(compression, zlib_compression, sizeof(compression));
                break;
            case bzip2:
                memcpy(compression, bzp2_compression, sizeof(compression));
                break;
            default:
                memcpy(compression, no_compression, sizeof(compression));
                break;
        }
    }

    /* Returns the value of the header size field */
    uint16_t get_header_size() const
    {
        return (header_size[0] << 8) | header_size[1];
    }

    /* Returns the actual size of the entire header */
    size_t total_header_size() const
    {
        return get_header_size() + sizeof(this->magic) + sizeof(this->header_size);
    }

    uint64_t get_allocated_size() const
    {
        return pack_u64be(allocated_size);
    }

    uint64_t get_used_size() const
    {
        return pack_u64be(used_size);
    }

    uint64_t get_data_size() const
    {
        return pack_u64be(data_size);
    }

    CompressionType get_compression() const
    {
        if (memcmp(compression, zlib_compression, sizeof(compression)) == 0)
        {
            return zlib;
        }
        else if (memcmp(compression, bzp2_compression, sizeof(compression)) == 0)
        {
            return bzip2;
        }
        else if (memcmp(compression, no_compression, sizeof(compression)) == 0)
        {
            return none;
        }

        return unknown;
    }

} block_header_t;
#pragma pack(pop)

const size_t header_size = sizeof(block_header_t) -
    (sizeof(((block_header_t *) 0)->magic) +
     sizeof(((block_header_t *) 0)->header_size));

static_assert(sizeof(block_header_t) == 0x36, "Invalid block header size");


void *process_block_data(const uint8_t *block_data);
void write_compressed_block(
        std::ostream &stream,
        size_t *compressed_size,
        const uint8_t *raw_data,
        size_t data_size,
        CompressionType compression);
