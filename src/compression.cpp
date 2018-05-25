#include <string>
#include <cstdint>
#include <stdexcept>

#include <asdf-cpp/compression.hpp>
#include <asdf-cpp/private/compression.hpp>

/*
 * This is fairly arbitrary right now and eventually could be configured to
 * balance performance and memory usage.
 */
#define OUTPUT_BUFF_SIZE    (1ul << 16)
#define PACK_U64(HI, LO)    (((uint64_t) HI << 32) | LO)


#ifdef HAS_ZLIB
#include <zlib.h>

static int zlib_compress(
    std::ostream &stream,
    size_t *output_size,
    const uint8_t *input,
    size_t input_size);

static int zlib_decompress(
    uint8_t *output,
    size_t output_size,
    const uint8_t *input,
    size_t input_size);
#endif

#ifdef HAS_BZIP2
#include <bzlib.h>

static int bzip2_compress(
    std::ostream &stream,
    size_t *output_size,
    const uint8_t *input,
    size_t input_size);

static int bzip2_decompress(
    uint8_t *output,
    size_t output_size,
    const uint8_t *input,
    size_t input_size);
#endif


#define CHECK_ZLIB_ERR(err, msg) {                  \
    if (err != Z_OK)                                \
    {                                               \
        std::string output = "zlib error: ";        \
        throw std::runtime_error(output + msg);     \
    }                                               \
}

#define CHECK_BZIP_ERR(err, msg) {                  \
    if (err != BZ_OK)                               \
    {                                               \
        std::string output = "bzip error: ";        \
        throw std::runtime_error(output + msg);     \
    }                                               \
}

