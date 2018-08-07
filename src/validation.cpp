/**
 * The bulk of this file represents an implementation of a custom JSON Schema
 * adapter that allows YAML schemas and files to be used for validation.
 */
#include <string>
#include <cstdio>
#include <cstdint>

#include <yaml-cpp/yaml.h>

#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/adapters/basic_adapter.hpp>


using valijson::adapters::Adapter;
using valijson::adapters::BasicAdapter;
using valijson::adapters::FrozenValue;
using valijson::adapters::DerefProxy;


class YamlCppArrayValueIterator;


/**
 * @brief  Light weight wrapper for a YamlCpp array node.
 *
 * This class is light weight wrapper for a YamlCpp array. It provides a
 * minimum set of container functions and typedefs that allow it to be used as
 * an iterable container.
 *
 * An instance of this class contains a single reference to the underlying
 * YamlCpp node, assumed to be an array, so there is very little overhead
 * associated with copy construction and passing by node.
 */
class YamlCppArray
{
public:
    typedef YamlCppArrayValueIterator const_iterator;
    typedef YamlCppArrayValueIterator iterator;

    /// Construct a YamlCppArray referencing an empty array.
    YamlCppArray() : node(emptyArray()) { }

    /**
     * @brief   Construct a YamlCppArray referencing a specific YAML node.
     *
     * @param   node   reference to a YAML node
     *
     * Note that this constructor will throw an exception if the node is not
     * an array.
     */
    YamlCppArray(const YAML::Node &node) : node(node)
    {
        if (!node.IsSequence()) {
            throw std::runtime_error("Node is not an array.");
        }
    }

    /**
     * @brief   Return an iterator for the first element of the array.
     *
     * The iterator return by this function is effectively the iterator
     * returned by the underlying YamlCpp implementation.
     */
    YamlCppArrayValueIterator begin() const;

    /**
     * @brief   Return an iterator for one-past the last element of the array.
     *
     * The iterator return by this function is effectively the iterator
     * returned by the underlying YamlCpp implementation.
     */
    YamlCppArrayValueIterator end() const;

    /// Return the number of elements in the array.
    size_t size() const
    {
        return node.size();
    }

private:
    /**
     * @brief   Return a reference to a YamlCpp node that is an empty array.
     *
     * Note that the value returned by this function is a singleton.
     */
    static const YAML::Node & emptyArray()
    {
        static const YAML::Node array(YAML::NodeType::Sequence);
        return array;
    }

    /// Reference to the contained array
    const YAML::Node &node;
};


class YamlCppObjectMember
{

};


class YamlCppObject
{

};


class YamlCppFrozenNode: public FrozenValue
{
    public:
        /**
         * @brief  Make a copy of a YAML Node
         *
         * @param  source  the YAML node to be copied
         */
        explicit YamlCppFrozenNode(const YAML::Node &source) : node(source) { }

        virtual FrozenValue * clone() const
        {
            return new YamlCppFrozenNode(node);
        }

        virtual bool equalTo(const Adapter &other, bool strict) const;

private:

    /// Stored YAML node
    YAML::Node node;

};


/**
 * @brief   Light weight wrapper for a YAML node.
 *
 * This class is passed as an argument to the BasicAdapter template class, and
 * is used to provide access to a YAML node. This class is responsible for the
 * mechanics of actually reading a YAML node, whereas the BasicAdapter class is
 * responsible for the semantics of type comparisons and conversions.
 *
 * The functions that need to be provided by this class are defined implicitly
 * by the implementation of the BasicAdapter template class.
 *
 * @see BasicAdapter
 */
class YamlCppNode
{
    public:

        YamlCppNode() : node(emptyObject()) { }
        YamlCppNode(const YAML::Node &node) : node(node) { }

        /**
         * @brief   Create a new YamlCppFrozenNode instance that contains the
         *          value referenced by this YamlCppNode instance.
         *
         * @returns pointer to a new YamlCppFrozenNode instance, belonging to
         *          the caller.
         */
        FrozenValue * freeze() const
        {
            return new YamlCppFrozenNode(node);
        }

        /*
         * YAML does not assign strict types to node values. This means that
         * many of the "is*" and "get*" methods below will also return false.
         */
        static bool hasStrictTypes()
        {
            return false;
        }

