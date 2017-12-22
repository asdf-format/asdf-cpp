#include <cstdio>
#include <yaml-cpp/yaml.h>
#include <asdf.hpp>



int load_asdf_file(const char *filename)
{
    printf("loading %s\n", filename);

    YAML::Node asdf = YAML::LoadFile(filename);

    return 0;
}
