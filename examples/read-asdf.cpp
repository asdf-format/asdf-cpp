#include <iostream>
#include <cstdio>
#include <cassert>
#include <random>

#include <asdf-cpp/asdf.hpp>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    /* Use this constructor to read the ASDF file */
    Asdf::AsdfFile asdf(argv[1]);
    Asdf::Node tree = asdf.get_tree();

    /* Print top-level tag */
    std::cout << "ASDF Tag: " << tree.Tag() << std::endl;

    /* Display the entire tree */
    std::cout << tree << std::endl;

    /*
     * Read the NDArray that is stored at this node. The application must know
     * the underlying data type of the array at compile time.
     */
    auto array = asdf.get_array<int>(tree["array"]);
    std::cout << array << std::endl;

    /* Demonstrate API for getting NDArray compression type */
    assert(array.get_compression_type() == none);
    assert(array.is_compressed() == false);

    /*
     * Get the raw data stored in the array. In this case it is actually a
     * memory map of the underlying file. This is safe to do as long as we
     * intend to use it as a read-only buffer. It is also acceptable since we
     * have confirmed that the array was not stored compressed.
     */
    int *ddata = array.get_raw_data();
    for (int i = 0; i < 10; i++)
    {
        assert(ddata[i] == i);
    }

    auto array_2d = asdf.get_array<int>(tree["2darray"]);
    std::cout << array_2d << std::endl;

    /* Confirm that the array is in fact two dimensional */
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
    int (*ddata_2d)[20] = reinterpret_cast<int (*)[20]>(array_2d.get_raw_data());
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            assert(ddata_2d[i][j] == 20*i + j);
        }
    }

    auto alphabet = asdf.get_array<char>(tree["alphabet"]);
    std::cout << alphabet << std::endl;

    char *alpha_buff = alphabet.get_raw_data();
    for (size_t i = 0; i < alphabet.get_shape()[0]; i++)
    {
        assert(alpha_buff[i] == 'a' + (char) i);
    }

    auto random = asdf.get_array<double>(tree["random"]);
    std::cout << random << std::endl;

    /* Use same seed for random engine as was used to create the file */
    std::ranlux48 engine(0);
    std::uniform_real_distribution<double> dist;

    double *rand_buff = random.get_raw_data();
    for (size_t i = 0; i < alphabet.get_shape()[0]; i++)
    {
        assert(rand_buff[i] == dist(engine));
    }
}
