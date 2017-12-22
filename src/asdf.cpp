#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <cstring>
#include <error.h>

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

static std::streampos find_yaml_end(std::stringstream &yaml, std::ifstream &ifs)
{
    std::string line;

    while(std::getline(ifs, line))
    {
        yaml << line << std::endl;
        if (line.compare(YAML_END_MARKER) == 0)
        {
            break;
        }
    }

    return ifs.tellg();
}

namespace Asdf {

AsdfFile::AsdfFile(std::string filename)
{
    this->filename = filename;

    ifs.open(this->filename);
    if (ifs.fail())
    {
        std::string msg("Error opening " + filename + ": ");
        throw std::runtime_error(msg + strerror(errno));
    }

    if (!parse_header(ifs))
    {
        throw std::runtime_error("Invalid ASDF header");
    }

    std::streampos end_index = find_yaml_end(yaml_data, ifs);
    std::cout << "end index=" << end_index << std::endl;

    /* Reset stream to the beginning of the file */
    ifs.seekg(0);

    asdf_tree = YAML::Load(yaml_data);
}

std::string AsdfFile::get_filename()
{
    return filename;
}

YAML::Node AsdfFile::operator[] (std::string key)
{
    return asdf_tree[key];
}

} // namespace Asdf