        bool getObjectSize(size_t &result) const
        {
            if (node.IsNull())
            {
                return false;
            }

            result = node.size();
            return true;
        }

        bool getArraySize(size_t &result) const
        {
            return getObjectSize(result);
        }

        bool getBool(bool &) const
        {
            return false;
        }

        bool getDouble(double &) const
        {
            return false;
        }

        bool getInteger(int64_t &) const
        {
            return false;
        }

        bool getString(std::string &result) const
        {
            result = node.as<std::string>();
            return true;
        }

        /**
         * @brief   Optionally return a YamlCppArray instance.
         *
         * If the referenced YAML node is an array, this function will return a
         * std::optional containing a YamlCppArray instance referencing the
         * array.
         *
         * Otherwise it will return an empty optional.
         */
        opt::optional<YamlCppArray> getArrayOptional() const
        {
            if (isArray()) {
                return opt::make_optional(YamlCppArray(node));
            }

            return opt::optional<YamlCppArray>();
        }

        bool isArray() const
        {
            return node.IsSequence() && !node.IsNull();
        }

        bool isInteger() const
        {
            return false;
        }

        bool isDouble() const
        {
            return false;
        }

        bool isNumber() const
        {
            return false;
        }

        bool isBool() const
        {
            return false;
        }

        bool isString() const
        {
            return false;
        }

        bool isObject() const
        {
            return false;
        }

        bool isNull() const
        {
            return node.IsNull();
        }

    private:

        static const YAML::Node& emptyObject()
        {
            static const YAML::Node object;
            return object;
        }

        const YAML::Node &node;
};


class YamlCppAdapter:
    public BasicAdapter<YamlCppAdapter,
                        YamlCppArray,
                        YamlCppObjectMember,
                        YamlCppObject,
                        YamlCppNode>
{
    public:

        YamlCppAdapter() : BasicAdapter() { }
        YamlCppAdapter(const YAML::Node &node) : BasicAdapter(node) { }

};


/**
 * @brief   Class for iterating over values held in a YAML array.
 *
 * This class provides a YAML array iterator that dereferences as an instance
 * of YamlCppAdapter representing a value stored in the array.
 *
 * @see YamlCppArray
 */
class YamlCppArrayValueIterator:
    public std::iterator<
        std::forward_iterator_tag,      // forward iterator tag
        YamlCppAdapter>                 // value type
{

public:

    /**
     * @brief   Construct a new YamlCppArrayValueIterator using an existing
     *          YamlCpp iterator.
     *
     * @param   itr  YamlCpp iterator to store
     */
    YamlCppArrayValueIterator(const YAML::Node::const_iterator &itr)
      : itr(itr) { }

    /// Returns a YamlCppAdapter that contains the value of the current element.
    YamlCppAdapter operator*() const
    {
        return YamlCppAdapter(*itr);
    }

    DerefProxy<YamlCppAdapter> operator->() const
    {
        return DerefProxy<YamlCppAdapter>(**this);
    }

    /**
     * @brief   Compare this iterator against another iterator.
     *
     * Note that this directly compares the iterators, not the underlying
     * values, and assumes that two identical iterators will point to the same
     * underlying object.
     *
     * @param   rhs  iterator to compare against
     *
     * @returns true if the iterators are equal, false otherwise.
     */
    bool operator==(const YamlCppArrayValueIterator &rhs) const
    {
        return itr == rhs.itr;
    }

    bool operator!=(const YamlCppArrayValueIterator &rhs) const
    {
        return !(itr == rhs.itr);
    }

    YamlCppArrayValueIterator& operator++()
    {
        itr++;

        return *this;
    }

    YamlCppArrayValueIterator operator++(int)
    {
        YamlCppArrayValueIterator iterator_pre(itr);
        ++(*this);
        return iterator_pre;
    }

    void advance(std::ptrdiff_t n)
    {
        if (n < 0) {
            throw std::runtime_error(
                    "Attempt to use negative increment on forward iterator");
        }

        while (n-- > 0) {
            itr++;
        }
    }

private:

    YAML::Node::const_iterator itr;
};


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s <schema> <document>\n", argv[0]);
        return 1;
    }

    YAML::Node schema = YAML::LoadFile(argv[1]);
    YamlCppAdapter schemaAdapter(schema);
}
