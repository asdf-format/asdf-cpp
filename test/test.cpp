#include <iostream>
#include <cstdio>
#include <asdf.hpp>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <file name>\n", argv[0]);
        return 1;
    }

    Asdf::AsdfFile asdf(argv[1]);
    YAML::Node tree = asdf.get_tree();
    std::cout << tree.Tag() << std::endl;

    YAML::Node top = asdf["top"];

    std::cout << "ASDF tree:" << std::endl;
    std::cout << top << std::endl;

    std::cout << "ASDF Subtree:" << std::endl;
    std::cout << top["nums"] << std::endl;
}
