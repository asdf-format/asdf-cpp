#pragma once

namespace Asdf {

class AsdfFile;

class AbstractNDArray
{
    public:
        virtual void register_array_block(AsdfFile *file) const = 0;

        int get_source() const
        {
            return source;
        }

        std::vector<size_t> get_shape() const
        {
            return shape;
        }

        void set_asdf_file(const AsdfFile *file)
        {
            this->file = file;
        }

    protected:
        int source;
        std::string datatype;
        std::string byteorder;
        std::vector<size_t> shape;
        const AsdfFile *file;

        AbstractNDArray() { file = nullptr; }

        AbstractNDArray(int source, std::vector<size_t> shape, const AsdfFile *file)
        {
            this->source = source;
            this->shape = shape;
            this->file = file;
        }

        friend std::ostream&
        operator<<(std::ostream &strm, const AbstractNDArray &array)
        {
            return strm << "NDArray";
        }
};

}
