#pragma once

#include <yaml-cpp/yaml.h>
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/memory.h"
#include "yaml-cpp/node/detail/node.h"
#include "yaml-cpp/exceptions.h"


namespace Asdf {
class AsdfFile;

/* Class definition */
class Node : public YAML::Node
{
    public:
        friend class NodeBuilder;
        inline Node() : YAML::Node() {}

        inline void EnsureNodeExists() const;

        template <typename Key>
            inline const Node operator[](const Key& key) const;
        template <typename Key>
            inline Node operator[](const Key& key);

    private:
        bool m_isValid; mutable YAML::detail::shared_memory_holder m_pMemory;
        mutable YAML::detail::node* m_pNode;

        enum Zombie { ZombieNode };
        explicit Node(Zombie);
        inline Node(YAML::detail::node& node, YAML::detail::shared_memory_holder pMemory)
            : m_isValid(true), m_pMemory(pMemory), m_pNode(&node) {}
};

/* Method implementations */
inline void Node::EnsureNodeExists() const {
  if (!m_isValid)
    throw YAML::InvalidNode();
  if (!m_pNode) {
    m_pMemory.reset(new YAML::detail::memory_holder);
    m_pNode = &m_pMemory->create_node();
    m_pNode->set_null();
  }
}

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
  return Node(*value, m_pMemory);
}

template <typename Key>
inline Node Node::operator[](const Key& key) {
  if (!m_isValid)
    throw YAML::InvalidNode();
  EnsureNodeExists();
  YAML::detail::node& value = m_pNode->get(YAML::detail::to_value(key), m_pMemory);
  return Node(value, m_pMemory);
}

} /* namespace Asdf */
