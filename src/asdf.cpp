#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <yaml-cpp/yaml.h>
#include <asdf.hpp>

#define ASDF_HEADER             "#ASDF"
#define ASDF_STANDARD_HEADER    "#ASDF_STANDARD"
#define YAML_END_MARKER         "..."


static bool parse_header(std::ifstream &ifs)
{
    std::string line;
    std::getline(ifs, line);

    if (line.compare(0, strlen(ASDF_HEADER), ASDF_HEADER))
    {
        return false;
    }

    std::getline(ifs, line);
    if (line.compare(0, strlen(ASDF_STANDARD_HEADER), ASDF_STANDARD_HEADER))
    {
        return false;
    }

    return true;
}

static std::streampos find_yaml_end(std::ifstream &ifs)
{
    std::string line;

    while(std::getline(ifs, line))
    {
        std::cout << line << std::endl;
        if (line.compare(YAML_END_MARKER) == 0)
        {
            break;
        }
    }

    return ifs.tellg();
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

    std::streampos end_index = find_yaml_end(ifs);
    std::cout << "end index=" << end_index << std::endl;

    /* Reset stream to the beginning of the file */
    ifs.seekg(0);

    YAML::Node asdf = YAML::Load(ifs);
    std::cout << asdf["top"] << std::endl;


    return 0;
}
