#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>


namespace Asdf {

template <typename T>
class NDArray
{
    public:
        NDArray() { };

        NDArray(int source, std::vector<int> shape)
        {
            this->source = source;
            this->shape = shape;
        }

        int get_source() const
        {
            return source;
        }

        std::vector<int> get_shape() const
        {
            return shape;
        }

        T * read(AsdfFile &file)
        {
            return (T *) file.get_block(source);
        }


    private:
        int source;
        std::vector<int> shape;

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

        array = Asdf::NDArray<T>(source, shape);

        return true;
    }
};

} /* namespace YAML */
