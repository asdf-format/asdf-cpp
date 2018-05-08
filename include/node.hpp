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

        template <typename Key>
            inline const Node operator[](const Key& key) const;
        template <typename Key>
            inline Node operator[](const Key& key);
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
