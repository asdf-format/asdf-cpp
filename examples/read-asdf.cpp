#include <iostream>
#include <cstdio>
#include <cassert>
#include <asdf.hpp>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf(argv[1]);
    Asdf::Node tree = asdf.get_tree();

    /* Print top-level tag */
    std::cout << "ASDF Tag: " << tree.Tag() << std::endl;

    /* Display the entire tree */
    std::cout << tree << std::endl;

    auto array = tree["array"].as<Asdf::NDArray<int>>();
    std::cout << array << std::endl;

    int *ddata = array.read();
    for (int i = 0; i < 10; i++)
    {
        assert(ddata[i] == i);
    }

    auto array_2d = tree["2darray"].as<Asdf::NDArray<int>>();
    std::cout << array_2d << std::endl;

    auto shape = array_2d.get_shape();
    assert(shape.size() == 2);
    assert(shape[0] == 10);
    assert(shape[1] == 20);

    /*
     * Recast the flat array data block into the original array shape. There
     * should be a better API for this in the future, including possibly
     * returning a multi-dimensional vector. It might be safer to do this with
     * templates, although all APIs will probably require the application to
     * know something about the array dimensions in advance.
     */
    int (*ddata_2d)[20] = reinterpret_cast<int (*)[20]>(array_2d.read());
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            assert(ddata_2d[i][j] == 20*i + j);
        }
    }
}
