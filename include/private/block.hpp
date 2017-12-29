#pragma once

#include <cstdint>


const uint8_t asdf_block_magic[] = {0xd3, 'B', 'L', 'K'};


typedef struct block_header
{
    uint8_t     magic[4];
    uint16_t    header_size;
    uint32_t    flags;
    char        compression[4];
    uint64_t    allocated_size;
    uint64_t    used_size;
    uint64_t    data_size;
    uint16_t    checksum;
} block_header_t;
