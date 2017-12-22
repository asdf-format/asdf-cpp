#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstdio>
#include <cstdint>

#include <yaml-cpp/yaml.h>


namespace Asdf {
class AsdfFile
{
    public:
        /* Constructors */
        AsdfFile(std::string filename);

        /* Getters/Setters */
        std::string get_filename(void);
        YAML::Node get_tree(void);
        YAML::Node operator[] (std::string key);

    private:
        /* Private members */
        std::ifstream ifs;
        std::stringstream yaml_data;
        std::string filename;

        YAML::Node asdf_tree;

}; /* AsdfFile class */
} /* namespace Asdf */
