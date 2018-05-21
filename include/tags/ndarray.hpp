#pragma once

#include <vector>
#include <sstream>
#include <type_traits>
#include <yaml-cpp/yaml.h>

#include "abstract_ndarray.hpp"

#define NDARRAY_TAG_BASE    "tag:stsci.edu:asdf/core/ndarray"
#define NDARRAY_TAG_VERSION "1.0.0"
#define NDARRAY_TAG         (NDARRAY_TAG_BASE "-" NDARRAY_TAG_VERSION)


namespace Asdf {

template <typename T>
class NDArray : public AbstractNDArray
{
    public:
        NDArray(T *data, size_t size)
        {
            this->data = data;
            byteorder = "little";
            shape = { size };

            using std::is_same;
            if (is_same<T, int>::value)
            {
                datatype = "int32";
            }
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
        friend class Node;
        friend struct YAML::convert<Asdf::NDArray<T>>;
        friend struct YAML::as_if<Asdf::NDArray<T>, void>;

        NDArray() : AbstractNDArray() {}

        /*
         * This constructor is called when creating a new NDArray object from a
         * YAML representation (in the "decode" method defined below). It is
         * protected since it will never be used by application code.
         */
        NDArray(int source, std::vector<size_t> shape, const AsdfFile *file) :
            AbstractNDArray(source, shape, file) {}

        /* 
         * This callback is used to register the associated NDArray data as a
         * data block with the parent AsdfFile object. It is called when the
         * associated Asdf Node is assigned to the tree.
         */
        int register_array_block(AsdfFile *file) const
        {
            return file->register_array_block<T>(data, shape[0]);
        }

        void write(AsdfFile &file);

    private:
        T * data = nullptr;

};

} /* namespace Asdf */

namespace YAML {

template <typename T>
struct convert<Asdf::NDArray<T>>
{
    /*
     * This method defines the way in which NDArray metadata is written to the
     * YAML tree. The block allocation of the NDArray is handled at a higher
     * level when the NDArray object is assigned to a Node belonging to an
     * AsdfFile tree.
     */
    static Node encode(const Asdf::NDArray<T> &array)
    {
        Asdf::Node node;
        node.SetTag(NDARRAY_TAG);

        /* TODO: handle the case of inline arrays */
        node["source"] = array.get_source();
        node["datatype"] = array.datatype;
        node["byteorder"] = array.byteorder;

        for (auto x : array.get_shape())
        {
            node["shape"].push_back(x);
        }

        return node;
    }

    /*
     * This method defines the way in which NDArray objects are constructed
     * when reading from a YAML tree. The AsdfFile pointer belonging to the
     * parent node is passed to the new NDArray object in order to facilitate
     * access to the associated data block.
     */
    static bool decode(const Node &node, Asdf::NDArray<T> &array)
    {
        if (node.Tag() != NDARRAY_TAG)
        {
            return false;
        }

        auto source = node["source"].as<int>();
        auto shape = node["shape"].as<std::vector<size_t>>();

        const Asdf::Node& asdf_node = static_cast<const Asdf::Node &>(node);
        array = Asdf::NDArray<T>(source, shape, asdf_node.get_asdf_file());

        return true;
    }
};

} /* namespace YAML */
