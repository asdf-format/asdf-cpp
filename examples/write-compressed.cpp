#include <iostream>
#include <vector>
#include <random>

#include <asdf-cpp/asdf.hpp>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf;
    Asdf::Node tree = asdf.get_tree();

    /* Create 1D array of integers */
    int array_size = 1000;
    int *nums = (int *) calloc(1, sizeof(*nums) * array_size);
    for (int i = 0; i < array_size; i++)
    {
        nums[i] = i;
    }

    auto zlib_array_ref = asdf.create_data_block<int>(nums, array_size,
                                                      CompressionType::zlib);
    tree["zlib_data"] = zlib_array_ref;

    /*
     * For now this will simply make a copy of the data, so no need to create a
     * new array for testing purposes.
     */
    auto bzip2_array_ref = asdf.create_data_block<int>(nums, array_size,
                                                       CompressionType::bzip2);
    tree["bzp2_data"] = bzip2_array_ref;

    std::ofstream outfile(argv[1]);
    outfile << asdf;
}
