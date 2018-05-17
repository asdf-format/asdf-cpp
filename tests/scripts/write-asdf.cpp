#include <iostream>
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

    std::cout << &asdf << std::endl;
    std::cout << sizeof(asdf) << std::endl;

    tree["foo"] = "hello, world!";
    tree["vec"] = std::vector<int>({1,2,3,4,5});
    tree["bar"] = Asdf::Node();
    tree["bar"]["baz"] = "blurg";

    int array_size = 1000;
    int *nums = (int *) calloc(1, sizeof(*nums) * array_size);
    for (int i = 0; i < array_size; i++)
    {
        nums[i] = i;
    }

    tree["array"] = Asdf::NDArray<int>(nums, array_size);
    std::cout << "array" << std::endl;


    std::ofstream outfile(argv[1]);
    outfile << asdf;
}
