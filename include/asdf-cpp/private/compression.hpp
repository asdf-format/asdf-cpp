#pragma once

#include <cstdlib>
#include <cstdint>

#include "../compression.hpp"


int compress_block(
        uint8_t *output,
        const uint8_t *input,
        size_t input_size,
        size_t *output_size,
        CompressionType compression);

int decompress_block(
        uint8_t *output,
        const uint8_t *input,
        size_t input_size,
        size_t *output_size,
        CompressionType compression);
