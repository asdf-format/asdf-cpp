#include <string>
#include <stdexcept>

#include <asdf-cpp/compression.hpp>
#include <asdf-cpp/private/compression.hpp>


#ifdef HAS_ZLIB
static int gzip_compress(void);
static int gzip_decompress(void);
#endif

#ifdef HAS_BZIP2
static int bzip2_compress(void);
static int bzip2_decompress(void);
#endif


int compress_block(
        uint8_t *output,
        const uint8_t *input,
        size_t input_size,
        size_t *output_size,
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
        const uint8_t *input,
        size_t input_size,
        size_t *output_size,
        CompressionType compression)
{
    std::string msg;

    switch (compression)
    {
        case gzip:
#if HAS_ZLIB
            return gzip_decompress();
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

static int gzip_decompress(void)
{

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
