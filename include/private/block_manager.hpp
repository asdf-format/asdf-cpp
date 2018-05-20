#pragma once

#include <map>
#include <vector>
#include <memory>
#include <iostream>

#include <private/block.hpp>


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
        Block(T *buff, size_t length) : GenericBlock()
        {
            this->buff = buff;
            this->length = length;
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

            memcpy(header.magic, asdf_block_magic, sizeof(header.magic));
            header.header_size = header_size;

            /* For now these are all the same */
            header.allocated_size = data_size;
            header.used_size = data_size;
            header.data_size = data_size;

            ostream.write((char *) &header, sizeof(header));
        }

    private:
        T *buff = nullptr;
        size_t length = 0;
};

class BlockManager {

    public:
        BlockManager()
        {
            std::cout << blocks.size() << std::endl;
        }

        void write_blocks(std::ostream &ostream) const;

        int get_length()
        {
            return blocks.size();
        }

        template <typename T> void add_data_block(T *data, size_t length)
        {
            std::cout << "length: " << blocks.size() << std::endl;
            auto block = new Block<T>(data, length);
            blocks.push_back(std::shared_ptr<GenericBlock>(dynamic_cast<GenericBlock*>(block)));
        }

    protected:
        std::vector<std::shared_ptr<GenericBlock>> blocks;
};

}
