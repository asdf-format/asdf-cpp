#pragma once

#include <vector>
#include <sstream>
#include <numeric>
#include <type_traits>
#include <yaml-cpp/yaml.h>

#include "../datatypes.hpp"
#include "../compression.hpp"
#include "../block.hpp"
#include "../byteswap.hpp"

#define NDARRAY_TAG_BASE    "tag:stsci.edu:asdf/core/ndarray"
#define NDARRAY_TAG_VERSION "1.0.0"
#define NDARRAY_TAG         (NDARRAY_TAG_BASE "-" NDARRAY_TAG_VERSION)


static std::string system_byte_order = "";

static inline std::string get_system_byte_order()
{
    if (system_byte_order == "")
    {
        union
        {
            uint32_t word;
            uint8_t bytes[4];
        };

        word = 0xdeadbeef;

        system_byte_order = (bytes[0] == 0xef) ? "little" : "big";
    }

    return system_byte_order;
}


namespace Asdf {

template <typename T>
class NDArray
{
    public:
        NDArray(T *data, std::vector<size_t> shape,
                CompressionType compression = CompressionType::none)
        {
            if (compression == unknown)
            {
                std::string msg("'unknown' is not a valid option for array compression");
                throw std::runtime_error(msg);
            }

            this->data = data;
            this->byteorder = get_system_byte_order();
            this->shape = shape;
            this->datatype = dtype_to_string<T>();
            this->compression = compression;
        }

        /* Simple constructor for a 1D array */
        NDArray(T *data, size_t shape,
                CompressionType compression = CompressionType::none) :
            NDArray(data, std::vector<size_t> { shape }, compression) {}

        int get_source() const
        {
            return source;
        }

        std::vector<size_t> get_shape() const
        {
            return shape;
        }

        T * get_raw_data(void)
        {
            const uint8_t *block_data = (const uint8_t *) file->get_block(source);
            const block_header_t *header = (const block_header_t *) block_data;
            return  (T *)(block_data + header->total_header_size());
        }

        CompressionType get_compression_type(void) const
        {
            const uint8_t *block_data = (const uint8_t *) file->get_block(source);
            const block_header_t *header = (const block_header_t *) block_data;
            CompressionType ct = header->get_compression();
            if (ct == unknown)
            {
                throw std::runtime_error("Unknown compression type detected");
            }

            return ct;
        }

        bool is_compressed(void) const
        {
            return get_compression_type() != CompressionType::none;
        }

        std::shared_ptr<T> read(void)
        {
            const uint8_t *block_data = (const uint8_t *) file->get_block(source);
            const block_header_t *header = (const block_header_t *) block_data;

            T *ptr = static_cast<T *>(process_block_data(block_data));

            if (byteorder != system_byte_order)
            {
                byteswap_data(ptr, header->get_data_size()/sizeof(T));
            }

            return std::shared_ptr<T>(ptr);
        }

    protected:
        friend class Node;
        friend struct YAML::convert<Asdf::NDArray<T>>;
        friend struct YAML::as_if<Asdf::NDArray<T>, void>;

        int source;
        std::string datatype;
        std::string byteorder;
        std::vector<size_t> shape;
        CompressionType compression = CompressionType::none;
        const AsdfFile *file;

        NDArray() { file = nullptr; }

        /*
         * This constructor is called when creating a new NDArray object from a
         * YAML representation (in the "decode" method defined below). It is
         * protected since it will never be used by application code.
         */
        NDArray(int source, std::vector<size_t> shape, std::string datatype, const AsdfFile *file)
        {
            if (not dtype_matches<T>(datatype))
            {
                throw std::runtime_error(
                    "Incompatible template argument for NDArray with datatype " + datatype);
            }

            this->source = source;
            this->shape = shape;
            this->datatype = datatype;
            this->file = file;
        }

        /* 
         * This callback is used to register the associated NDArray data as a
         * data block with the parent AsdfFile object. It is called when the
         * associated Asdf Node is assigned to the tree.
         */
        int register_array_block(AsdfFile *file) const
        {
            using std::accumulate;
            using std::multiplies;
            using std::begin;
            using std::end;

            auto size = accumulate(begin(shape), end(shape), 1, multiplies<size_t>());
            return file->register_array_block<T>(data, size, compression);
        }

        friend std::ostream&
        operator<<(std::ostream &strm, const NDArray<T> &array)
        {
            strm << "NDArray[ ";
            for (auto dim : array.shape)
            {
                strm << dim << ", ";
            }

            strm << "], datatype=" << array.datatype;
            strm << ", source=" << array.source;
            strm << ", compression=" << CompressionType_to_string(array.get_compression_type());

            return strm;
        }

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
        auto datatype = node["datatype"].as<std::string>();

        const Asdf::Node& asdf_node = static_cast<const Asdf::Node &>(node);
        array = Asdf::NDArray<T>(source, shape, datatype, asdf_node.get_asdf_file());

        return true;
    }
};

} /* namespace YAML */
