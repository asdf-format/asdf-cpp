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


class YamlCppAdapter;
class YamlCppArrayValueIterator;
class YamlCppObjectMemberIterator;

typedef std::pair<std::string, YamlCppAdapter> YamlCppObjectMember;


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


/**
 * @brief  Light weight wrapper for a YAML object.
 *
 * This class is light weight wrapper for a YAML object. It provides a
 * minimum set of container functions and typedefs that allow it to be used as
 * an iterable container.
 *
 * An instance of this class contains a single reference to the underlying
 * YAML object, assumed to be an object, so there is very little overhead
 * associated with copy construction and passing by value.
 */
class YamlCppObject
{
public:

    typedef YamlCppObjectMemberIterator const_iterator;
    typedef YamlCppObjectMemberIterator iterator;

    /// Construct a YamlCppObject referencing an empty object singleton.
    YamlCppObject() : node(emptyObject()) { }

    /**
     * @brief   Construct a YamlCppObject referencing a specific YAML node.
     *
     * @param   node  reference to a YAML node
     *
     * Note that this constructor will throw an exception if the node is not
     * an object.
     */
    YamlCppObject(const YAML::Node &node) : node(node)
    {
        if (!node.IsMap()) {
            throw std::runtime_error("Value is not an object.");
        }
    }

    /**
     * @brief   Return an iterator for this first object member
     *
     * The iterator return by this function is effectively a wrapper around
     * the iterator value returned by the underlying YamlCpp implementation.
     */
    YamlCppObjectMemberIterator begin() const;

    /**
     * @brief   Return an iterator for an invalid object member that indicates
     *          the end of the collection.
     *
     * The iterator return by this function is effectively a wrapper around
     * the iterator value returned by the underlying YamlCpp implementation.
     */
    YamlCppObjectMemberIterator end() const;

    /**
     * @brief   Return an iterator for a member/property with the given name
     *
     * @param   propertyName   Property name
     *
     * @returns a valid iterator if found, or an invalid iterator if not found
     */
    YamlCppObjectMemberIterator find(const std::string &propertyName) const;

    /// Return the number of members in the object
    size_t size() const
    {
        return node.size();
    }

private:

    /// Return a reference to an empty YamlCpp object
    static const YAML::Node & emptyObject()
    {
        static const YAML::Node object(YAML::NodeType::Map);
        return object;
    }

    /// Reference to the contained object
    const YAML::Node &node;
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
        YamlCppNode(const YAML::Node node) : node(node) { }

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

