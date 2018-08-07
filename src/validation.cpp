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


class YamlCppArray
{

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
