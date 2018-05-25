#pragma once

#include <string>


typedef enum _CompressionType
{
    zlib = 0,
    bzip2,
    none,
    unknown,
} CompressionType;


std::string inline CompressionType_to_string(CompressionType ct)
{
    switch (ct)
    {
        case zlib:
            return "zlib";
        case bzip2:
            return "bzip2";
        case none:
            return "none";
        default:
            break;
    }

    return "unknown";
}
