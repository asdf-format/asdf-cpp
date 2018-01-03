#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdint>

#include <node.hpp>


namespace Asdf {
class AsdfFile
{
    public:
        /* Constructors */
        AsdfFile(std::string filename);
        ~AsdfFile(void);

        /* Getters/Setters */
        std::string get_filename(void);
        Node get_tree(void);
        Node operator[] (std::string key);

        void * get_block(int source);

    private:
        /* Private members */
        std::ifstream ifs;
        std::stringstream yaml_data;
        std::string filename;

        Node asdf_tree;

        int fd = -1;
        long file_size = 0;
        uint8_t *memmap = nullptr;

        std::streampos end_index = 0;

        std::vector<uint8_t *> blocks;

        /* Private methods */
        void setup_memmap(void);
        void find_blocks(void);

}; /* AsdfFile class */
} /* namespace Asdf */