        /**
         * @brief   Optionally return a YamlCppObject instance.
         *
         * If the referenced YAML node is an object, this function will return
         * a std::optional containing a YamlCppObject instance referencing the
         * object.
         *
         * Otherwise it will return an empty optional.
         */
        opt::optional<YamlCppObject> getObjectOptional() const
        {
            if (isObject()) {
                return opt::make_optional(YamlCppObject(node));
            }

            return opt::optional<YamlCppObject>();
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
            return node.IsMap() && !node.IsNull();
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

        const YAML::Node node;
};


class YamlCppAdapter:
    public BasicAdapter<YamlCppAdapter,
                        YamlCppArray,
                        YamlCppObjectMember,
                        YamlCppObject,
                        YamlCppNode>
{
    public:

        YamlCppAdapter() : BasicAdapter(), node() { }
        YamlCppAdapter(const YAML::Node node) : BasicAdapter(node), node(node)
            { }


        bool maybeString() const
        {
            return !(node.isObject() || node.isArray()) ;
        }

        std::string asString() const
        {
            std::string result;
            if (asString(result))
            {
                return result;
            }

            throw std::runtime_error("YAML value cannot be cast to string");
        }

        bool asString(std::string &result) const
        {
            if (maybeString())
            {
                node.getString(result);
                return true;
            }

            return false;
        }

    private:
        const YamlCppNode node;
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


/**
 * @brief   Class for iterating over the members belonging to a YAML object.
 *
 * This class provides a YAML object iterator that dereferences as an instance
 * of YamlCppObjectMember representing one of the members of the object. It has
 * been implemented using the boost iterator_facade template.
 *
 * @see YamlCppObject
 * @see YamlCppObjectMember
 */
class YamlCppObjectMemberIterator:
    public std::iterator<
        std::forward_iterator_tag,      // forward iterator
        YamlCppObjectMember>            // value type
{
public:

    /**
     * @brief   Construct an iterator from a YamlCpp iterator.
     *
     * @param   itr  YamlCpp iterator to store
     */
    YamlCppObjectMemberIterator(const YAML::const_iterator &itr) : itr(itr) { }

    /**
     * @brief   Returns a YamlCppObjectMember that contains the key and value
     *          belonging to the object member identified by the iterator.
     */
    YamlCppObjectMember operator*() const
    {
        return YamlCppObjectMember(itr->first.as<std::string>(), itr->second);
    }

    DerefProxy<YamlCppObjectMember> operator->() const
    {
        return DerefProxy<YamlCppObjectMember>(**this);
    }

    /**
     * @brief   Compare this iterator with another iterator.
     *
     * Note that this directly compares the iterators, not the underlying
     * values, and assumes that two identical iterators will point to the same
     * underlying object.
     *
     * @param   rhs  Iterator to compare with
     *
     * @returns true if the underlying iterators are equal, false otherwise
     */
    bool operator==(const YamlCppObjectMemberIterator &rhs) const
    {
        return itr == rhs.itr;
    }

    bool operator!=(const YamlCppObjectMemberIterator &rhs) const
    {
        return !(itr == rhs.itr);
    }

    const YamlCppObjectMemberIterator& operator++()
    {
        itr++;

        return *this;
    }

    YamlCppObjectMemberIterator operator++(int)
    {
        YamlCppObjectMemberIterator iterator_pre(itr);
        ++(*this);
        return iterator_pre;
    }

private:

    /// Iternal copy of the original YamlCpp iterator
    YAML::const_iterator itr;
};


inline bool YamlCppFrozenNode::equalTo(const Adapter &other, bool strict) const
{
    return YamlCppAdapter(node).equalTo(other, strict);
}


inline YamlCppArrayValueIterator YamlCppArray::begin() const
{
    return node.begin();
}


inline YamlCppArrayValueIterator YamlCppArray::end() const
{
    return node.end();
}


inline YamlCppObjectMemberIterator YamlCppObject::begin() const
{
    return node.begin();
}


inline YamlCppObjectMemberIterator YamlCppObject::end() const
{
    return node.end();
}


inline YamlCppObjectMemberIterator YamlCppObject::find(
    const std::string &propertyName) const
{
    for (auto itr = node.begin(); itr != node.end(); ++itr) {
        if (itr->first.as<std::string>() == propertyName) {
            return itr;
        }
    }

    return node.end();
}


/// Specialization of the AdapterTraits template struct for YamlCppAdapter.
template<>
struct valijson::adapters::AdapterTraits<YamlCppAdapter>
{
    typedef YAML::Node DocumentType;

    static std::string adapterName()
    {
        return "YamlCppAdapter";
    }
};


class AsdfSchemaParser : public valijson::SchemaParser
{
    /* Overridden from implementation in Schema parser */
    opt::optional<std::string> findAbsoluteDocumentUri(
            const opt::optional<std::string> resolutionScope,
            const opt::optional<std::string> documentUri)
    {
        namespace internal = valijson::internal;

        std::string base = "/Users/ddavella/asdf/asdf-standard/schemas/stsci.edu/asdf/core/";
        return base + documentUri.value() + ".yaml";
    }
};


const YAML::Node * fetchYamlDoc(const std::string &uri)
{
    YAML::Node temp = YAML::LoadFile(uri);
    YAML::Node *node = new YAML::Node(temp);
    return node;
}


void freeYamlDoc(const YAML::Node *node)
{
    delete node;
}


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "USAGE: %s <schema> <document>\n", argv[0]);
        return 1;
    }

    YAML::Node yaml_schema = YAML::LoadFile(argv[1]);

    valijson::Schema schema;
    AsdfSchemaParser parser;
    YamlCppAdapter schemaAdapter(yaml_schema);
    parser.populateSchema(schemaAdapter, schema, fetchYamlDoc, freeYamlDoc);
}
