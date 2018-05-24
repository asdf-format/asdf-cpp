#include <string>
#include <stdexcept>

#include <asdf-cpp/compression.hpp>
#include <asdf-cpp/private/compression.hpp>


#ifdef HAS_ZLIB
#include <zlib.h>

static int gzip_compress(void);
static int gzip_decompress(
    uint8_t *output,
    size_t output_size,
    const uint8_t *input,
    size_t input_size);
#endif

#ifdef HAS_BZIP2
static int bzip2_compress(void);
static int bzip2_decompress(void);
#endif


#define CHECK_ZLIB_ERR(err, msg) {                  \
    if (err != Z_OK)                                \
    {                                               \
        std::string output = "zlib error: ";        \
        throw std::runtime_error(output + msg);     \
    }                                               \
}


int compress_block(
        uint8_t *output,
        size_t *output_size,
        const uint8_t *input,
        size_t input_size,
        CompressionType compression)
{
    std::string msg;

    switch (compression)
    {
        case gzip:
#if HAS_ZLIB
            return gzip_compress();
#else
            msg = "Can't compress block: gzip library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        case bzip2:
#if HAS_BZIP2
            return bzip2_compress();
#else
            msg = "Can't compress block: bzip2 library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        default:
            break;
    }

    return 0;
}

int decompress_block(
        uint8_t *output,
        size_t output_size,
        const uint8_t *input,
        size_t input_size,
        CompressionType compression)
{
    std::string msg;

    switch (compression)
    {
        case gzip:
#if HAS_ZLIB
            return gzip_decompress(output, output_size, input, input_size);
#else
            msg = "Can't decompress block: gzip library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        case bzip2:
#if HAS_BZIP2
            return bzip2_decompress();
#else
            msg = "Can't decompress block: bzip2 library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        default:
            break;

    }

    return 0;
}

#ifdef HAS_ZLIB
static int gzip_compress(void)
{
    return 0;
}

static int gzip_decompress(
        uint8_t *output,
        size_t output_size,
        const uint8_t *input,
        size_t input_size)
{
    int err;
    z_stream stream;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.next_in = (Bytef *) input;
    stream.avail_in = input_size;
    stream.next_out = (Bytef *) output;
    stream.avail_out = output_size;

    err = inflateInit(&stream);
    CHECK_ZLIB_ERR(err, "inflateInit");

    err = inflate(&stream, Z_NO_FLUSH);
    if (err != Z_STREAM_END)
    {
        CHECK_ZLIB_ERR(err, "inflate");
    }

    err = inflateEnd(&stream);
    CHECK_ZLIB_ERR(err, "inflateEnd");

    return 0;
}
#endif

#ifdef HAS_BZIP2
static int bzip2_compress(void)
{

    return 0;
}

static int bzip2_decompress(void)
{

    return 0;
}
#endif
