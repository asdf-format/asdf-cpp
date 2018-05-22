#include <iostream>
#include <vector>
#include <random>
#include <asdf.hpp>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf;
    Asdf::Node tree = asdf.get_tree();

    tree["foo"] = "hello, world!";
    tree["vec"] = std::vector<int>({1,2,3,4,5});
    tree["bar"] = Asdf::Node();
    tree["bar"]["baz"] = "blurg";

    /* Create 1D array of integers */
    int array_size = 1000;
    int *nums = (int *) calloc(1, sizeof(*nums) * array_size);
    for (int i = 0; i < array_size; i++)
    {
        nums[i] = i;
    }

    tree["array"] = Asdf::NDArray<int>(nums, array_size);

    /* Create 2D array of integers */
    int array_2d[10][20] = {};
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            array_2d[i][j] = 20*i + j;
        }
    }

    auto shape = std::vector<size_t> { 10, 20 };
    tree["2darray"] = Asdf::NDArray<int>((int *) array_2d, shape);

    /* Create 1D array of characters */
    char alphabet[26] = {};
    for (int i = 0; i < 26; i++)
    {
        alphabet[i] = 'a' + (char) i;
    }

    tree["alphabet"] = Asdf::NDArray<char>(alphabet, 26);


    /* Create 1D array of pseudo-random doubles */
    std::ranlux48 engine(0);
    std::uniform_real_distribution<double> dist;
    std::vector<double> random;

    for (int i = 0; i < 1024; i++)
    {
        random.push_back(dist(engine));
    }

    tree["random"] = Asdf::NDArray<double>(random.data(), random.size());

    std::ofstream outfile(argv[1]);
    outfile << asdf;
}
