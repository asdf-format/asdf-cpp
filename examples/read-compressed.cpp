#include <iostream>
#include <cstdio>
#include <cassert>

#include <asdf-cpp/asdf.hpp>


using namespace Asdf;

static void verify(int *data, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        assert(data[i] == i);
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
    auto zlib_array = tree["zlib_data"].as<NDArray<int>>();
    std::cout << zlib_array << std::endl;
    auto zlib_data = zlib_array.read().get();
    verify(zlib_data, zlib_array.get_shape()[0]);

    /* Read array that is known to be compressed with bzp2 */
    auto bzp2_array = tree["bzp2_data"].as<NDArray<int>>();
    std::cout << bzp2_array << std::endl;
    auto bzp2_data = bzp2_array.read().get();
    verify(bzp2_data, bzp2_array.get_shape()[0]);
}
