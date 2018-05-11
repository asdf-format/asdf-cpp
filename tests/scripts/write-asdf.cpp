#include <asdf.hpp>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::Node tree;
    tree["foo"] = "hello, world!";
    tree["vec"] = std::vector<int>({1,2,3,4,5});
    tree["bar"] = Asdf::Node();
    tree["bar"]["baz"] = "blurg";

    Asdf::AsdfFile asdf(tree);

    std::ofstream outfile(argv[1]);
    outfile << asdf;
}