int compress_and_write_block(
        std::ostream &stream,
        size_t *output_size,
        const uint8_t *input,
        size_t input_size,
        CompressionType compression)
{
    std::string msg;

    switch (compression)
    {
        case zlib:
#if HAS_ZLIB
            return zlib_compress(stream, output_size, input, input_size);
#else
            msg = "Can't compress block: zlib library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        case bzip2:
#if HAS_BZIP2
            return bzip2_compress(stream, output_size, input, input_size);
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
        case zlib:
#if HAS_ZLIB
            return zlib_decompress(output, output_size, input, input_size);
#else
            msg = "Can't decompress block: zlib library is not installed";
            throw std::runtime_error(msg);
#endif
            break;

        case bzip2:
#if HAS_BZIP2
            return bzip2_decompress(output, output_size, input, input_size);
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
static int zlib_compress(
    std::ostream &ostream,
    size_t *output_size,
    const uint8_t *input,
    size_t input_size)
{
    int ret;
    z_stream c_stream;

    /* Allocate temporary buffer for compressed data output */
    /*
     * TODO: consider using deflateBound to get an upper bound on this and just
     * allocate it a single time and free.
     */
    Bytef *outbuf = (Bytef *) malloc(OUTPUT_BUFF_SIZE);
    if (outbuf == nullptr)
    {
        std::string msg = "Failed to allocate temporary buffer for compressed output: ";
        throw std::runtime_error(msg + strerror(errno));
    }

    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.next_in = (Bytef *) input;
    c_stream.avail_in = input_size;
    c_stream.next_out = outbuf;
    c_stream.avail_out = OUTPUT_BUFF_SIZE;

    ret = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
    {
        free(outbuf);
        throw std::runtime_error("zlib error: deflateInit");
    }

    size_t compressed_size = 0;

    /* Process all of the input */
    while (c_stream.total_in < input_size)
    {
        ret = deflate(&c_stream, Z_NO_FLUSH);
        if (ret != Z_OK)
        {
            free(outbuf);
            CHECK_ZLIB_ERR(ret, "deflate");
        }

        if (c_stream.avail_out == 0)
        {
            ostream.write((const char *) outbuf, OUTPUT_BUFF_SIZE);
            /* Reset to the beginning of the temporary buffer */
            c_stream.next_out = outbuf;
            c_stream.avail_out = OUTPUT_BUFF_SIZE;
            compressed_size += OUTPUT_BUFF_SIZE;
        }
    }

    /* Finish producing the output */
    for (;;)
    {
        if (c_stream.avail_out == 0 || ret == Z_STREAM_END)
        {
            size_t new_bytes_written = c_stream.total_out - compressed_size;
            ostream.write((const char *) outbuf, new_bytes_written);
            c_stream.next_out = outbuf;
            c_stream.avail_out = OUTPUT_BUFF_SIZE;
            compressed_size += new_bytes_written;
        }

        if (ret == Z_STREAM_END) break;
        ret = deflate(&c_stream, Z_FINISH);
    }

    ret = deflateEnd(&c_stream);
    if (ret != Z_OK)
    {
        free(outbuf);
        throw std::runtime_error("zlib error: deflateEnd");
    }

    free(outbuf);
    *output_size = compressed_size;

    return 0;
}

static int zlib_decompress(
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
static int bzip2_compress(
    std::ostream &ostream,
    size_t *output_size,
    const uint8_t *input,
    size_t input_size)
{
    int ret;
    bz_stream c_stream;

    /* Allocate temporary buffer for compressed data output */
    char *outbuf = (char *) malloc(OUTPUT_BUFF_SIZE);
    if (outbuf == nullptr)
    {
        std::string msg = "Failed to allocate temporary buffer for compressed output: ";
        throw std::runtime_error(msg + strerror(errno));
    }

    c_stream.bzalloc = nullptr;
    c_stream.bzfree = nullptr;
    c_stream.next_in = (char *) input;
    c_stream.avail_in = input_size;
    c_stream.next_out = outbuf;
    c_stream.avail_out = OUTPUT_BUFF_SIZE;

    ret = BZ2_bzCompressInit(&c_stream, 5, 0, 0);
    if (ret != BZ_OK)
    {
        free(outbuf);
        throw std::runtime_error("bzip2 error: deflateInit");
    }

    size_t compressed_size = 0;
    size_t total_in = PACK_U64(c_stream.total_in_hi32, c_stream.total_in_lo32);

    /* Process all of the input */
    while (total_in < input_size)
    {
        ret = BZ2_bzCompress(&c_stream, BZ_RUN);
        total_in = PACK_U64(c_stream.total_in_hi32, c_stream.total_in_lo32);
        if (ret != BZ_RUN_OK)
        {
            free(outbuf);
            CHECK_BZIP_ERR(ret, "compress");
        }

        if (c_stream.avail_out == 0)
        {
            ostream.write((const char *) outbuf, OUTPUT_BUFF_SIZE);
            /* Reset to the beginning of the temporary buffer */
            c_stream.next_out = outbuf;
            c_stream.avail_out = OUTPUT_BUFF_SIZE;
            compressed_size += OUTPUT_BUFF_SIZE;
        }
    }

    /* Finish producing the output */
    for (;;)
    {
        if (c_stream.avail_out == 0 || ret == BZ_STREAM_END)
        {
            size_t total_out = PACK_U64(c_stream.total_out_hi32, c_stream.total_out_lo32);
            size_t new_bytes_written = total_out - compressed_size;

            ostream.write((const char *) outbuf, new_bytes_written);
            c_stream.next_out = outbuf;
            c_stream.avail_out = OUTPUT_BUFF_SIZE;
            compressed_size += new_bytes_written;
        }

        if (ret == BZ_STREAM_END) break;
        ret = BZ2_bzCompress(&c_stream, BZ_FINISH);
    }

    free(outbuf);
    *output_size = compressed_size;

    return 0;
}

static int bzip2_decompress(
        uint8_t *output,
        size_t output_size,
        const uint8_t *input,
        size_t input_size)
{
    int ret;
    bz_stream stream;

    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.next_in = (char *) input;
    stream.avail_in = input_size;
    stream.next_out = (char *) output;
    stream.avail_out = output_size;

    ret = BZ2_bzDecompressInit(&stream, 0, 0);
    CHECK_BZIP_ERR(ret, "bzDecompressInit");

    ret = BZ2_bzDecompress(&stream);
    if (ret != BZ_STREAM_END)
    {
        CHECK_BZIP_ERR(ret, "bzDecompress");
    }

    ret = BZ2_bzDecompressEnd(&stream);
    CHECK_BZIP_ERR(ret, "bzDecompressEnd");

    return 0;
}
#endif
