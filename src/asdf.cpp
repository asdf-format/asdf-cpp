#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <yaml-cpp/yaml.h>
#include <asdf.hpp>

#define ASDF_HEADER             "#ASDF"
#define ASDF_STANDARD_HEADER    "#ASDF_STANDARD"


static bool parse_header(std::ifstream &ifs)
{
    std::string line;
    std::getline(ifs, line);

    if (line.compare(0, strlen(ASDF_HEADER), ASDF_HEADER) != 0)
    {
        return false;
    }

    std::getline(ifs, line);
    if (line.compare(0, strlen(ASDF_STANDARD_HEADER), ASDF_STANDARD_HEADER) !=0)
    {
        return false;
    }

    return true;
}

int load_asdf_file(const char *filename)
{
    printf("loading %s\n", filename);

    std::ifstream ifs;
    ifs.open(filename);

    if (!parse_header(ifs))
    {
        fprintf(stderr, "Invalid ASDF header\n");
        return 1;
    }

    YAML::Node asdf = YAML::Load(ifs);
    std::cout << asdf["top"] << std::endl;


    return 0;
}
