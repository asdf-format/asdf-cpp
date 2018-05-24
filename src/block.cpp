#include <iostream>
#include <cstdint>
#include <cstring>

#include <asdf-cpp/block.hpp>
#include <asdf-cpp/private/compression.hpp>


void * process_block_data(const uint8_t *block_data)
{
    const block_header_t *header = (const block_header_t *) block_data;
    const uint8_t *data = block_data + header->total_header_size();

    size_t data_size = header->get_used_size();
    uint8_t *data_copy = (uint8_t *) malloc(data_size);
    memcpy(data_copy, data, data_size);

    return (void *) data_copy;
}
