#pragma once

namespace Asdf {

class AsdfFile;

class AbstractNDArray
{
    public:
        void do_whatever(void) const
        {
            std::cout << "doing whatever" << std::endl;
        }

        int get_source() const
        {
            return source;
        }

        std::vector<int> get_shape() const
        {
            return shape;
        }

    protected:
        int source;
        std::string datatype;
        std::string byteorder;
        std::vector<int> shape;
        const AsdfFile *file;

        AbstractNDArray() { file = nullptr; }

        AbstractNDArray(int source, std::vector<int> shape, const AsdfFile *file)
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
