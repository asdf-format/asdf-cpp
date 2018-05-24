#pragma once

#include <map>
#include <vector>
#include <memory>
#include <iostream>

#include "block.hpp"
#include "compression.hpp"


namespace Asdf {

class GenericBlock {
    public:
        GenericBlock(void) {}

    protected:
        friend class BlockManager;

        virtual void write(std::ostream &ostream) const = 0;
        virtual void write_header(std::ostream &ostream) const = 0;
};

template <typename T>
class Block : public GenericBlock {

    public:
        Block(T *buff, size_t length, CompressionType compression) : GenericBlock()
        {
            this->buff = buff;
            this->length = length;
            this->compression = compression;
        }

    protected:
        friend class BlockManager;

        void write(std::ostream &ostream) const
        {
            write_header(ostream);
            ostream.write((char *) buff, sizeof(T) * length);
        }

        void write_header(std::ostream &ostream) const
        {
            block_header_t header = {};

            size_t data_size = sizeof(T) * length;

            header.set_header_size(header_size);

            /* For now these are all the same */
            header.set_allocated_size(data_size);
            header.set_used_size(data_size);
            header.set_data_size(data_size);

            ostream.write((char *) &header, sizeof(header));
        }

    private:
        T *buff = nullptr;
        size_t length = 0;
        CompressionType compression = CompressionType::none;
};

class BlockManager {

    public:
        void write_blocks(std::ostream &ostream) const
        {
            for (auto b : blocks)
            {
                b->write(ostream);
            }
        }

        int get_length()
        {
            return blocks.size();
        }

        template <typename T> int
            add_data_block(T *data, size_t length, CompressionType compression)
        {
            int source_idx = blocks.size();
            auto block = new Block<T>(data, length, compression);
            blocks.push_back(std::shared_ptr<GenericBlock>(dynamic_cast<GenericBlock*>(block)));
            return source_idx;
        }

    protected:
        std::vector<std::shared_ptr<GenericBlock>> blocks;
};

}
