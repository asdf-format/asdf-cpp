#pragma once

#include <vector>
#include <sstream>
#include <type_traits>
#include <yaml-cpp/yaml.h>

#include <asdffile.hpp>

#define NDARRAY_TAG_BASE    "tag:stsci.edu:asdf/core/ndarray"
#define NDARRAY_TAG_VERSION "1.0.0"
#define NDARRAY_TAG         (NDARRAY_TAG_BASE "-" NDARRAY_TAG_VERSION)


namespace Asdf {

template <typename T>
class NDArray
{
    public:
        NDArray(T *data)
        {
            this->data = data;
            byteorder = "little";
            shape = { 1 };

            using std::is_same;
            if (is_same<T, int>::value)
            {
                datatype = "int32";
            }
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

    protected:
        friend struct YAML::convert<Asdf::NDArray<T>>;
        friend struct YAML::as_if<Asdf::NDArray<T>, void>;

        NDArray() { file = nullptr; }

        NDArray(int source, std::vector<int> shape, const AsdfFile *file)
        {
            this->source = source;
            this->shape = shape;
            this->file = file;
        }

        void write(AsdfFile &file);

    private:
        int source;
        std::string datatype;
        std::string byteorder;
        std::vector<int> shape;
        const AsdfFile *file;

        T * data = nullptr;

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
        Asdf::Node node(array);
        node.SetTag(NDARRAY_TAG);

        node["source"] = array.get_source();
        node["datatype"] = array.datatype;
        node["byteorder"] = array.byteorder;

        for (auto x : array.get_shape())
        {
            node["shape"].push_back(x);
        }

        return node;
    }

    static bool decode(const Node &node, Asdf::NDArray<T> &array)
    {
        if (node.Tag() != NDARRAY_TAG)
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
