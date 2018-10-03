#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

#include <cstdint>

#include "node.hpp"
#include "compression.hpp"
#include "block_manager.hpp"
#include "tags/ndarray.hpp"

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

        template <typename T> NDArray<T> create_array_node(
            T *data,
            size_t size,
            CompressionType compression = CompressionType::none)
        {
            return create_array_node(data, std::vector<size_t> { size },
                                     compression);
        }

        template <typename T> NDArray<T> create_array_node(
            T *data,
            std::vector<size_t> shape,
            CompressionType compression = CompressionType::none)
        {
            using std::accumulate;
            using std::multiplies;
            using std::begin;
            using std::end;

            auto size = accumulate(begin(shape), end(shape), 1,
                                   multiplies<size_t>());

            int block_id = block_manager.add_data_block<T>(data, size,
                                                           compression);

            return NDArray<T>(block_id, shape, compression);
        }

        template <typename T> NDArray<T> get_array(Node node) const
        {
            auto array = node.as<NDArray<T>>();
            array.set_array_block(blocks[array.get_source()]);

            return array;
        }

    private:
        /* Private members */
        BlockManager block_manager;

        Node asdf_tree;

        int fd = -1;
        size_t data_size = 0;
        /* TODO: make this a shared pointer */
        uint8_t *data = nullptr;

        bool memmapped = false;

        std::streampos end_index = 0;

        std::vector<uint8_t *> blocks;

        /* Private methods */
        void write_blocks(std::ostream &ostream) const;
        void setup_memmap(std::string filename);
        void copy_stream(std::iostream &stream);

        void find_blocks(void);

}; /* AsdfFile class */
} /* namespace Asdf */
