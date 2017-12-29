#include <iostream>
#include <vector>

#include <asdffile.hpp>
#include <tags/ndarray.hpp>


namespace Asdf {

NDArray::NDArray()
{
}

NDArray::NDArray(int source, std::vector<int> shape)
{
    this->source = source;
    this->shape = shape;
}

int NDArray::get_source() const
{
    return source;
}

std::vector<int> NDArray::get_shape() const
{
    return shape;
}

std::ostream& operator<<(std::ostream &strm, const NDArray &array)
{
    return strm << "NDArray";
}

} /* namespace Asdf */

namespace YAML {

Node convert<Asdf::NDArray>::encode(const Asdf::NDArray &array)
{
    Node node;

    node["source"] = array.get_source();
    for (auto x : array.get_shape())
    {
        node["shape"].push_back(x);
    }

    return node;
}

bool convert<Asdf::NDArray>::decode(const Node &node, Asdf::NDArray &array)
{
    if (node.Tag() != "tag:stsci.edu:asdf/core/ndarray-1.0.0")
    {
        return false;
    }

    int source = node["source"].as<int>();
    std::vector<int> shape = node["shape"].as<std::vector<int>>();

    array = Asdf::NDArray(source, shape);

    return true;
}

} /* namespace YAML */
