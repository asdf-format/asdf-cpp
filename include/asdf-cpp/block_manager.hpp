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
        virtual void write_header(
                std::ostream &ostream,
                size_t data_size,
                size_t actual_size) const = 0;
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
            if (compression == CompressionType::none)
            {
                size_t data_size = sizeof(T) * length;

                write_header(ostream, data_size, data_size);
                ostream.write((char *) buff, data_size);
            }
            else
            {
                write_compressed_data(ostream);
            }
        }

        void write_header(std::ostream &ostream, size_t data_size, size_t storage_size) const
        {
            block_header_t header = {};

            header.set_header_size(header_size);

            /* TODO: set compression value */

            /* For now these are all the same */
            header.set_allocated_size(storage_size);
            header.set_used_size(storage_size);
            header.set_data_size(data_size);

            ostream.write((char *) &header, sizeof(header));
        }

        void write_compressed_data(std::ostream &ostream) const
        {
            size_t input_size = sizeof(T) * length;
            size_t output_size = 0;

            void *compressed = create_compressed_block(
                                        &output_size,
                                        (const uint8_t *) this->buff,
                                        input_size,
                                        compression);

            /* Once the compressed size is known, write the header */
            write_header(ostream, input_size, output_size);
            ostream.write((char *) compressed, output_size);
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
