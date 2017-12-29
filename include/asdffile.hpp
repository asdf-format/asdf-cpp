#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <yaml-cpp/yaml.h>


namespace Asdf {
class AsdfFile
{
    public:
        /* Constructors */
        AsdfFile(std::string filename);
        ~AsdfFile(void);

        /* Getters/Setters */
        std::string get_filename(void);
        YAML::Node get_tree(void);
        YAML::Node operator[] (std::string key);

        void * get_block(int source);

    private:
        /* Private members */
        std::ifstream ifs;
        std::stringstream yaml_data;
        std::string filename;

        YAML::Node asdf_tree;

        int fd = -1;
        long file_size = 0;
        void *memmap = nullptr;

        std::streampos end_index = 0;

        void setup_memmap();

}; /* AsdfFile class */
} /* namespace Asdf */
