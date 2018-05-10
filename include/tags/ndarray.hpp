#pragma once

#include <vector>
#include <sstream>
#include <yaml-cpp/yaml.h>


namespace Asdf {

template <typename T>
class NDArray
{
    public:
        NDArray() { file = nullptr; };

        NDArray(int source, std::vector<int> shape, const AsdfFile *file)
        {
            this->source = source;
            this->shape = shape;
            this->file = file;
        }

        int get_source() const
        {
            return source;
        }

        std::vector<int> get_shape() const
        {
            return shape;
        }

        T * read(void)
        {
            return (T *) file->get_block(source);
        }

        template<typename... Longs>
        T operator()(Longs... indices)
        {
            std::vector<long> values = {indices...};
            if (values.size() != shape.size())
            {
                std::stringstream msg;
                msg << "Given number of array indices (" << values.size();
                msg << ") doesn't match dimensions (" << shape.size();
                msg << ")";

                throw std::runtime_error(msg.str());
            }

            return 1;
        }


    private:
        int source;
        std::vector<int> shape;
        const AsdfFile *file;

        friend std::ostream&
        operator<<(std::ostream &strm, const NDArray &array)
        {
            return strm << "NDArray";
        }

};

} /* namespace Asdf */

namespace YAML {

template <typename T>
struct convert<Asdf::NDArray<T>>
{
    static Node encode(const Asdf::NDArray<T> &array)
    {
        Node node;

        node["source"] = array.get_source();
        for (auto x : array.get_shape())
        {
            node["shape"].push_back(x);
        }

        return node;
    }

    static bool decode(const Node &node, Asdf::NDArray<T> &array)
    {
        if (node.Tag() != "tag:stsci.edu:asdf/core/ndarray-1.0.0")
        {
            return false;
        }

        int source = node["source"].as<int>();
        std::vector<int> shape = node["shape"].as<std::vector<int>>();

        const Asdf::Node& asdf_node = static_cast<const Asdf::Node &>(node);
        array = Asdf::NDArray<T>(source, shape, asdf_node.get_asdf_file());

        return true;
    }
};

} /* namespace YAML */
