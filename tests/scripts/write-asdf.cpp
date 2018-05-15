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

    tree["foo"] = "hello, world!";
    tree["vec"] = std::vector<int>({1,2,3,4,5});
    tree["bar"] = Asdf::Node();
    tree["bar"]["baz"] = "blurg";

    int *nums = (int *) calloc(1, sizeof(*nums) * 100);
    tree["array"] = Asdf::NDArray<int>(nums);
    std::cout << "array" << std::endl;


    std::ofstream outfile(argv[1]);
    outfile << asdf;
}
