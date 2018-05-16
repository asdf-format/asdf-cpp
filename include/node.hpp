#pragma once

#include <iostream>

#include <yaml-cpp/yaml.h>
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/memory.h"
#include "yaml-cpp/node/detail/node.h"
#include "yaml-cpp/exceptions.h"

#include <tags/abstract_ndarray.hpp>

namespace Asdf {

/* Forward declarations from other parts of ASDF */
class AsdfFile;

/* Class definition */
class Node : public YAML::Node
{
    public:
        inline Node() : YAML::Node() {}

        template <typename Key>
            const Node operator[](const Key& key) const;
        template <typename Key>
            Node operator[](const Key& key);

        /* Inherit support for assignment to Node from YAML::Node */
        using YAML::Node::operator=;

        const AsdfFile *get_asdf_file(void) const;

    protected:
        friend class NodeBuilder;
        template <typename T> friend struct YAML::convert;

        AsdfFile *file;

        inline Node(Zombie zombie) : YAML::Node(zombie) {}
        explicit Node(
                YAML::detail::node& node,
                YAML::detail::shared_memory_holder pMemory,
                AsdfFile *file) : YAML::Node(node, pMemory)
        {
            node.set_style(YAML::EmitterStyle::Flow);
            this->file = file;
        }

        /* This constructor is used when creating an NDArray node */
        Node(const AbstractNDArray &array) : Node()
        {
            array.register_array_block(file);
        }
};

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

template <typename Key>
inline Node Node::operator[](const Key& key) {
  if (!m_isValid)
    throw YAML::InvalidNode();
  EnsureNodeExists();
  YAML::detail::node& value = m_pNode->get(YAML::detail::to_value(key), m_pMemory);
  return Node(value, m_pMemory, this->file);
}

} /* namespace Asdf */
