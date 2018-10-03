#include <iostream>
#include <cstdio>
#include <cassert>

#include <asdf-cpp/asdf.hpp>


using namespace Asdf;

static void verify(int *data, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        assert(data[i] == (int) i);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf(argv[1]);
    Asdf::Node tree = asdf.get_tree();

    /* Read array that is known to be compressed with zlib */
    auto zlib_array = asdf.get_array<int>(tree["zlib_data"]);
    std::cout << zlib_array << std::endl;

    /* Demonstrate methods for accessing compression state */
    assert(zlib_array.get_compression_type() == CompressionType::zlib);
    assert(zlib_array.is_compressed() == true);

    /*
     * Unlike the example in read-asdf that used the get_raw_data method of
     * NDArray, in this case we must use read instead. This is because the
     * underlying data is compressed, so it does not make sense to access the
     * raw memory map of the file. Instead, read returns to the user a copy of
     * the decompressed data.
     */
    auto zlib_data = zlib_array.read().get();
    verify(zlib_data, zlib_array.get_shape()[0]);

    /* Read array that is known to be compressed with bzp2 */
    auto bzp2_array = asdf.get_array<int>(tree["bzp2_data"]);
    std::cout << bzp2_array << std::endl;

    assert(bzp2_array.get_compression_type() == CompressionType::bzip2);
    assert(bzp2_array.is_compressed() == true);

    auto bzp2_data = bzp2_array.read().get();
    verify(bzp2_data, bzp2_array.get_shape()[0]);
}
