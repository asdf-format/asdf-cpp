#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#ifdef __APPLE__
#include <mach/error.h>
#else
#include <error.h>
#endif

#include <asdf-cpp/block.hpp>
#include <asdf-cpp/compression.hpp>
#include <asdf-cpp/private/compression.hpp>

static const uint8_t no_compression[] = { 0, 0, 0, 0 };


static void * process_compressed_data(
        const uint8_t *data,
        size_t input_size,
        size_t output_size,
        CompressionType compression)
{
        /* Allocate enough memory to contain the uncompressed data */
        uint8_t *output = (uint8_t *) malloc(output_size);
        if (output == nullptr)
        {
            std::string msg = "Unable to allocate memory for uncompressed data";
            throw std::runtime_error(msg + strerror(errno));
        }

        decompress_block(output, output_size, data, input_size, compression);
        return (void *) output;
}

void * process_block_data(const uint8_t *block_data)
{
    const block_header_t *header = (const block_header_t *) block_data;
    const uint8_t *data = block_data + header->total_header_size();
    const size_t data_size = header->get_data_size();
    const size_t used_size = header->get_used_size();
    const size_t comp_field_size = sizeof(header->compression);


    if (memcmp(header->compression, "zlib", comp_field_size) == 0)
    {
#ifdef HAS_ZLIB
        return process_compressed_data(data, used_size, data_size, CompressionType::zlib);
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

void *create_compressed_block(
        size_t *compressed_size,
        const uint8_t *raw_data,
        size_t data_size,
        CompressionType compression)
{

    return nullptr;
}
