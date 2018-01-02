#pragma once

#include <assert.h>
#include <cstdint>


const uint8_t asdf_block_magic[] = {0xd3, 'B', 'L', 'K'};


#pragma pack(push, 1)
typedef struct block_header
{
    uint8_t     magic[4];
    uint16_t    header_size;
    uint32_t    flags;
    char        compression[4];
    uint64_t    allocated_size;
    uint64_t    used_size;
    uint64_t    data_size;
    uint8_t     checksum[16];
} block_header_t;
#pragma pack(pop)

static_assert(sizeof(block_header_t) == 0x36, "Invalid block header size");
