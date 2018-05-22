#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdint>

#include <node.hpp>
#include <private/block_manager.hpp>

namespace Asdf {

class AsdfFile
{
    public:
        /* Constructors */
        AsdfFile();
        /* TODO: consider providing a factory method here instead */
        AsdfFile(std::string filename);
        AsdfFile(std::stringstream &stream);
        ~AsdfFile(void);

        /* Getters/Setters */
        std::string get_filename(void);
        Node get_tree(void);
        Node operator[] (std::string key);

        friend std::ostream&
            operator<<(std::ostream& stream, const AsdfFile &af);

        void * get_block(int source) const;

    protected:
        BlockManager block_manager;

        template <typename T> friend class NDArray;
        template <typename T> int register_array_block(T *data, size_t size)
        {
            return block_manager.add_data_block<T>(data, size);
        }

        void write_blocks(std::ostream &ostream) const;

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
