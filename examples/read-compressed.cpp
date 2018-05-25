#include <iostream>
#include <cstdio>
#include <cassert>

#include <asdf-cpp/asdf.hpp>


using namespace Asdf;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf(argv[1]);
    Asdf::Node tree = asdf.get_tree();

    auto data_array = tree["zlib_data"].as<NDArray<int>>();
    auto data = data_array.read().get();

    for (size_t i = 0; i < data_array.get_shape()[0]; i++)
    {
        assert(data[i] == i);
    }
}
