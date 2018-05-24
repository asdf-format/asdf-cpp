#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdint>

#include "../compression.hpp"


int compress_and_write_block(
        std::ostream &stream,
        size_t *output_size,
        const uint8_t *input,
        size_t input_size,
        CompressionType compression);

int decompress_block(
        uint8_t *output,
        size_t output_size,
        const uint8_t *input,
        size_t input_size,
        CompressionType compression);
