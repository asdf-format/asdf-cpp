#pragma once

#include <iostream>
#include <csignal>

#include <yaml-cpp/yaml.h>
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/memory.h"
#include "yaml-cpp/node/detail/node.h"
#include "yaml-cpp/exceptions.h"


namespace Asdf {

/* Forward declarations from other parts of ASDF */
class AsdfFile;
template <typename T> class NDArray;

/* Class definition */
class Node : public YAML::Node
{
    public:
        inline Node() : YAML::Node() {}
        inline Node(AsdfFile *file) : Node() { this->file = file; }

        template <typename Key>
            const Node operator[](const Key& key) const;
        template <typename Key>
            Node operator[](const Key& key);

        /* Inherit support for assignment to Node from YAML::Node */
        using YAML::Node::operator=;

        /* Override assignment operator when assigning NDArray to Node */
        template<typename T, template <typename> class NDArray>
            YAML::Node& operator=(const NDArray<T> &rhs);

        const AsdfFile *get_asdf_file(void) const;

    protected:
        friend class NodeBuilder;
        template <typename T> friend struct YAML::convert;

        AsdfFile *file = nullptr;

        inline Node(Zombie zombie) : YAML::Node(zombie) {}

        /*
         * This constructor is called by the associated "encode" method for
         * NDArray. It allows a pointer to the AsdfFile object contained
         * by the parent Node to be passed to the new Asdf Node when writing
         * NDArray objects to a file.
         */
        explicit Node(
                YAML::detail::node& node,
                YAML::detail::shared_memory_holder pMemory,
                AsdfFile *file) : YAML::Node(node, pMemory)
        {
            this->file = file;
        }
};

/*
 * Override that ensures that top-level AsdfFile object is passed to each new
 * node that is assigned to the tree.
 */
template <typename Key>
inline const Node Node::operator[](const Key& key) const {
  if (!m_isValid)
    throw YAML::InvalidNode();
  EnsureNodeExists();
  YAML::detail::node* value = static_cast<const YAML::detail::node&>(*m_pNode)
                            .get(YAML::detail::to_value(key), m_pMemory);
  if (!value) {
    return Node(ZombieNode);
  }

  return Node(*value, m_pMemory, this->file);
}

/*
 * Override that ensures that top-level AsdfFile object is passed to each new
 * node that is assigned to the tree.
 */
template <typename Key>
inline Node Node::operator[](const Key& key) {
  if (!m_isValid)
    throw YAML::InvalidNode();
  EnsureNodeExists();
  YAML::detail::node& value = m_pNode->get(YAML::detail::to_value(key), m_pMemory);

  return Node(value, m_pMemory, this->file);
}

/*
 * Override that handles block allocation when assigning NDArrays to tree
 * nodes.
 */
template<typename T, template <typename> class NDArray>
inline YAML::Node& Node::operator=(const NDArray<T> &rhs)
{
    /* Use NDArray callback to register array block with AsdfFile object */
    int source = rhs.register_array_block(file);
    YAML::Node &node = YAML::Node::operator=(rhs);

    /* TODO: this needs to be conditioned on whether the array is inline or not */
    node["source"] = source;
    return node;
}

} /* namespace Asdf */
