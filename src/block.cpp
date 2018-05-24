#include <string>
#include <cstdint>
#include <cstring>

#include <asdf-cpp/block.hpp>
#include <asdf-cpp/private/compression.hpp>

static const uint8_t no_compression[] = { 0, 0, 0, 0 };


void * process_block_data(const uint8_t *block_data)
{
    const block_header_t *header = (const block_header_t *) block_data;
    const uint8_t *data = block_data + header->total_header_size();
    const size_t data_size = header->get_used_size();
    const size_t comp_field_size = sizeof(header->compression);


    if (memcmp(header->compression, "zlib", comp_field_size) == 0)
    {
#ifdef HAS_ZLIB

#else
        std::string msg("Can't read zlib block: zlib is not installed");
        throw std::runtime_error(msg);
#endif
    }
    else if (memcmp(header->compression, "bzp2", comp_field_size) == 0)
    {
#ifdef HAS_BZIP2

#else
        std::string msg("Can't read bzp2 block: bzip is not installed");
        throw std::runtime_error(msg);
#endif
    }
    else if (memcmp(header->compression, no_compression, comp_field_size) != 0)
    {
        std::string msg("Unrecognized or unsupported encryption algorithm");
        throw std::runtime_error(msg);
    }

    /* Handle the uncompressed case: simply make a copy of the data block */
    uint8_t *data_copy = (uint8_t *) malloc(data_size);
    memcpy(data_copy, data, data_size);

    return (void *) data_copy;
}
